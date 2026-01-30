#include "main.h"
#include "canbus.h"
#ifdef __cplusplus
extern "C"
#endif

uint8_t Booted = 0;
uint8_t DisplayIndex = 0;
uint16_t lastCanHeadAddress = 0xff;
uint16_t CanWatchdogtimer = 0;//setup the canwatchdog timer for 10 second reset 
void SetupIWDG(uint32_t interval)
{
	// if goal is a 5 second watchdog....CNT = 5 * 125 = 625 nom.... (could be off by a factor of 2
	// due to mfg tolerance. not worth calibrating, so double result to be safe ... so scaling
	// is 32000 / 256 * 2 = 250

	IWDG->KR = IWDG_WriteAccess_Enable; // Enable write access to IWDG_PR and IWDG_RLR register
	IWDG->PR = IWDG_Prescaler_256; // prescale from nom 32KHz clock to nom 125Hz (8ms period)
	IWDG->RLR = (uint16_t)(interval * 250); // set reload value/count
	IWDG->KR = IWDG_KR_KEY_Reload; // Reload IWDG counter
	IWDG->KR = IWDG_KR_KEY_Enable; // turn on watchdog ... can't be disable
}

void registerDeviceAndCheckCommWatchdog(void)
{
	if (CanWatchdogtimer == 0)
	{
		if (CanHeadAddress == lastCanHeadAddress)
		{
			// continue trying to register once every second until we get some feedback
			payloadUnion payload;
			//LoadPayloadWithDeviceInfo(1, &payload);
			payload.u32[0] = 0x30303034;
			payload.u32[1] = 0x30304131; //hard coded uvata light pen for now
			canPackIntoTxQueue8x8(CAN_WRITE, CanHeadAddress, CAN_MSG_EVENT_MESSAGE, CAN_EVENT_DEVICE_ANNOUNCE, false, (byte *)&payload);
		
			//SET_SENT_ANNOUNCE_BIT; // log that we sent the request for later processing
			//updateBootSequence(BOOT_STAGE_SENT_ANNOUNCE);
		}
		else
		{
			lastCanHeadAddress = CanHeadAddress; //update for next check address, must be stable for 2 seconds			
		}
	}
	
}

int main(void)
{
	tim1_init();
	gpio_init();
	
	DisplayInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	//adc_init();
	canInit();
	Init_ADC_DMA_Circular_Mode(); //setup adc in dma mode for autmatic conversion
	Start_ADC_IN_CONTNOUS_DMA_MODE();
	SysTick_Config(SystemCoreClock / SYSTICKS_PER_SECOND);
	
	Booted = 1;
	while (1)
	{
		DisplayIndex %= 3;
		switch (DisplayIndex)
		{
		case 0:UpdateScreen(LcdVarsTable); break;
		case 1:UpdateScreen(TaskTimeTable1); break;
		case 2:UpdateScreen(TaskTimeTable2); break;
		}
		//delay_sec(1);
		//pinToggleOutput(PIN_LED_175);
	}
}


void SysTick_Handler(void)
{
	func_SystickCallback();
}