#pragma once
#ifdef USE_ADC
#include "main.h"

void adc_1xx_init();
void adc_1xx_channel_config(uint8_t channel, uint8_t rank);
void adc_1xx_start(uint16_t* adcValues, uint32_t channel);
#endif