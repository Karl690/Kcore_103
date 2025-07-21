#include "main.h"
#include "taskmanager.h"
#include "adc/adc.h"

uint16_t lastTaskCnt1000 = 0;
uint16_t lastTaskCnt100 = 0;
uint16_t lastTaskCnt10 = 0;
uint16_t lastTaskCnt1 = 0;
uint16_t TaskTime[32] = { 0 }; // last execution time
uint16_t MaxTaskTime[32] = { 0 }; // Maximum execution time
uint16_t SliceOffset = 0;
uint16_t SliceCnt = 0; // current slice being processed
uint32_t HeartBeat = 0;
uint32_t tickCount = 0;
const PFUNC F1000HZ[NUM_1000HZ] =
{
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
};

const PFUNC F100HZ[NUM_100HZ] =
{
	Spare, 
	ProcessRawADC_Data, // reading ADC raw data
	Spare,
	Spare,
	Spare,
	Spare, 
	Spare,
	Spare,
};

const PFUNC F10HZ[NUM_10HZ] =
{
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
};

const PFUNC F1HZ[NUM_1HZ] =
{
	Spare,
	BlinkHeartBeat,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
};

void Spare(void) {}


void func_SystickCallback()
{
	//HEARTBEAT_SET;
	if (!Booted) return;

	// TIM6->CNT = 0; //reset the slice timer
	//CheckForUart6TxRx();
	SliceCnt++;
	tickCount++;
	SliceOffset = SliceCnt & 0x0007; //precalculate the slice index into the jump table
	if (SliceOffset)
	{
		//reall 6.4khz
		F1000HZ[SliceOffset](); //run the task
//		if (LastSliceTimeFlag)TaskTime[SliceOffset] = TIM6->CNT;
//		else
//		{
//		if (TIM6->CNT > TaskTime[SliceOffset])
//		{
//			TaskTime[SliceOffset] = TIM6->CNT;
//		}	
		//		}
		
//		if ((COM2_SC01.UartHandler->SR & USART_SR_TXE))	checkForUart2Tx();
//		if ((COM6_SSD.UartHandler->SR & USART_SR_TXE))	checkForUart6Tx();

		return;//toggle pin so we can see on Oscillosclope and exit
	}
	if (SliceCnt & 0x0038)
	{
		//really 800 hz
		//HEARTBEAT_SET;
		SliceOffset = (SliceCnt >> 3) & 0x0007;
		F100HZ[SliceOffset]();
		SliceOffset += 8;
		// if (TIM6->CNT > TaskTime[SliceOffset]) TaskTime[SliceOffset] = TIM6->CNT;
		//HEARTBEAT_CLR;
		//HEARTBEAT_CLR;
		return;//toggle pin so we can see on Oscillosclope and exit
	}

	if (SliceCnt & 0x01C0)
	{
		//12.5 hz
		SliceOffset = (SliceCnt >> 6)  & 0x0007;
		F10HZ[SliceOffset]();
		SliceOffset += 16;
		// if (TIM6->CNT > TaskTime[SliceOffset]) TaskTime[SliceOffset] = TIM6->CNT;
		//HEARTBEAT_CLR;
		return;//toggle pin so we can see on Oscillosclope and exit
	}
//1.5625 hz
	SliceOffset = (SliceCnt >> 9)  & 0x0007;
	F1HZ[SliceOffset]();
	SliceOffset += 24;
//	if (TIM6->CNT > TaskTime[SliceOffset])
//	{
//		TaskTime[SliceOffset] = TIM6->CNT;
//	}
	//HEARTBEAT_CLR;
	return;//toggle pin so we can see on Oscillosclope and exit

}

void BlinkHeartBeat()
{
	HeartBeat++;
	pinToggleOutput(PIN_LED_HEARTBEAT);
//	pinToggleOutput(PIN_LED_50);
//	pinToggleOutput(PIN_LED_100);
//	pinToggleOutput(PIN_LED_150);
//	pinToggleOutput(PIN_LED_175);
}

void ClearSliceTimes()
{
	memset(TaskTime, 0, 32 * sizeof(uint16_t));
	memset(MaxTaskTime, 0, 32 * sizeof(uint16_t));
}