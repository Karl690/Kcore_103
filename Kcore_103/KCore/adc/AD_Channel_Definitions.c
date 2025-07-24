#include "AD_Channel_Definitions.h"
#include "ADC_Tables.h"
#define CovertionTableIndexSize 16

ADC_ChannelDef AdcChannelTable[ADC_NUM_CHANNELS] = {
	{ ADC_CHANNEL_POSITION, PIN_ADC_POSITION, CONV_NEAREST, DevicePositionTable},
	{ ADC_CHANNEL_RTD1, PIN_ADC_RTD1_IN, CONV_NEAREST, RtdTable_100 },
	{ ADC_CHANNEL_RTD2, PIN_ADC_RTD2_IN, CONV_NEAREST, RtdTable_1K},
} ;