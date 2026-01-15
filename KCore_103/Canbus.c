#include "canbus.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_can.h>
#include "stdint.h""


uint8_t                        preDefinedAliases[NUM_PRE_DEFINED_ALIASES]; // current list of forced aliases
uint8_t                        userDefinedAliases[NUM_USER_DEFINED_ALIASES]; // current list of user added aliases

uint32_t                    _canTransmitMailboxErrorCount[1][3];
uint32_t                    _canTransmitAbortedPackets[1][3];
canStruct                   _canImmediateRx; // buffer to hold next immediate mode can message to process
bool                     _canImmediateRxIsAvail; // flag to indicate an immediate can message is ready to be processed
canStruct                   _canRx; // buffer to hold next can message to process
rxQueueStruct               _canRxQ;
txQueueStruct               _canTxQ;
uint32_t                    _workingBufferU32[WORKING_BUFFER_SIZE_IN_WORDS]; // local storage for page data to/from canbus

uint16_t                    _extendedSliceTimeNeeded; // down counter of extra slice times needed
uint32_t                    _codeCheckSum;
uint16_t                    _soapPage;
uint32_t                    _soapAddr;

uint16_t					_savedSettingsPage;
uint32_t					_savedSettingsAddr;
uint32_t					_savedSettingsSize;

void canInit(void)
{
	uint8_t i;

	initClkAndResetAPB1(RCC_APB1Periph_CAN1);

	CAN1->MCR &= (~(uint32_t)CAN_MCR_SLEEP); // Exit from sleep mode
	CAN1->MCR |= CAN_MCR_INRQ; // Request initialization
	//waitForCanInitMode(0);
	CAN1->MCR &= ~((uint32_t)(CAN_MCR_TTCM | CAN_MCR_AWUM | CAN_MCR_NART | CAN_MCR_RFLM));
	CAN1->MCR |= (uint32_t)(CAN_MCR_ABOM | CAN_MCR_TXFP);
	// prescale divides APB1 clk down and results needs to be (SJW+BS1+BS2)*desired_bit_rate
#ifdef STM32F10X_HYREL
#define CAN_PRESCALE_VALUE 12  // 48=125Kbps; 12=500Kbps; 6=1Mbit/sec  [APB1_CLK=36MHz]
#elif defined(STM32F4XX_HYREL)
#define CAN_PRESCALE_VALUE 14  // 56=125Kbps; 14=500Kbps; 7=1Mbit/sec  [APB1_CLK=42MHz]
#endif
	CAN1->BTR = (uint32_t)((uint32_t)CAN_Mode_Normal << 30) | \
				((uint32_t)CAN_SJW_1tq << 24) | \
				((uint32_t)CAN_BS1_3tq << 16) | \
				((uint32_t)CAN_BS2_2tq << 20) | \
				((uint32_t)CAN_PRESCALE_VALUE - 1);

	CAN1->MCR &= ~(uint32_t)CAN_MCR_INRQ; // Request leave initialization
	//waitForCanInitMode(1);

	// can filters
	//      filter 00 -- devicePosition (actual physical location
	//      filter 01 -- alias 0
	//      filter 02 -- alias 90 or 100 (all HOTBED vs all HOTHEAD) [80 for all canMotors]
	//      filter 03 -- yoke (10, 20, 30, 40, etc)                  [81,82, etc for all canMotors on a given axis]
	//      filter 04 -- alias aka0 - user controlled
	//      filter 05 -- alias aka1 - user controlled
	//      filter 11 -- alias aka7 - user controlled
	//      filter 12 -- alias aka8 - user controlled
	//      filter 13 -- all immediate commands (ignore device/alias)

	for (i = 0; i < CAN_NUM_FILTERS; i++)
	{
		canInitFilter(CAN1, i, ALIAS_UNUSED, false, false);
	}

	canInitFilter(CAN1, ALIAS_BROADCAST_ALL_FILTER_INDEX, ALIAS_ALL_DEVICES, false, true); // setup filter 1 for alias '0'

	preDefinedAliases[ALIAS_BROADCAST_ALL_FILTER_INDEX] = ALIAS_ALL_DEVICES;

	canInitFilter(CAN1, ALIAS_IMMEDIATE_FILTER_INDEX, 0, true, true);

	// setup interrupt for RX1 fifo -- used for immediate mode commands (no backing fifo, so limit use)
//polled input only look at taskmanager list
	//interruptSetupAndEnable(CAN1_RX1_IRQn, (DEVICE_IS_A_CAN_AXIS_MOTOR) ? 2 : 0);

	CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);
}
void initClkAndResetAPB1(uint32_t periph)
{
	RCC->APB1ENR  &= ~(periph); // disable clock for peripheral
	RCC->APB1RSTR |=  (periph); // set reset for peripheral
	RCC->APB1ENR  |=  (periph); // enable clock for peripheral
	RCC->APB1RSTR &= ~(periph); // remove reset from peripheral
}


