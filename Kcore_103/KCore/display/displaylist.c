#include "main.h"
#include "displaylist.h"
#include "display.h"
#include "taskmanager/taskmanager.h"
#include "adc/adc.h"
#include "RevisionHistory.h" 
LcdVariableInfo LcdVarsTable[] = {	
	{&HeartBeat, HEARTBEATSTRING,         FUNC_INT32, RED,   MAGENTA, 0},
	{&SliceCnt,				  "Slice Cnt",FUNC_INT16, WHITE, MAGENTA, 0},
	{&laserTemperature,		  "LASER_TMP", FUNC_FLOAT, WHITE, MAGENTA, 0},
	{&ADC_Channel[0].adcRaw,  "ADC RAW0", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[0].adcAvg,  "ADC AVG0", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[0].convAvg, "ADC COV0", FUNC_FLOAT, WHITE, MAGENTA, 0},
	// {&ADC_Channel[0].convVolt, "ADC VOL0", FUNC_FLOAT, WHITE, MAGENTA, 0},
	{&ADC_Channel[1].adcRaw,  "ADC RAW1", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[1].adcAvg,  "ADC AVG1", FUNC_INT16, WHITE, MAGENTA, 0},
	{&ADC_Channel[1].convAvg, "ADC COV1", FUNC_FLOAT, WHITE, MAGENTA, 0},
	{&laserTemperature, "rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0},
	// {&ADC_Channel[1].convVolt, "ADC VOL1", FUNC_FLOAT, WHITE, MAGENTA, 0},
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
