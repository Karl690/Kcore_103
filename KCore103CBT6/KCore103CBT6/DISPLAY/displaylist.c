#include "main.h"
#include "ADC/adc.h"
#include "displaylist.h"


LcdVariableInfo LcdVarsTable[] = {	
	{ (void*)1,   "MAIN",     FUNC_TITLE, RED, MAGENTA, 0 },
	{&HeartBeat, "HB     ", FUNC_INT32, WHITE, MAGENTA, 0},
	{&SliceCnt, "HB     ", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[0].adcRaw, "ADC1 RAW0", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[0].convAvg, "ADC Volt", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[0].calcValue, "ADC Temp", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[1].adcRaw, "ADC2 RAW", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[1].convAvg, "ADC Volt", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[1].calcValue, "ADC Temp", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[2].adcRaw, "ADC2 RAW", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[2].convAvg, "ADC Volt", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[2].calcValue, "ADC Temp", FUNC_INT16, WHITE, MAGENTA, 0},
	{(uint32_t)0, "--------", FUNC_INT32, WHITE, MAGENTA, 0},
};


LcdVariableInfo TaskTimeTable1[] = {
	{ (void*)1, "TT 1", FUNC_TITLE, RED, MAGENTA, 0 },
	{&HeartBeat, "HB      ", FUNC_INT32, WHITE, MAGENTA, 0},
};

LcdVariableInfo TaskTimeTable2[] = {
	{ (void*)1, "TT 2", FUNC_TITLE, RED, MAGENTA, 0 },
	{&HeartBeat, "HB      ", FUNC_INT32, WHITE, MAGENTA, 0},
	{(uint32_t)0, "--------", FUNC_INT32, WHITE, MAGENTA, 0},
};
