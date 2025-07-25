#pragma once
#include "main.h"
#include "ADC_Tables.h"

typedef enum {
	CONV_UNDEFINED = 0,
	CONV_NONE = 1,
	// return input
	CONV_NEAREST = 2,
	CONV_INTERPOLATE = 3,
	CONV_DC_VOLTS = 4
} adcConv_t;


typedef struct {
	uint32_t Channel;
	pinType Pin;
	adcConv_t converstionType;
	AdcTableStruct* ConvertionTable;
}ADC_ChannelDef;
#define ADC_CHANNEL_NUM 10
extern ADC_ChannelDef AdcChannelTable[ADC_NUM_CHANNELS];
