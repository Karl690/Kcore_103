#include "main.h"
#include "displaylist.h"
#include "display.h"
#include "taskmanager/taskmanager.h"

LcdVariableInfo LcdVarsTable[] = {	
	{ (void*)1,   "MAIN",     FUNC_TITLE, RED, MAGENTA, 0 },
	{&HeartBeat,  "HB     ",  FUNC_INT32, WHITE, MAGENTA, 0},
	{&SliceCnt,   "HB     ",  FUNC_INT16, WHITE, MAGENTA, 0},
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
