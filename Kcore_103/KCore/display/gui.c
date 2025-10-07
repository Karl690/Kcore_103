#include "display.h"
#include "st7735.h"

void DisplayInit(uint8_t width, uint8_t height)
{
	ST7735_Init(width,height);
	ST7735_AddrSet(0,0,width,height);
	ST7735_Clear(0);
}
void SetDisplayAddress(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE)
{
	ST7735_AddrSet(XS, YS, XE, YE);
}
void SetDisplayOrientation(uint8_t orientation)
{
	ST7735_Orientation(orientation);
}
void ClearDisplay(uint16_t color)
{
	ST7735_Clear(color);
}
void DrawPixel(uint16_t X, uint16_t Y, uint16_t color)
{
	ST7735_Pixel(X, Y, color);
}
void DrawHLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color)
{
	ST7735_HLine(X1, X2, Y, color);
}
void DrawVLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color)
{
	ST7735_VLine(X, Y1, Y2, color);
}
void DrawLine(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color)
{
	ST7735_Line(X1, Y1, X2, Y2, color);
}
void DrawRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
{
	ST7735_Rect(X1, Y1, X2, Y2, color);
}
void FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
{
	ST7735_FillRect(X1, Y1, X2, Y2, color);
}

void DrawChar(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color)
{
	ST7735_PutChar5x7(X, Y, chr, color);
}
void DrawString(uint8_t X, uint8_t Y, char *str, uint16_t color)
{
#ifdef FONT16
	ST7735_PutStr16x8(X, Y, str, color);
#else
	ST7735_PutStr5x7(X, Y, str, color);
#endif
}