void canInitFilter(CAN_TypeDef *CANx, uint8_t filterNum, uint8_t alias, bool immediate, bool enable)
{
#define FMR_FINIT    ((uint32_t)0x00000001) // Filter init mode

	uint32_t filterBit;

	filterBit   = (1 << filterNum);

	CANx->FMR |= FMR_FINIT; // Initialization mode for the filter
	CANx->FA1R &= ~filterBit; // Filter Deactivation

	CANx->FS1R |= filterBit; // 32-bit scale for the filter
	CANx->FM1R &= ~filterBit; // Id+Mask mode for the filter

	if (immediate)
	{
		CANx->FFA1R |= filterBit; // assign FIFO 1 for immediate mode the filters
		CANx->sFilterRegister[filterNum].FR1 = FILTER_CONSTANT_VALUE | FILTER_IMMEDIATE_BIT; // compare value
		CANx->sFilterRegister[filterNum].FR2 = FILTER_MASK_CONSTANT_HEAD; // compare mask
	}
	else
	{
		CANx->FFA1R &= ~filterBit; // assign FIFO 0 for non-immediate mode (buffered) filters
		CANx->sFilterRegister[filterNum].FR1 = FILTER_CONSTANT_VALUE | (alias << FILTER_DEVICE_LSB_POSITION); // compare value
		CANx->sFilterRegister[filterNum].FR2 = (FILTER_MASK_CONSTANT_HEAD | FILTER_MASK_DEVICE); // compare mask
	}

	if (enable == true)
	{
		CANx->FA1R |= filterBit; // Filter Activation
	}

	CANx->FMR &= ~FMR_FINIT;
}

////////////////////////////////////////////////////////////////////////////////

void canAddUserDefinedAlias(uint8_t newAlias, uint8_t aliases[])
{
	uint8_t i;
	// first, find an open slot within the range specified
	for (i = 0; i < NUM_USER_DEFINED_ALIASES; i++)
	{
		if (aliases[i] == ALIAS_UNUSED)
		{
			// filters are ordered predef(4), the user(8)
			canInitFilter(CAN1, NUM_PRE_DEFINED_ALIASES + i, newAlias, false, true);
			aliases[i] = newAlias;
			break;
		}
	}
	if (i == NUM_USER_DEFINED_ALIASES)
	{
		//put error handler here
	}
}

////////////////////////////////////////////////////////////////////////////////

void canRemoveUserDefinedAlias(uint8_t oldAlias, uint8_t aliases[])
{
	uint8_t i;

	// find matching alias and disable it
	for (i = 0; i < NUM_USER_DEFINED_ALIASES; i++)
	{
		if (aliases[i] == oldAlias)
		{
			// filters are ordered predef(4), the user(8)
			canInitFilter(CAN1, NUM_PRE_DEFINED_ALIASES + i, ALIAS_UNUSED, false, false);
			aliases[i] = ALIAS_UNUSED;
			break;
		}
	}
}



////////////////////////////////////////////////////////////////////////////////

bool canIsValidAlias(uint8_t device)
{
	uint8_t i;

	for (i = 0; i < NUM_PRE_DEFINED_ALIASES; i++)
	{
		if (preDefinedAliases[i] == device)    // match!!! so it is for us
		{
			return (true);
		}
	}

	for (i = 0; i < NUM_USER_DEFINED_ALIASES; i++)
	{
		if (userDefinedAliases[i] == device)   // match!!! so it is for us
		{
			return (true);
		}
	}
	return (false);
}