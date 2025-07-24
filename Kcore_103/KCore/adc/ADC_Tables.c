#include "ADC_Tables.h"
#include "AD_Channel_Definitions.h"

AdcTableStruct const DevicePositionTable[] __attribute__((aligned(4))) =
{ // from the Hot Head Resistor Value and 12-bit ADC Value spec
		// 27 entries, 4 bytes each, 108 total bytes
		// (uint16_t)adcVal, (uint16)devPos   (MUST be in order with increasing adcVals)
		{ 108, 91 }, // max adc value for hotbed1 position
		{ 294, 26 }, // max adc value for yoke 2 aux2
		{ 452, 16 }, // max adc value for yoke 1 aux2
		{ 635, 21 }, // max adc value for yoke 2 hot head 1
		{ 804, 11 }, // max adc value for yoke 1 hot head 1
		{ 943, 22 }, // max adc value for yoke 2 hot head 2
		{ 1082, 12 }, // max adc value for yoke 1 hot head 2
		{ 1228, 23 }, // max adc value for yoke 2 hot head 3
		{ 1389, 13 }, // max adc value for yoke 1 hot head 3
		{ 1563, 24 }, // max adc value for yoke 2 hot head 4
		{ 1751, 14 }, // max adc value for yoke 1 hot head 4
		{ 1946, 25 }, // max adc value for yoke 2 aux1
		{ 2140, 15 }, // max adc value for yoke 1 aux1
		{ 2345, 92 }, // max adc value for hotbed2 position
		{ 2544, 46 }, // max adc value for yoke 4 aux2
		{ 2723, 36 }, // max adc value for yoke 3 aux2
		{ 2901, 41 }, // max adc value for yoke 4 hot head 1
		{ 3065, 31 }, // max adc value for yoke 3 hot head 1
		{ 3200, 42 }, // max adc value for yoke 4 hot head 2
		{ 3317, 32 }, // max adc value for yoke 3 hot head 2
		{ 3439, 43 }, // max adc value for yoke 4 hot head 3
		{ 3557, 33 }, // max adc value for yoke 3 hot head 3
		{ 3667, 44 }, // max adc value for yoke 4 hot head 4
		{ 3780, 34 },
	// max adc value for yoke 3 hot head 4
		{ 3906, 45 }, // max adc value for yoke 4 aux1
		{ 4034, 35 }, // max adc value for yoke 3 aux1
		{ MAX_ADC12, HH_POSITION_UNPLUGGED },
	// max adc value an unplugged hothead
};

////////////////////////////////////////////////////////////////////////////////

AdcTableStruct const RtdTable_50K[] __attribute__((aligned(4))) =
{ // 50K RTD temperature conversion table
		// 17 entries, 4 bytes each, 68 total bytes
		// (uint16_t)adcVal, (uint16_t)(tempC * 32)    (MUST be in order with increasing adcVals)
		{ 0, (427 << TEMP_FRAC_BITS) }, // fake datapoint to have a 0 adc value
		{ 42, (271 << TEMP_FRAC_BITS) },
	{ 597, (131 << TEMP_FRAC_BITS) },
	{ 667, (126 << TEMP_FRAC_BITS) },
	{ 833, (116 << TEMP_FRAC_BITS) },
	{ 1039, (106 << TEMP_FRAC_BITS) },
	{ 1288, (96 << TEMP_FRAC_BITS) },
	{ 1580, (86 << TEMP_FRAC_BITS) },
	{ 1913, (76 << TEMP_FRAC_BITS) },
	{ 2270, (66 << TEMP_FRAC_BITS) },
	{ 2633, (56 << TEMP_FRAC_BITS) },
	{ 2977, (46 << TEMP_FRAC_BITS) },
	{ 3281, (36 << TEMP_FRAC_BITS) },
	{ 3530, (26 << TEMP_FRAC_BITS) },
	{ 3721, (16 << TEMP_FRAC_BITS) },
	{ 3848, (7 << TEMP_FRAC_BITS) },
	{ MAX_ADC12, (0 << TEMP_FRAC_BITS) }, // fake data point of 0C
};

////////////////////////////////////////////////////////////////////////////////

