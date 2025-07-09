#pragma once
#include <stdint.h>

typedef enum {
	FUNC_TITLE,
	FUNC_INT32,
	FUNC_INT16,
	FUNC_FLOAT,
	FUNC_HEX8,
	FUNC_HEX16,
	FUNC_HEX32,
	FUNC_ASCII,
	FUNC_BOOLEAN,
	FUNC_MEMDUMPASCII,
	FUNC_MEMDUMPHEX,
}DISPLAYFUNCTYPE;

typedef struct {
	void* 		VariablePointer; //points to the variable that holds the data we want to display
	char* 			Label; //[15];
	DISPLAYFUNCTYPE FuncType; //function type how we show the data, i.e. hex, int, float
	uint16_t		Color_1; // this is for Label or background in Bar.
	uint16_t			Color_2; // this is for Value or progress bar in Bar.
	uint32_t 		Offset; //for FUNC_MEMDUMPASCII if string is array variable(char a[]), it would be 1 otherwise 0. it is only for memory ascii function
							//for FUNC_MEMDUMPHEX it means offset.
} LcdVariableInfo;


extern uint32_t HeartBeat;
extern LcdVariableInfo LcdVarsTable[];
extern LcdVariableInfo TaskTimeTable1[];
extern LcdVariableInfo TaskTimeTable2[];

