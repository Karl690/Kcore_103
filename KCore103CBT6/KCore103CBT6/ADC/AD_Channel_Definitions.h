#pragma once
#ifdef USE_ADC
#include "main.h"
typedef struct {
	uint32_t Channel;
	uint32_t Rank;
	uint32_t Pin;
	// float* ConvertionTable;
	void* convertionTable;
}ADC_ChannelDef;



#define ADC_CHANNEL_POSITION            ADC_CHANNEL_9
#define ADC_CHANNEL_RTD1                ADC_CHANNEL_1
#define ADC_CHANNEL_RTD2                ADC_CHANNEL_0
#define ADC_CHANNEL_RTD3                ADC_CHANNEL_4
#define ADC_CHANNEL_TEMP_SENSOR         ADC_CHANNEL_16  // internal temp sensor
#define ADC_CHANNEL_PA3                 ADC_CHANNEL_3

extern ADC_ChannelDef AdcChannelTable[];
#endif