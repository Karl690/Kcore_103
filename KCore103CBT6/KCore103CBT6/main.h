#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>
#include <stm32f1xx_hal_dma.h>
#include <stm32f1xx_hal_spi.h>

#include "taskmanager.h"

#include "GPIO/pins.h"
#include "GPIO/pinout.h"
#include "GPIO/gpio.h"
#include "ADC/adc.h"
#include "DISPLAY/display.h"
#include "DISPLAY/gui.h"

#define USE_SPI_DMA
extern uint8_t Booted;
extern uint8_t DisplayIndex;

void Error_Handler(void);