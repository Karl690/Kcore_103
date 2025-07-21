#ifdef USE_ADC
#include "AD_Channel_Definitions.h"
#define CovertionTableIndexSize 16
float ConvertionTable[CovertionTableIndexSize] = {1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1};
ADC_ChannelDef AdcChannelTable[ADC_NUM_CHANNELS] = {
	{ ADC_CHANNEL_POSITION, ADC_REGULAR_RANK_1, PIN_ADC_POSITION, ConvertionTable },
	{ ADC_CHANNEL_RTD1, ADC_REGULAR_RANK_2, PIN_ADC_RTD1_IN, ConvertionTable },
	{ ADC_CHANNEL_RTD2, ADC_REGULAR_RANK_3, PIN_ADC_RTD2_IN, ConvertionTable },
} ;
#endif