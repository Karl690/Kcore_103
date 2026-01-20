#include "canbus.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_can.h>
#include "stdint.h"
#include "string.h"
#include "adc/adc.h"


uint8_t                        preDefinedAliases[NUM_PRE_DEFINED_ALIASES]; // current list of forced aliases
uint8_t                        userDefinedAliases[NUM_USER_DEFINED_ALIASES]; // current list of user added aliases
uint16_t				HydraCanAddress = 0;
uint8_t 				CanTransmitMailbox;
uint32_t 				CanTrasmitMsgWaitCounter = 0;
uint8_t					CanRxTargetId;
uint8_t					CanRxSourceId;
uint8_t					CanRxPage;
uint8_t					CanRxMsgType;
uint8_t					CanRxImmediateFlag = 0;


uint16_t				CanRxInIndex = 0;
uint16_t				CanRxOutIndex = 0;

CANMsg 					CanRxMsgBuffer[CAN_MSG_BUFFER_SIZE];
uint16_t				CanTxQueHead = 0;
uint16_t				CanTxQueTail = 0;
uint16_t				CanTxWaitCount = 0;
CANMsg 					CanTxMsgBuffer[CAN_MSG_BUFFER_SIZE];
payloadUnion			CanTxPayLoad = { 0 };


uint8_t 				RemoteSoapString[0x400] = { 0 };

CANMsg					CanLastRxBuffer = { 0 };
CANMsg					CanLastTxBuffer = { 0 };
payloadUnion			TxWorkData = { 0xff };
CanRxMsg 				CanRxMessage;
CanTxMsg 				CanTxMessage;
uint16_t		CanRxLedCountDown = 0;
uint16_t		CanTxLedCountDown = 0;

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
void ProcessCanTxMessage(void)
{
	if (CanTxQueHead == CanTxQueTail) return;
	CANMsg* pBuffer = &CanTxMsgBuffer[CanTxQueTail];
	if (SendCanMessage(pBuffer->ID, &pBuffer->PayLoad.u8[0], pBuffer->DataSize) == SUCCESS) 
	{
		CanTxQueTail++;
		memcpy(&CanLastRxBuffer, pBuffer, sizeof(CANMsg));
		CanTxQueTail &= CAN_MSG_QUE_CNT_MASK;
		return;
	}
	CanTxWaitCount++; //keep track of number of rejected messages
}

void CheckCanRxMessages(void)
{
	//there are 2 canrx buffers in the stm chip, so we need to check each one
	if (CAN1->RF0R & 0x3) //is there a message in the rx buffer 0
	{
		CAN_Receive(CAN1, CAN_FIFO0, &CanRxMessage);
	}
	else if (CAN1->RF1R & 0x3) //is there a message in the rx buffer 1
	{
		CAN_Receive(CAN1, CAN_FIFO1, &CanRxMessage);
	}
	else return;//nothing is waiting, so return 
//at this moment, CanRxMessage struct has been processed.
	CanRxTargetId = (CanRxMessage.ExtId >> 20) & 0xFF;
	//CanRxSourceId = (CanRxMessage.ExtId >> 12) & 0xFF;
	if (CanRxTargetId == CAN_BROADCAST_ADDRESS || isFilteredAddress(CanRxTargetId)) //if Head Address is same as Can message Head identifier.
	{
		CanRxLedCountDown = LED_ON_MAXCOUNT;
		CANMsg* pInBuffer = &CanRxMsgBuffer[CanRxInIndex];

		pInBuffer->ID = CanRxMessage.ExtId;
		memcpy(pInBuffer->PayLoad.i8, CanRxMessage.Data, 8);
		pInBuffer->DataSize = CanRxMessage.DLC;

		pInBuffer->Immediate = CanRxMessage.ExtId & 0x10000000 ? 1 : 0;
		pInBuffer->MsgType = (CanRxMessage.ExtId & 0x800) ? CAN_WRITE : CAN_READ;
		pInBuffer->MsgId = ((CanRxMessage.ExtId &  0xFF0) >> 4) & 0x7F;
		pInBuffer->Page = CanRxMessage.ExtId & 0xF;
		pInBuffer->SourceAddress = (CanRxMessage.ExtId >> 12) & 0xFF;
		pInBuffer->TargetAddress = (CanRxMessage.ExtId >> 20) & 0xFF;
		CanRxInIndex++;
		if (CanRxInIndex >= CAN_MSG_BUFFER_SIZE) CanRxInIndex = 0;
	}
}
//////////////////////////////////////////////////
//this function is for sending message thru CAN.
//
// param:
//		target is a head number to send message.
//		funcId means the function identifier of message.
//		data is buffer to send. it must be less than 8bytes.
//		size is the length of buffer.


