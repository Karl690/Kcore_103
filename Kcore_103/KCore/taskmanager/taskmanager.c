#include "main.h"
#include "taskmanager.h"
#include "adc/adc.h"
uint16_t SliceOffset = 0;
uint16_t SliceCnt = 0; // current slice being processed
uint32_t HeartBeat = 0;
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
	taskmanager_blink_led,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
	Spare,
};

void Spare(void) {}


void taskmanager_callback()
{
	if (!isBooted) return;

	SliceCnt++;
	SliceOffset = SliceCnt & 0x0007; //precalculate the slice index into the jump table
	if (SliceOffset)
	{
		F1000HZ[SliceOffset](); //run the task
		return;//toggle pin so we can see on Oscillosclope and exit
	}
	if (SliceCnt & 0x0038)
	{
		//really 800 hz
		//HEARTBEAT_SET;
		SliceOffset = (SliceCnt >> 3) & 0x0007;
		F100HZ[SliceOffset]();
		SliceOffset += 8;
		return;//toggle pin so we can see on Oscillosclope and exit
	}

	if (SliceCnt & 0x01C0)
	{
		//12.5 hz
		SliceOffset = (SliceCnt >> 6)  & 0x0007;
		F10HZ[SliceOffset]();
		SliceOffset += 16;
		return;//toggle pin so we can see on Oscillosclope and exit
	}
//1.5625 hz
	SliceOffset = (SliceCnt >> 9)  & 0x0007;
	F1HZ[SliceOffset]();
	SliceOffset += 24;
	return;//toggle pin so we can see on Oscillosclope and exit
}

void taskmanager_blink_led()
{
	HeartBeat++;
	pinToggleOutput(PIN_LED_50);
}