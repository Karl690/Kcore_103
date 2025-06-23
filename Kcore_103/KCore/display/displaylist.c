#include "main.h"
#include "displaylist.h"
#include "display.h"

LcdVariableInfo LcdVarsTable[] = {	
	{ (void*)1, "MAIN", FUNC_TITLE, RED, MAGENTA, 0 },
	{&HeartBeat, "HB     ", FUNC_INT32, WHITE, MAGENTA, 0},
	
	{(uint32_t)0, "--------", FUNC_INT, WHITE, MAGENTA, 0},
};


LcdVariableInfo TaskTimeTable1[] = {
	{&HeartBeat, "HB      ", FUNC_INT, WHITE, MAGENTA, 0},
};

LcdVariableInfo TaskTimeTable2[] = {
	{&HeartBeat, "HB      ", FUNC_INT, WHITE, MAGENTA, 0},
	{(uint32_t)0, "--------", FUNC_INT, WHITE, MAGENTA, 0},
};