uint8_t SendCanMessage(uint32_t id, uint8_t* data, uint8_t size)
{
	uint8_t TxMailBoxIndex = CAN_TxStatus_NoMailBox;
	/* Select one empty transmit mailbox */
	if ((CAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
	{
		TxMailBoxIndex = 0;
	}
	else if ((CAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
	{
		TxMailBoxIndex = 1;
	}
	else if ((CAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
	{
		TxMailBoxIndex = 2;
	}
	else
	{
		return ERROR;
	}
	CAN1->sTxMailBox[TxMailBoxIndex].TIR &= 1; //TMIDxR_TXRQ;
	CAN1->sTxMailBox[TxMailBoxIndex].TIR |= ((id << 3) | \
												CAN_Id_Extended | \
													CAN_RTR_Data);

	/* Set up the DLC */

	CAN1->sTxMailBox[TxMailBoxIndex].TDTR &= (uint32_t)0xFFFFFFF0;
	CAN1->sTxMailBox[TxMailBoxIndex].TDTR |= size; //Data Size

	/* Set up the data field */
	CAN1->sTxMailBox[TxMailBoxIndex].TDLR = (((uint32_t)data[3] << 24) |
											 ((uint32_t)data[2] << 16) |
											 ((uint32_t)data[1] << 8) |
											 ((uint32_t)data[0]));
	CAN1->sTxMailBox[TxMailBoxIndex].TDHR = (((uint32_t)data[7] << 24) |
											 ((uint32_t)data[6] << 16) |
											 ((uint32_t)data[5] << 8) |
											 ((uint32_t)data[4]));
	/* Request transmission */
	CAN1->sTxMailBox[TxMailBoxIndex].TIR |= 1; //TMIDxR_TXRQ;
	CanTxLedCountDown = LED_ON_MAXCOUNT;
	return SUCCESS;
}
void ProcessCanRxMessage(void)
{	
	if (CanRxOutIndex == CanRxInIndex) return;//if nothing in the rx que, return
	CANMsg* pOutBuffer = &CanRxMsgBuffer[CanRxOutIndex]; //point to current messge
	memcpy(&CanLastRxBuffer, pOutBuffer, sizeof(CANMsg));
	uint16_t sender = pOutBuffer->SourceAddress; //who sent this message
	uint16_t Target = pOutBuffer->TargetAddress; //who it was inteneded to go to, 0xff means ALL

	uint16_t address = 0;
	uint16_t arg1, arg2, arg3, arg4;
	uint8_t channelOffset = (uint8_t)pOutBuffer->Page & 0x1F; //32 channels maximum
	//uint8_t channelOffset = sender&0x1F;//32 channels maximum
//	uint8_t prgVariableIndex = channelOffset;//second variable for clarity
	//
	arg1 = pOutBuffer->PayLoad.i16[0]; //assemble 16bit arg from first 2 bytes
	arg2 = pOutBuffer->PayLoad.i16[1];
	arg3 = pOutBuffer->PayLoad.i16[2];
	arg4 = pOutBuffer->PayLoad.i16[3];
	
	switch (pOutBuffer->MsgType)
	{
	case CAN_WRITE:
		switch (pOutBuffer->MsgId)
		{

		//case CAN_MSG_WRITE_SOAPSTRING: break;
		case CAN_MSG_DEVICE_INIT: break;//it is a response on slave

		}
		break;
	case CAN_READ:
		switch (pOutBuffer->MsgId)
		{
		case CAN_MSG_MOTOR_ENABLE:
//			sprintf(SendString, ">RG:T%d\n", (int)pOutBuffer->SourceAddress);
//			sendString(SendString);
			break;

//		case CAN_MSG_READ_SOAPSTRING:
//			if (pOutBuffer->SourceAddress != AmplifierMasterAddress) {
//				//read one's soap
//				uint16_t Address = (pOutBuffer->PayLoad.i16[0]) * 6;
//				if (Address == 0) memset(RemoteSoapString, 0, 0x400);
//				for (uint16_t i = 2; i < 8; i++)
//				{
//					RemoteSoapString[Address + i - 2] = pOutBuffer->PayLoad.i8[i];
//					if (RemoteSoapString[Address + i - 2] == 0x0 || RemoteSoapString[Address + i - 2] == 0xFF)
//					{
//
//						sprintf(SendString, ">RI:%d:SOP:%s\n", pOutBuffer->SourceAddress, RemoteSoapString);
//						sendString(SendString);
//						break;
//					}
//				}
//			}

//			break;
		case CAN_MSG_LOOPBACK:
			CanAddTxBuffer(HydraCanAddress, CanHeadAddress, CAN_READ, CAN_MSG_LOOPBACK, pOutBuffer->Page, 0, 0, 0);
			break;
		}
		break;
	}
	CanRxOutIndex++;
	CanRxOutIndex &= CAN_MSG_QUE_CNT_MASK;
}
void CanAddTxBuffer(uint16_t target, uint16_t Source, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate, payloadUnion* data, uint8_t size)
{	//karlchris
	//add check to see if que is full   if cantxquehead++ &0x1f == cantxquetail then que is full
	CANMsg* pOutBuffer = &CanTxMsgBuffer[CanTxQueHead];
	pOutBuffer->ID = GenerateFrameID(target, Source, msgType, msgId, page, immediate);

	if (data && size > 0) memcpy(pOutBuffer->PayLoad.i8, data, size);
	pOutBuffer->DataSize = size;
	pOutBuffer->Immediate = immediate;
	pOutBuffer->MsgType = msgType;
	pOutBuffer->MsgId = msgId;
	pOutBuffer->Page = page;
	pOutBuffer->SourceAddress = Source;
	pOutBuffer->TargetAddress = target;

	CanTxQueHead++;
	CanTxQueHead &= CAN_MSG_QUE_CNT_MASK;
}
uint32_t GenerateFrameID(uint16_t target, uint16_t SourceAddress, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate)
{
	return (uint32_t)((immediate << 28) 
		+ 	(target << 20) 
		+ 	((uint32_t)SourceAddress << 12) 
		+ 	(msgType << 11) 
		+ 	(msgId << 4) 
		+ 	(page & 0x0f));//page can only be 4 bits 0-16
}
void CanAddTxBuffer8x8Args(uint16_t target, uint16_t SourceAddress, uint8_t msgType, uint8_t msgId, uint8_t page, 
	uint8_t arg1, uint8_t arg2, uint8_t arg3, uint8_t arg4,uint8_t arg5, uint8_t arg6, uint8_t arg7, uint8_t arg8)
{
	CanTxPayLoad.u8[0] = arg1;
	CanTxPayLoad.u8[1] = arg2;
	CanTxPayLoad.u8[2] = arg3;
	CanTxPayLoad.u8[3] = arg4;
	CanTxPayLoad.u8[4] = arg5;
	CanTxPayLoad.u8[5] = arg6;
	CanTxPayLoad.u8[6] = arg7;
	CanTxPayLoad.u8[7] = arg8;
	CanAddTxBuffer(target, SourceAddress, msgType, msgId, page, 0, &CanTxPayLoad, 8);
}
void CanAddTxBuffer4x16Args(uint16_t target, uint16_t SourceAddress, uint8_t msgType, uint8_t msgId, uint8_t page, uint16_t arg1, uint16_t arg2, uint16_t arg3, uint16_t arg4)
{
	CanTxPayLoad.u16[0] = arg1;
	CanTxPayLoad.u16[1] = arg2;
	CanTxPayLoad.u16[2] = arg3;
	CanTxPayLoad.u16[3] = arg4;
	CanAddTxBuffer(target, SourceAddress, msgType, msgId, page, 0, &CanTxPayLoad, 8);
}
void CanAddTxBuffer2x32Args(uint16_t target, uint16_t SourceAddress, uint8_t msgType, uint8_t msgId, uint8_t page, uint32_t arg1, uint32_t arg2)
{
	CanTxPayLoad.u32[0] = arg1;
	CanTxPayLoad.u32[1] = arg2;
	CanAddTxBuffer(target, SourceAddress, msgType, msgId, page, 0, &CanTxPayLoad, 8);
}
void CanAddTxBuffer1x64Args(uint16_t target, uint16_t SourceAddress, uint8_t msgType, uint8_t msgId, uint8_t page, uint64_t arg1)
{
	CanTxPayLoad.u64 = arg1;
	CanAddTxBuffer(target, SourceAddress, msgType, msgId, page, 0, &CanTxPayLoad, 8);
}
void CanAddStringTxBuffer(uint16_t target, uint16_t SourceAddress, uint8_t msgType, uint8_t msgId, uint8_t page, char* WorkString)
{
	for (int i=0;i<8;i++)
	{//transfers up to 8 bytes of string into can payload
			if (WorkString[i] > 0)
			{
				CanTxPayLoad.u8[i] = WorkString[i];
			}
			else
			{
				while (i < 8)
				{//fill extra payload byes with nulls
					CanTxPayLoad.u8[i] = 0; //end of the string
					i++;
				}
			}
		}	
	CanAddTxBuffer(target, SourceAddress, msgType, msgId, page, 0, &CanTxPayLoad, 8);
}

////////////////////////////////////////////////////////////////////////////////