AdcTableStruct const RtdTable_1M[] __attribute__((aligned(4))) =
{ // 1M RTD temperature conversion table
		// 18 entries, 4 bytes each, 72 total bytes
		// (uint16_t)adcVal, (uint16_t)(tempC * 32)    (MUST be in order with increasing adcVals)
		{ 0, (427 << TEMP_FRAC_BITS) }, // fake datapoint to have a 0 adc value //   13653 },
		{ 400, (260 << TEMP_FRAC_BITS) }, //   8320 },
		{ 500, (250 << TEMP_FRAC_BITS) }, //   8000 },
		{ 540, (247 << TEMP_FRAC_BITS) }, //   7896 },
		{ 630, (240 << TEMP_FRAC_BITS) }, //   7656 },
		{ 745, (230 << TEMP_FRAC_BITS) },
	//   7360 },
		{ 918, (218 << TEMP_FRAC_BITS) }, //   6960 },
		{ 1003, (213 << TEMP_FRAC_BITS) }, //   6824 },
		{ 1242, (201 << TEMP_FRAC_BITS) }, //   6424 },
		{ 1613, (185 << TEMP_FRAC_BITS) }, //   5904 },
		{ 2502, (152 << TEMP_FRAC_BITS) }, //   4864 },
		{ 3108, (129 << TEMP_FRAC_BITS) }, //   4136 },
		{ 3550, (106 << TEMP_FRAC_BITS) }, //   3376 },
		{ 3900, (71 << TEMP_FRAC_BITS) }, //   2272 },
		{ 3974, (54 << TEMP_FRAC_BITS) },
	//   1736 },
		{ 4016, (33 << TEMP_FRAC_BITS) }, //   1040 },
		{ 4080, (3 << TEMP_FRAC_BITS) }, //    100 },
		{ MAX_ADC12, (0 << TEMP_FRAC_BITS) }, // fake data point of 0C
};

////////////////////////////////////////////////////////////////////////////////

AdcTableStruct const RtdTable_1K[] __attribute__((aligned(4))) =
{ // 1K RTD -- based on datasheet
		// 20 entries; 4 bytes each; 80 bytes total
		// (uint16_t)adcVal, (uint16_t)(tempC * 32)    (MUST be in order with increasing adcVals)
		{ 0, (-242 << TEMP_FRAC_BITS) }, // extrapolated to get -242 as 0 ohms (yielding 0 adc value with 2K divider)
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
	{ 2500, (600 << TEMP_FRAC_BITS) }, // 1K mfg tables ends at 600
	{ 2809, (999 << TEMP_FRAC_BITS) }, // extrapolated to get 999 as 4370 ohms (yielding 2809 adc value with 2K divider)
	{ MAX_ADC12, MAX_TEMP },
	// in reality, to get to MAX_ADC, would take about 10 billion degrees.
};

////////////////////////////////////////////////////////////////////////////////

AdcTableStruct const RtdTable_10K[] __attribute__((aligned(4))) =
{ // 10K Type 3 RTD -- based on datasheet
		// 22 entries; 4 bytes each; 88 bytes total
		// (uint16_t)adcVal, (uint16_t)(tempC * 32)    (MUST be in order with increasing adcVals)
		{ 0, (200 << TEMP_FRAC_BITS) }, // fake datapoint to have a 0 adc value
		{ 654, (130 << TEMP_FRAC_BITS) }, // first real data point
		{ 801, (120 << TEMP_FRAC_BITS) },
	{ 979, (110 << TEMP_FRAC_BITS) },
	{ 1192, (100 << TEMP_FRAC_BITS) },
	{ 1443, (90 << TEMP_FRAC_BITS) },
	{ 1729, (80 << TEMP_FRAC_BITS) },
	{ 2044, (70 << TEMP_FRAC_BITS) },
	{ 2375, (60 << TEMP_FRAC_BITS) },
	{ 2705, (50 << TEMP_FRAC_BITS) },
	{ 3016, (40 << TEMP_FRAC_BITS) },
	{ 3292, (30 << TEMP_FRAC_BITS) },
	{ 3413, (25 << TEMP_FRAC_BITS) },
	{ 3521, (20 << TEMP_FRAC_BITS) },
	{ 3701, (10 << TEMP_FRAC_BITS) },
	{ 3836, (0 << TEMP_FRAC_BITS) },
	{ 3931, (-10 << TEMP_FRAC_BITS) },
	{ 3995, (-20 << TEMP_FRAC_BITS) },
	{ 4036, (-30 << TEMP_FRAC_BITS) },
	{ 4062, (-40 << TEMP_FRAC_BITS) },
	{ 4077, (-50 << TEMP_FRAC_BITS) },
	{ MAX_ADC12, MIN_TEMP },
};

////////////////////////////////////////////////////////////////////////////////

AdcTableStruct const RdcTable_4to20ma[] __attribute__((aligned(4))) =
{
	// 4 to 20ma sensor, coverted to voltage and read
		// XX entries; 4 bytes each; XX bytes total
		// (uint16_t)adcVal, (uint16_t)(voltage(mV))    (MUST be in order with increasing adcVals)
		{ 0, 0 },
	{ 500, 130 },
	{ 1000, 260 },
	{ MAX_ADC12, 1111 },
};
	
////////////////////////////////////////////////////////////////////////////////

AdcTableStruct const RdcTable_rawAdc[] __attribute__((aligned(4))) =
{ // dummy table to allow rest of the RTD related code to work in the "raw" case
		// 2 entries; 4 bytes each; 8 bytes total
		// (uint16_t)adcVal, (uint16_t)(raw)    (MUST be in order with increasing adcVals)
		{ 0, 0 },
	{ MAX_ADC12, MAX_ADC12 },
};

