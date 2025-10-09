#include "main.h"
#include "displaylist.h"
#include "display.h"
#include "taskmanager/taskmanager.h"
#include "adc/adc.h"
#include "RevisionHistory.h" 
//LcdVariableInfo LcdVarsTable[] = {	
//	{&HeartBeat, HEARTBEATSTRING,         FUNC_INT32, RED,   MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 1},
//	{&SliceCnt,					"Tick",FUNC_INT16, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 22, 2}, // Y + 20px
//	{&laserTemperature,			"TEMP", FUNC_FLOAT, WHITE, MAGENTA, 0, DISPLAY_WIDTH - 50, 44, 2}, // Y + 20px
//	{&ADC_Channel[0].adcRaw,	"ADC RAW0", FUNC_INT16, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 64, 2}, // Y + 20px
//	{&ADC_Channel[0].adcAvg,	"ADC AVG0", FUNC_INT16, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 84, 1}, // Y + 11px
//	{&ADC_Channel[0].convAvg,	"ADC COV0", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 95, 1},
//	{&ADC_Channel[1].adcRaw,	"ADC RAW1", FUNC_INT16, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 106, 1},
//	{&ADC_Channel[1].adcAvg,	"ADC AVG1", FUNC_INT16, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 117, 1},
//	// {&ADC_Channel[1].convAvg,	"ADC COV1", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 128, 2},
//	// {&laserTemperature,			"rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2},
//	// {&ADC_Channel[1].convVolt, "ADC VOL1", FUNC_FLOAT, WHITE, MAGENTA, 0},
//	{(uint32_t)0, "--------", FUNC_INT32, WHITE, MAGENTA, 0},
//};
LcdVariableInfo LcdVarsTable[] = {	
	{ &HeartBeat, HEARTBEATSTRING, FUNC_INT32, RED, MAGENTA, 0,			VariableDisplayStart, 2, 1 },
//	{ &SliceCnt, "Tick", FUNC_INT16, WHITE, MAGENTA, 0,					VariableDisplayStart, 22, 2 },
	//{ &laserTemperature, "VOLT", FUNC_FLOAT, WHITE, MAGENTA, 0,			VariableDisplayStart, 15, 2 },
	//{&laserTemperature,			"rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2},
	{ &ADC_Channel[1].adcAvg, "VOLTS", FUNC_TITLE, WHITE, MAGENTA, 0, VariableDisplayStart, 15, 2 },
	{ &ADC_Channel[0].convAvg, "", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart-55, 35, 2 },
	//{ &ADC_Channel[1].adcAvg, "", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart-30, 50, 2 },
	{ &ADC_Channel[1].adcAvg, "AMPS", FUNC_TITLE, WHITE, MAGENTA, 0, VariableDisplayStart, 70, 2 },
	{ &ADC_Channel[0].adcAvg, "", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart-40, 90, 2 },
	//{ &ADC_Channel[0].adcAvg, "ADC AVG0", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 84, 1 },
	//{ &ADC_Channel[0].convAvg, "ADC COV0", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 95, 1 },
//	{ &ADC_Channel[1].adcRaw, "ADC RAW1", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 106, 1 },
	{ &ADC_Channel[1].adcAvg, "ADC AVG1", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 117, 1 },
	// {&ADC_Channel[1].convAvg,	"ADC COV1", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 128, 2},
	// {&laserTemperature,			"rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2},
	// {&ADC_Channel[1].convVolt, "ADC VOL1", FUNC_FLOAT, WHITE, MAGENTA, 0},
	{ (uint32_t)0, "--------", FUNC_INT32, WHITE, MAGENTA, 0 },
};


LcdVariableInfo TaskTimeTable1[] = {
	{ (void*)1,				"TT 1",			FUNC_TITLE, RED, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2 },
	{&HeartBeat,			"HB      ",		FUNC_INT32, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 22, 2},
};

LcdVariableInfo TaskTimeTable2[] = {
	{ (void*)1,				"TT 2",			FUNC_TITLE, RED, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2 },
	{&HeartBeat,			"HB      ",		FUNC_INT32, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 22, 2},
	{(uint32_t)0,			"--------",		FUNC_INT32, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 42, 2},
};
