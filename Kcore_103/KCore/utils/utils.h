#pragma once
#include "main.h"
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delay_sec(uint32_t sec);

void initClkAndResetAPB2(uint32_t periph);