////////////////////////////////////////////////////////////////////////////////

AdcTableStruct const RtdTable_100[] __attribute__((aligned(4))) =
{ // 100 RTD -- based on datasheet
		// 14 entries; 4 bytes each; 56 bytes total
		// (uint16_t)adcVal, (uint16_t)(tempC * 32)    (MUST be in order with increasing adcVals)
		{ 0, (-260 << TEMP_FRAC_BITS) }, // extrapolated to get -242 as 0 ohms (yielding 0 adc value with 2K divider)
		{ 159, (-50 << TEMP_FRAC_BITS) },
	// 1K mfg table starts at -200
		{ 195, (0 << TEMP_FRAC_BITS) },
	{ 209, (20 << TEMP_FRAC_BITS) },
	{ 223, (40 << TEMP_FRAC_BITS) },
	{ 237, (60 << TEMP_FRAC_BITS) },
	{ 251, (80 << TEMP_FRAC_BITS) },
	{ 265, (100 << TEMP_FRAC_BITS) },
	{ 299, (150 << TEMP_FRAC_BITS) },
	{ 333, (200 << TEMP_FRAC_BITS) },
	{ 366, (250 << TEMP_FRAC_BITS) },
	{ 522, (500 << TEMP_FRAC_BITS) },
	{ 799, (999 << TEMP_FRAC_BITS) },
	// extrapolated to get 999 as 4370 ohms (yielding 2809 adc value with 2K divider)
	{ MAX_ADC12, MAX_TEMP },
	// in reality, to get to MAX_ADC, would take about 10 billion degrees.
};


int32_t AdcConvertFromTable(adcConv_t conversionType, uint16_t value2convert, AdcTableStruct table[])
{
	// routine to convert the input value2convert into either a temperature or position utilizing the built in table.
	// the table is a linear array of increasing uint16_t representing the maximum adc value
	// along with a corresponding position or temperature.  the temperature is calculated by linearly
	// interpolating between the two table entries that bound in the input value2convert. the position is not interpolated.
	//
	// tables are checked for increasing and monotonic and end with a max_adc12 value at boot time and are in non
	// user loadable flash, so no need to recheck.

	int32_t retVal = -1;

	if (value2convert > MAX_ADC12)
	{
		// report error and shutdown (illegal adc value -- no way to convert)
		// reportError1x32(SHUTDOWN_FATAL_ERROR, ERROR_UNIT_ADC, ERROR_ADC_EXCEEDED_RANGE, (uint32_t)table);
	}
	else
	{
		int i = 0;
		while (i < MAX_POSSIBLE_TABLE_SIZE) // no table is longer than this -- just a safety stop on the while loop
		{
			if (value2convert <= table[i].adcRaw)
			{
				break;
			}
			i++;
		}

		if (conversionType == CONV_NEAREST)
		{
			retVal = (int32_t)table[i].value;
		}
		else if (conversionType == CONV_INTERPOLATE)
		{
			int32_t value;
			int32_t deltaAdc;
			int32_t rangeAdc;
			int32_t rangeValue;

			if (i == 0) // if first entry, then no chance to interpolated between two entries.
			{
				value = table[0].value;
			}
			else
			{
				deltaAdc = (int32_t)((value2convert - table[i - 1].adcRaw) << 16);
				rangeAdc = (int32_t)(table[i].adcRaw - table[i - 1].adcRaw);
				rangeValue = (int32_t)(table[i].value - table[i - 1].value);
				value   = ((((deltaAdc / rangeAdc) * rangeValue) + 0x8000) >> 16) + table[i - 1].value;
			}
			retVal = value;
		}
	}
	return (retVal);
}

int16_t AdcConvertValue(uint8_t conversionType, uint16_t value2convert, AdcTableStruct * table)
{
	// convert the raw adc input value to an alternate data type based on the conversionType
	int16_t convValue;
	switch (conversionType)
	{
	case CONV_NEAREST :
		convValue = (int16_t)AdcConvertFromTable(CONV_NEAREST, value2convert, table);
		break;
	case CONV_INTERPOLATE:
		convValue = (int16_t)AdcConvertFromTable(CONV_INTERPOLATE, value2convert, table);
		break;
		//        case CONV_CURRENT:
		//            convValue = value2convert; //adcp->adcAvg;   // send raw avg back; host will convert to current
		//            break;
		//        case CONV_VOLTAGE:
		//            convValue = value2convert; //adcp->adcAvg; // send raw avg back; host will convert to voltage
		//            break;
	case CONV_NONE:
		convValue = value2convert;
		break;
	default :
		convValue = BOGUS_TEMPERATURE;
		break;
	}
	return (convValue);
}