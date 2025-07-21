
#include "st7735.h"
#include "font5x7.h"
uint16_t scr_w;
uint16_t scr_h;


uint16_t scr_width;
uint16_t scr_height;

SPI_HandleTypeDef hspi;
DMA_HandleTypeDef hdma_spi_tx; // Add DMA handle
void ST7735_write(uint8_t data) {
	
#ifdef USE_SPI_DMA
  HAL_SPI_Transmit_DMA(&hspi, &data, 1);
	//while(hspi1.State == HAL_SPI_STATE_BUSY_TX);
#else
	HAL_SPI_Transmit(&hspi, &data, sizeof(data), HAL_MAX_DELAY);
#endif
}

void ST7735_cmd(uint8_t cmd) {
	DC_L();
	ST7735_write(cmd);
}

void ST7735_data(uint8_t data) {
	DC_H();
	ST7735_write(data);
}

uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B) {
	return (((R >> 3) & 0x1f) << 11) | (((G >> 2 ) & 0x3f) << 5) | ((B >> 3) & 0x1f);
}

void ST7735_Init(uint8_t wid, uint8_t height) {
	
#ifdef USE_SPI_DMA
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
#endif	
	hspi.Instance = SPI_PORT;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi.Init.NSS = SPI_NSS_SOFT;
	hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi) != HAL_OK)
	{
		Error_Handler();
	}
#if (_SPI_PORT == 1)	
	__HAL_RCC_SPI1_CLK_ENABLE();
#else
	__HAL_RCC_SPI2_CLK_ENABLE();
#endif
  
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = SPI_SCK_PIN | SPI_MOSI_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#ifdef USE_SPI_DMA	
	/* SPI1 DMA Init */
    /* SPI1_TX Init */
	hdma_spi_tx.Instance = DMA1_Channel3;
	hdma_spi_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_spi_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi_tx.Init.Mode = DMA_NORMAL;
	hdma_spi_tx.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_spi_tx) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(&hspi, hdmatx, hdma_spi_tx);
	
#endif
	
	// Reset display
	CS_H();
	RST_H();
	HAL_Delay(5);
	RST_L();
	HAL_Delay(5);
	RST_H();
	CS_H();
	HAL_Delay(5);
	CS_L();

	ST7735_cmd(0x11); // Sleep out & booster on
	//Delay_ms(20); // Datasheet says what display wakes about 120ms (may be much faster actually)

	ST7735_cmd(0xb1);   // In normal mode (full colors):
	DC_H();
	ST7735_write(0x05); //   RTNA set 1-line period: RTNA2, RTNA0
	ST7735_write(0x3c); //   Front porch: FPA5,FPA4,FPA3,FPA2
	ST7735_write(0x3c); //   Back porch: BPA5,BPA4,BPA3,BPA2

	ST7735_cmd(0xb2);   // In idle mode (8-colors):
	DC_H();
	ST7735_write(0x05); //   RTNB set 1-line period: RTNAB, RTNB0
	ST7735_write(0x3c); //   Front porch: FPB5,FPB4,FPB3,FPB2
	ST7735_write(0x3c); //   Back porch: BPB5,BPB4,BPB3,BPB2

	ST7735_cmd(0xb3);   // In partial mode + full colors:
	DC_H();
	ST7735_write(0x05); //   RTNC set 1-line period: RTNC2, RTNC0
	ST7735_write(0x3c); //   Front porch: FPC5,FPC4,FPC3,FPC2
	ST7735_write(0x3c); //   Back porch: BPC5,BPC4,BPC3,BPC2
	ST7735_write(0x05); //   RTND set 1-line period: RTND2, RTND0
	ST7735_write(0x3c); //   Front porch: FPD5,FPD4,FPD3,FPD2
	ST7735_write(0x3c); //   Back porch: BPD5,BPD4,BPD3,BPD2

	ST7735_cmd(0xB4);   // Display dot inversion control:
	ST7735_data(0x03);  //   NLB,NLC

	ST7735_cmd(0x3a);   // Interface pixel format
//	ST7735_data(0x03);  // 12-bit/pixel RGB 4-4-4 (4k colors)
	ST7735_data(0x05);  // 16-bit/pixel RGB 5-6-5 (65k colors)
//	ST7735_data(0x06);  // 18-bit/pixel RGB 6-6-6 (256k colors)

//	ST7735_cmd(0x36);   // Memory data access control:
						//   MY MX MV ML RGB MH - -
//	ST7735_data(0x00);  //   Normal: Top to Bottom; Left to Right; RGB
//	ST7735_data(0x80);  //   Y-Mirror: Bottom to top; Left to Right; RGB
//	ST7735_data(0x40);  //   X-Mirror: Top to Bottom; Right to Left; RGB
//	ST7735_data(0xc0);  //   X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
//	ST7735_data(0x20);  //   X-Y Exchange: X and Y changed positions
//	ST7735_data(0xA0);  //   X-Y Exchange,Y-Mirror
//	ST7735_data(0x60);  //   X-Y Exchange,X-Mirror
//	ST7735_data(0xE0);  //   X-Y Exchange,X-Mirror,Y-Mirror

	ST7735_cmd(0x20);   // Display inversion off
//	ST7735_cmd(0x21);   // Display inversion on

	ST7735_cmd(0x13);   // Partial mode off

	ST7735_cmd(0x26);   // 	 Gamma curve set:
	ST7735_data(0x01);  //   Gamma curve 1 (G2.2) or (G1.0)
