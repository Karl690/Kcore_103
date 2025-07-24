#pragma once
#include <stdint.h>
#include "1xx/adc_1xx.h"

#define ADC_NUM_CHANNELS                3
#define ADC_CHANNEL_POSITION            ADC_Channel_9
#define ADC_CHANNEL_RTD1                ADC_Channel_1
#define ADC_CHANNEL_RTD2                ADC_Channel_0
#define ADC_CHANNEL_RTD3                ADC_Channel_4
#define ADC_CHANNEL_TEMP_SENSOR         ADC_Channel_16  // internal temp sensor
#define ADC_CHANNEL_PA3                 ADC_Channel_3

#define ANALOG_VOLTAGE  (0x34cd)    // 3.3V in a 4.12 format
#define VDDA_FLOAT		3.3f

#define ADC_NUM_SAMPLES                 10  // 10 values saved; toss high and low to get average
#define ADC_SHIFT_FOR_AVG               3


typedef enum {
	CONV_UNDEFINED = 0,
	CONV_NONE = 1, // return input
	CONV_NEAREST = 2,
	CONV_INTERPOLATE = 3,
} adcConv_t;


typedef struct {
	uint16_t    adcRaw; // last raw value read from ADC
	int16_t     convRaw; // converted value from last rawValue read
	int16_t     adcAvg; // average of the last 10 read (or converted reads) after tossing high and low
	float     convAvg; // converted value using the rawAvg
	int16_t     sampleHistory[ADC_NUM_SAMPLES]; // last N reads from ADC
	uint8_t     sampleIndex; // index of last valid value in the history
	uint8_t     inputChannel; // actual adc channel input selection
	adcConv_t   conversionType;
} adcStruct;

extern uint16_t RawADCDataBuffer[];
extern adcStruct ADC_Channel[ADC_NUM_CHANNELS];
void adc_init(void);
void adc_start(void);
void ProcessRawADC_Data();
void SmoothDataUsingOlympicVotingAverage();