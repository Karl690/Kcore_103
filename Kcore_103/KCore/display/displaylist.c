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
//LcdVariableInfo LcdVarsTable[] = {	
//	{ &HeartBeat, HEARTBEATSTRING, FUNC_INT32, RED, MAGENTA, 0,			VariableDisplayStart, 5, 1 },
////	{ &SliceCnt, "Tick", FUNC_INT16, WHITE, MAGENTA, 0,					VariableDisplayStart, 22, 2 },
//	//{ &laserTemperature, "VOLT", FUNC_FLOAT, WHITE, MAGENTA, 0,			VariableDisplayStart, 15, 2 },
//	//{&laserTemperature,			"rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2},
//	{ &ADC_Channel[1].adcAvg, "--VOLTS--", FUNC_TITLE, WHITE, MAGENTA, 0, 10, 20, 2 },
//	{ &ADC_Channel[0].convAvg, "", FUNC_FLOAT, WHITE, MAGENTA, 0, 10, 40, 2 },
//	//{ &ADC_Channel[1].adcAvg, "", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart-30, 50, 2 },
//	{ &ADC_Channel[1].adcAvg, "--AMPS--", FUNC_TITLE, WHITE, MAGENTA, 0, 15, 70, 2 },
//	{ &ADC_Channel[0].adcAvg, "", FUNC_INT16, WHITE, MAGENTA, 0, 35, 90, 2 },
//	//{ &ADC_Channel[0].adcAvg, "ADC AVG0", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 84, 1 },
//	//{ &ADC_Channel[0].convAvg, "ADC COV0", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 95, 1 },
////	{ &ADC_Channel[1].adcRaw, "ADC RAW1", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 106, 1 },
//	{ &ADC_Channel[1].adcAvg, "ADC AVG1", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 115, 1 },
//	// {&ADC_Channel[1].convAvg,	"ADC COV1", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 128, 2},
//	// {&laserTemperature,			"rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2},
//	// {&ADC_Channel[1].convVolt, "ADC VOL1", FUNC_FLOAT, WHITE, MAGENTA, 0},
//	{ (uint32_t)0, "--------", FUNC_INT32, WHITE, MAGENTA, 0 },
//};
//LcdVariableInfo LcdVarsTable[] = {	
//	{ &HeartBeat, HEARTBEATSTRING, FUNC_INT32, RED, MAGENTA, 0, VariableDisplayStart, 5, 1 },
//	//	{ &SliceCnt, "Tick", FUNC_INT16, WHITE, MAGENTA, 0,					VariableDisplayStart, 22, 2 },
//		{ &laserTemperature, "Temp", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 15, 1 },
//		//{&laserTemperature,			"rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2},
//	{ &ADC_Channel[0].adcAvg, "PB1 POS",	FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 25, 1 },
//	{ &ADC_Channel[0].convVolt, "PB1 volt", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 35, 1 },	
//	{ &ADC_Channel[1].adcAvg, "PA0 RTD",	FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 45, 1 },
//	{ &ADC_Channel[1].convVolt, "PA0 volt", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 55, 1 },
//	{ &ADC_Channel[2].adcAvg, "PA1 Raw",	FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 65, 1 },
//	{ &UvataVoltage, "Uvata VOLT", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 75, 1 },
//	{ &ADC_Channel[3].adcAvg, "PA3 ISET",	FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 85, 1 },
//	{ &ADC_Channel[3].convVolt, "PA3 volt", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 95, 1 },
//	{ &CurrentSetPoint, "I_SET",			FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 105, 1 },
//	//{ &ADC_Channel[0].adcAvg, "ADC AVG0", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 84, 1 },
//	//{ &ADC_Channel[0].convAvg, "ADC COV0", FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 95, 1 },
////	{ &ADC_Channel[1].adcRaw, "ADC RAW1", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 106, 1 },
//	// {&ADC_Channel[1].convAvg,	"ADC COV1", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 128, 2},
//	// {&laserTemperature,			"rtd temp", FUNC_FLOAT, WHITE, MAGENTA, 0,	DISPLAY_WIDTH - 50, 2, 2},
//	// {&ADC_Channel[1].convVolt, "ADC VOL1", FUNC_FLOAT, WHITE, MAGENTA, 0},
//	{ (uint32_t)0, "--------", FUNC_INT32, WHITE, MAGENTA, 0 },
//};
LcdVariableInfo LcdVarsTable[] = {	

	{ &ADC_Channel[1].adcAvg, "DUTY", FUNC_TITLE, WHITE, MAGENTA, 0, VariableDisplayStart - 40, 10, 2 },
	{ &UvataDuty, "",					FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart - 40, 30, 2 },
	{ &UvataVoltage, "",				FUNC_TITLE, WHITE, MAGENTA, 0, 0, 50, 1 },
	//
	{ &UvataVoltage, "VOLTS",			FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 60, 1 },	
	{ &CanHeadAddress,		"Can ADDR", FUNC_INT16, WHITE, MAGENTA, 0, VariableDisplayStart, 70, 1 },
	{ &CurrentSetPoint, "ISET",			FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 80, 1 },
	{ &laserTemperature, "Temp",		FUNC_FLOAT, WHITE, MAGENTA, 0, VariableDisplayStart, 90, 1 },
	{ &HeartBeat, HEARTBEATSTRING, FUNC_INT32, RED, MAGENTA, 0, VariableDisplayStart, 100, 1 },
	{ (uint32_t)0, "--------",			FUNC_INT32, WHITE, MAGENTA, 0, VariableDisplayStart , 110,1 },
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