//	ST7735_data(0x02);  //   Gamma curve 2 (G1.8) or (G2.5)
//	ST7735_data(0x04);  //   Gamma curve 3 (G2.5) or (G2.2)
//	ST7735_data(0x08);  //   Gamma curve 4 (G1.0) or (G1.8)

	ST7735_cmd(0x29);   // Display on

	CS_H();

	//ST7735_Orientation(scr_CW);
	ST7735_Orientation(scr_180);
}

void ST7735_Orientation(ScrOrientation_TypeDef orientation) {
	CS_L();
	ST7735_cmd(0x36); // Memory data access control:
	switch(orientation) {
	case scr_CW:
		scr_width  = scr_h;
		scr_height = scr_w;
		ST7735_data(0xA0); // X-Y Exchange,Y-Mirror
		break;
	case scr_CCW:
		scr_width  = scr_h;
		scr_height = scr_w;
		ST7735_data(0x60); // X-Y Exchange,X-Mirror
		break;
	case scr_180:
		scr_width  = scr_w;
		scr_height = scr_h;
		ST7735_data(0xc0); // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		break;
	default:
		scr_width  = scr_w;
		scr_height = scr_h;
		ST7735_data(0x00); // Normal: Top to Bottom; Left to Right; RGB
		break;
	}
	CS_H();
}

void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE) {
	ST7735_cmd(0x2a); // Column address set
	DC_H();
	ST7735_write(XS >> 8);
	ST7735_write(XS);
	ST7735_write(XE >> 8);
	ST7735_write(XE);

	ST7735_cmd(0x2b); // Row address set
	DC_H();
	ST7735_write(YS >> 8);
	ST7735_write(YS);
	ST7735_write(YE >> 8);
	ST7735_write(YE);

	ST7735_cmd(0x2c); // Memory write
}

void ST7735_Clear(uint16_t color) {
	uint16_t i;
	uint8_t  CH,CL;

	CH = color >> 8;
	CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(0,0,scr_width - 1,scr_height - 1);
	DC_H();
	for (i = 0; i < scr_width * scr_height; i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_Pixel(uint16_t X, uint16_t Y, uint16_t color) {
    CS_L();
    ST7735_AddrSet(X,Y,X,Y);
    DC_H();
    ST7735_write(color >> 8);
    ST7735_write((uint8_t)color);
    CS_H();
}

void ST7735_HLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color) {
    uint16_t i;
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    CS_L();
    ST7735_AddrSet(X1,Y,X2,Y);
	DC_H();
	for (i = 0; i <= (X2 - X1); i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_VLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color) {
    uint16_t i;
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    CS_L();
    ST7735_AddrSet(X,Y1,X,Y2);
	DC_H();
	for (i = 0; i <= (Y2 - Y1); i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_Line(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color) {
	int16_t dX = X2-X1;
	int16_t dY = Y2-Y1;
	int16_t dXsym = (dX > 0) ? 1 : -1;
	int16_t dYsym = (dY > 0) ? 1 : -1;

	if (dX == 0) {
		if (Y2>Y1) ST7735_VLine(X1,Y1,Y2,color); else ST7735_VLine(X1,Y2,Y1,color);
		return;
	}
	if (dY == 0) {
		if (X2>X1) ST7735_HLine(X1,X2,Y1,color); else ST7735_HLine(X2,X1,Y1,color);
		return;
	}

	dX *= dXsym;
	dY *= dYsym;
	int16_t dX2 = dX << 1;
	int16_t dY2 = dY << 1;
	int16_t di;

	if (dX >= dY) {
		di = dY2 - dX;
		while (X1 != X2) {
			ST7735_Pixel(X1,Y1,color);
			X1 += dXsym;
			if (di < 0) {
				di += dY2;
			} else {
				di += dY2 - dX2;
				Y1 += dYsym;
			}
		}
	} else {
		di = dX2 - dY;
		while (Y1 != Y2) {
			ST7735_Pixel(X1,Y1,color);
			Y1 += dYsym;
			if (di < 0) {
				di += dX2;
			} else {
				di += dX2 - dY2;
				X1 += dXsym;
			}
		}
	}
	ST7735_Pixel(X1,Y1,color);
}

void ST7735_Rect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color) {
	ST7735_HLine(X1,X2,Y1,color);
	ST7735_HLine(X1,X2,Y2,color);
	ST7735_VLine(X1,Y1,Y2,color);
	ST7735_VLine(X2,Y1,Y2,color);
}

void ST7735_FillRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color) {
	uint16_t i;
	uint16_t FS = (X2 - X1 + 1) * (Y2 - Y1 + 1);
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

	CS_L();
	ST7735_AddrSet(X1,Y1,X2,Y2);
	DC_H();
	for (i = 0; i < FS; i++) {
		ST7735_write(CH);
		ST7735_write(CL);
	}
	CS_H();
}

void ST7735_PutChar5x7(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color) {
	uint16_t i,j;
	uint8_t buffer[5];
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

	memcpy(buffer,&Font5x7[(chr - 32) * 5],5);

	CS_L();
	ST7735_AddrSet(X,Y,X + 4,Y + 6);
	DC_H();
	for (j = 0; j < 7; j++) {
        for (i = 0; i < 5; i++) {
    		if ((buffer[i] >> j) & 0x01) {
    			ST7735_write(CH);
    			ST7735_write(CL);
    		} else {
    			ST7735_write(0x00);
    			ST7735_write(0x00);
    		}
    	}
    }
	CS_H();
}

void ST7735_PutStr5x7(uint8_t X, uint8_t Y, char *str, uint16_t color) {
    while (*str) {
        ST7735_PutChar5x7(X,Y,*str++,color);
        if (X < scr_width - 6) { X += 6; } else if (Y < scr_height - 8) { X = 0; Y += 8; } else { X = 0; Y = 0; }
    };
}
