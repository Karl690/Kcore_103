#pragma once
#include <stdint.h>
#include "main.h"

#define TEMP_FRAC_BITS                  5
#define TEMP_SCALE                      32
#define TEMP_SCALEF                     32.0f
#define BOGUS_TEMPERATURE               (temp_t)0x8000  // max negative
#define MIN_TEMP                        (temp_t)0x8001  // max negative
#define MAX_TEMP                        (temp_t)0x7fff  // max positive
#define MAX_AMBIENT_TEMPERATURE         (500 << TEMP_FRAC_BITS)


#define MAX_ADC12                       0x0fff
#define HH_POSITION_UNPLUGGED           0xFE	//254
#define MAX_POSSIBLE_TABLE_SIZE			33

typedef int16_t temp_t; // temperature (s10.5 format - 1/32 degree resolution)
typedef struct {
	uint16_t    adcRaw; // expected adcValue for specified temp
	temp_t      value; // if temperature (s10.5 format - 1/32 degree) at specified adcValue; position, just value, etc
} AdcTableStruct;

extern AdcTableStruct const DevicePositionTable[];
extern AdcTableStruct const RtdTable_50K[];
extern AdcTableStruct const RtdTable_1M[];
extern AdcTableStruct const RtdTable_1K[];
extern AdcTableStruct const RtdTable_10K[];
extern AdcTableStruct const RdcTable_4to20ma[];
extern AdcTableStruct const RdcTable_rawAdc[];
extern AdcTableStruct const RtdTable_100[];

float AdcConvertValue(uint8_t conversionType, uint16_t value2convert, AdcTableStruct * table);
