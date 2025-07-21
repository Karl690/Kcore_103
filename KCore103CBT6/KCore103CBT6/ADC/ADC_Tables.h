#pragma once
#ifdef USE_ADC

#include <stdint.h>
#include "main.h"

#define MAX_ADC12 						4096
#define HH_POSITION_UNPLUGGED           0xFE	//254


#define TEMP_FRAC_BITS                  5
#define TEMP_SCALE                      32
#define TEMP_SCALEF                     32.0f
#define BOGUS_TEMPERATURE               (temp_t)0x8000  // max negative
#define MIN_TEMP                        (temp_t)0x8001  // max negative
#define MAX_TEMP                        (temp_t)0x7fff  // max positive
#define MAX_AMBIENT_TEMPERATURE         (500 << TEMP_FRAC_BITS)

typedef int16_t temp_t; // temperature (s10.5 format - 1/32 degree resolution)
typedef struct {
	uint16_t    adcRaw; // expected adcValue for specified temp
	temp_t      value; // if temperature (s10.5 format - 1/32 degree) at specified adcValue; position, just value, etc
} AdcTableStruct;

extern const AdcTableStruct HeadPositionTable[];
extern const AdcTableStruct RtdTable_50K[];
extern const AdcTableStruct RtdTable_1K[];
extern const AdcTableStruct RtdTable_100[];

int16_t convertRtdDataFromRawADCValue(const AdcTableStruct* adcTable, uint16_t raw);
#endif