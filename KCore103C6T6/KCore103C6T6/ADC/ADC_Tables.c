#ifdef USE_ADC

#include "ADC_Tables.h"


AdcTableStruct const RtdTable_1K[] __attribute__((aligned(4))) =
{
	// 1K RTD -- based on datasheet
		// 20 entries; 4 bytes each; 80 bytes total
		// (uint16_t)adcVal, (uint16_t)(tempC * 32)    (MUST be in order with increasing adcVals)
		{ 0, (-242 << TEMP_FRAC_BITS) },
	// extrapolated to get -242 as 0 ohms (yielding 0 adc value with 2K divider)
		{ 347, (-200 << TEMP_FRAC_BITS) },
	// 1K mfg table starts at -200
		{ 678, (-150 << TEMP_FRAC_BITS) },
	{ 948, (-100 << TEMP_FRAC_BITS) },
	{ 1173, (-50 << TEMP_FRAC_BITS) },
	{ 1365, (0 << TEMP_FRAC_BITS) },
	{ 1531, (50 << TEMP_FRAC_BITS) },
	{ 1676, (100 << TEMP_FRAC_BITS) },
	{ 1803, (150 << TEMP_FRAC_BITS) },
	{ 1916, (200 << TEMP_FRAC_BITS) },
	{ 2017, (250 << TEMP_FRAC_BITS) },
	{ 2107, (300 << TEMP_FRAC_BITS) },
	{ 2189, (350 << TEMP_FRAC_BITS) },
	{ 2263, (400 << TEMP_FRAC_BITS) },
	{ 2330, (450 << TEMP_FRAC_BITS) },
	{ 2392, (500 << TEMP_FRAC_BITS) },
	{ 2448, (550 << TEMP_FRAC_BITS) },
	{ 2500, (600 << TEMP_FRAC_BITS) },
	// 1K mfg tables ends at 600
	{ 2809, (999 << TEMP_FRAC_BITS) },
	// extrapolated to get 999 as 4370 ohms (yielding 2809 adc value with 2K divider)
	{ 4096, MAX_TEMP },
	// in reality, to get to MAX_ADC, would take about 10 billion degrees.
};
#endif