#pragma once
#include "main.h"


typedef struct {
	uint32_t Channel;
	pinType Pin;
	float* ConvertionTable;
}ADC_ChannelDef;
#define ADC_CHANNEL_NUM 10
extern ADC_ChannelDef AdcChannelTable[ADC_NUM_CHANNELS];
