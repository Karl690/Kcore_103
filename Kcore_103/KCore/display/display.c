#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "gui.h"
#include "display.h"
#include "displaylist.h"
char strTempVal[40] = {0};
uint16_t idx = 0;
LcdVariableInfo *varInfo  = NULL;


void Format_Int(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE) ,varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "%d ", (int)(*((uint32_t*)varInfo->VariablePointer)));
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}
void Format_Int16(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE) ,varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "%d ", (int)(*((uint16_t*)varInfo->VariablePointer)));
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}

void Format_Hex8(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE), varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "0x%02x ", (uint8_t)(*((uint8_t*)varInfo->VariablePointer)));
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}
void Format_Hex16(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE), varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "0x%04x ", (uint16_t)(*((uint16_t*)varInfo->VariablePointer)));
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}
void Format_Hex32(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE), varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "0x%08x ", (int)(*((uint32_t*)varInfo->VariablePointer)));
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}

void Format_Ascii(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE), varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "%s ", (char*)varInfo->VariablePointer);
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}

void Format_Float3_3(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE),varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "%.3f ", (float)(*((float*)varInfo->VariablePointer)));
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}

void Format_MemoryDumpAscii(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	memcpy(strTempVal, (char*)varInfo->VariablePointer, 16);//copy ram to display string portrait
	strTempVal[16]=0;//add null to terminate the string
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE),strTempVal, varInfo->Color_1);//write to lcd
	//DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}
void Format_MemoryDumpHex(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	memset(strTempVal, 0, 20);
	memcpy(strTempVal, (char*)varInfo->VariablePointer, 4);//copy ram to display string portrait
	if(strTempVal[0]<32) strTempVal[0]=45;//non printing characters
	if(strTempVal[1]<32) strTempVal[1]=45;
	if(strTempVal[2]<32) strTempVal[2]=45;
	if(strTempVal[3]<32) strTempVal[3]=45;

	strTempVal[4]=32;
	char* temp = strTempVal;
	temp+=5;
	uint8_t* soapHex = (uint8_t*)varInfo->VariablePointer;
	for(uint8_t i = 0; i < 4; i ++)
	{
		sprintf(temp, "%02X ", soapHex[i]);
		temp += 3;
	}
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_1);
}

//void Format_Float3(uint8_t row, void* info)
//{
//	//return;
//	varInfo = (LcdVariableInfo*)info;
//	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE),varInfo->Label, varInfo->Color_1);
//	sprintf(strTempVal, "%.3f ", (float)(*((float*)varInfo->VariablePointer)));
//	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
//}
void Format_Boolean(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, TOP_PADDING + (row * LCD_LINESIZE) ,varInfo->Label, varInfo->Color_1);
	sprintf(strTempVal, "%s ", (*(uint8_t*)varInfo->VariablePointer) == 0?"False":"True");
	DrawString(DISPLAY_WIDTH/2, TOP_PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
}
void Format_Bar(uint8_t row, void* info)
{
	varInfo = (LcdVariableInfo*)info;

	float percentON = (float)((*(uint16_t*)varInfo->VariablePointer)* 0.01) ;//lets see how much ON time we have
	/*   |=====60%==|==40%==| */
	/*   X1========X2====X3*/
	uint16_t X1 = LEFT_PADDING;
	uint16_t X2 = LEFT_PADDING + percentON * (DISPLAY_WIDTH - LEFT_PADDING - RIGHT_PADDING); //Screen Width -Left padding - Right Padding
	uint16_t X3 = DISPLAY_WIDTH - LEFT_PADDING;
	FillRect(X1, TOP_PADDING + (row * LCD_LINESIZE), X2, TOP_PADDING + ((row +1)* LCD_LINESIZE)-4, varInfo->Color_1);
	FillRect(X2, TOP_PADDING + (row * LCD_LINESIZE), X3, TOP_PADDING + ((row +1)* LCD_LINESIZE)-4, varInfo->Color_2); //BLACK);//

}

void UpdateScreen(LcdVariableInfo* InfoToDisplay)
{
	uint16_t row = 0;
	while (InfoToDisplay[row].VariablePointer)
	{
		switch (InfoToDisplay[row].FuncType)
		{
		case FUNC_INT: 			Format_Int(row, &InfoToDisplay[row]); break;
		case FUNC_INT16: 		Format_Int16(row, &InfoToDisplay[row]); break;
		case FUNC_FLOAT: 		Format_Float3_3(row, &InfoToDisplay[row]); break;
		case FUNC_HEX8: 		Format_Hex8(row, &InfoToDisplay[row]); break;
		case FUNC_HEX16: 		Format_Hex16(row, &InfoToDisplay[row]); break;
		case FUNC_HEX32: 		Format_Hex32(row, &InfoToDisplay[row]); break;
		case FUNC_ASCII: 		Format_Ascii(row, &InfoToDisplay[row]); break;
		case FUNC_BOOLEAN: 		Format_Boolean(row, &InfoToDisplay[row]); break;
		case FUNC_MEMDUMPASCII: Format_MemoryDumpAscii(row, &InfoToDisplay[row]); break;
		case FUNC_MEMDUMPHEX:   Format_MemoryDumpHex(row, &InfoToDisplay[row]); break;
		default: break;
		}
		row++;
	}
}
