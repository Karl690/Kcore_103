#pragma once
#include "displaylist.h"
//to enable a display, simply uncomment the appropriate display format below
//only 1 format can be supported at a time, so only uncomment out 1 format and
//make sure to comment out the unused format

#define DISPLAY_128X128 //Set the LCD with screen 128x128pixcel
// #define DISPLAY_128X160//Set the LCD with screen 128x160pixcel

#define FONT16
#ifdef FONT16
#define FONT_SIZE 16
#define LCD_LINESIZE	18			//The size for one line, in pixels
#else
#define FONT_SIZE 7
#define LCD_LINESIZE	11			//The size for one line, in pixels
#endif
//128 x 128 display
#ifdef DISPLAY_128X128              //LCD with screen 128x128 pixels
#define DISPLAY_WIDTH 130			//width in pixels
#define DISPLAY_HEIGHT 132			//height in pixels
#define TOP_PADDING 5				//Top side dead zone, in pixels
#define LEFT_PADDING 5				//Left side dead zone, in pixels
#define RIGHT_PADDING 5				//Right side dead zone, in pixels
#define LCD_LINESIZE	11			//The size for one line, in pixels
//128x160 display
#elif defined(DISPLAY_128X160) //LCD with screen 128x160 pixels
#define DISPLAY_WIDTH 128			//width in pixels
#define DISPLAY_HEIGHT 160			//height in pixels
#define TOP_PADDING 5				//Top side dead zone, in pixels
#define LEFT_PADDING 5				//Left side dead zone, in pixels
#define RIGHT_PADDING 5				//Right side dead zone, in pixels

//NO display  default if on other display is selected....
#define NO_DISPLAY
#define DISPLAY_WIDTH 130
#define DISPLAY_HEIGHT 130
#define TOP_PADDING 5
#define LEFT_PADDING 5
#define RIGHT_PADDING 5
#define LCD_LINESIZE		11
#endif

#define HALF_SIZE DISPLAY_WIDTH/2

typedef enum {
	COLOR_BLACK           = 0,
	COLOR_WHITE           = 1,
	COLOR_RED             = 2,
	COLOR_LIME            = 3,
	COLOR_BLUE            = 4,
	COLOR_YELLOW          = 5,
	COLOR_CYAN            = 6,
	COLOR_MAGENTA         = 7,
	COLOR_SILVER          = 8,
	COLOR_GRAY            = 9,
	COLOR_MAROON          = 10,
	COLOR_OLIVE           = 11,
	COLOR_GREEN           = 12,
	COLOR_PURPLE          = 13,
	COLOR_TEAL            = 14,
	COLOR_NAVY            = 15,
	COLOR_DARKRED         = 16,
	COLOR_TOMATO          = 17,
	COLOR_GOLD            = 18,
	COLOR_YELLOWGREEN     = 19,
	COLOR_FORESTGREEN     = 20,
	COLOR_SPRINGGREEN     = 21,
	COLOR_DARKCYAN        = 22,
	COLOR_PALETURQUOISE   = 23,
	COLOR_POWDERBLUE      = 24,
	COLOR_DEEPSKYBLUE     = 25,
	COLOR_LIGHTSKYBLUE    = 26,
	COLOR_DARKBLUE        = 27,
	COLOR_MEDINUSLATEBLUE = 28,
	COLOR_DARKVIOLET      = 29,
	COLOR_PLUM            = 30,
	COLOR_BEIGE           = 31,
	COLOR_INDEX_SIZE      = 32
}ColorIndex_t;

#define WHITE         	 (0xFFFF)
#define BLACK         	 (0x0000)
#define BLUE         	 (0x001F)
#define BRED             (0XF81F)
#define GRED 			 (0XFFE0)
#define GBLUE			 (0X07FF)
#define RED           	 (0xF800)
#define MAGENTA       	 (0xF81F)
#define GREEN         	 (0x07E0)
#define CYAN          	 (0x7FFF)
#define YELLOW        	 (0xFFE0)
#define BROWN 			 (0XBC40)
#define BRRED 			 (0XFC07)
#define GRAY  			 (0X8430)
#define DARKGRAY		 (0x3186)
#define DARKBLUE      	 (0X01CF)
#define LIGHTBLUE      	 (0X7D7C)
#define GRAYBLUE       	 (0X5458)


typedef void (*FORMATFUNC)(uint8_t, void* );


void Format_Title(uint8_t row, void* info);
void Format_Int32(uint8_t row, void* info);
void Format_Int16(uint8_t row, void* info);
void Format_Hex8(uint8_t row, void* info);
void Format_Hex16(uint8_t row, void* info);
void Format_Hex32(uint8_t row, void* info);
void Format_Ascii(uint8_t row, void* info);
void Format_Float3_3(uint8_t row, void* info);
void Format_Boolean(uint8_t row, void* info);
void Format_Bar(uint8_t row, void* info);
void Format_MemoryDumpAscii(uint8_t row, void* info);
void Format_MemoryDumpHex(uint8_t row, void* info);
void UpdateScreen(LcdVariableInfo*);

