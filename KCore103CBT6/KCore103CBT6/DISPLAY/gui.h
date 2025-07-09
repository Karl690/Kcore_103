#include <stdint.h>
#include "st7735.h"
void DisplayInit(uint8_t width, uint8_t height);
void SetDisplayAddress(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE);
void SetDisplayOrientation(uint8_t orientation);
void DisplayClear(uint16_t color);

void DrawPixel(uint16_t X, uint16_t Y, uint16_t color);
void DrawHLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color);
void DrawVLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color);
void DrawLine(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color);
void DrawRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color);
void FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color);
void DrawChar(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color);
void DrawString(uint8_t X, uint8_t Y, char *str, uint16_t color);
