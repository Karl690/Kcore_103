#include "main.h"
#include "adc.h"
#include "ADC_Tables.h"

uint16_t   RawADCDataBuffer[ADC_CHANNEL_NUM] = { 0 };
float laserTemperature = 0;
float UvataVoltage = 0;
float CurrentSetPoint = 0;
float adcConversionFactor = 0.0008f;
float UvataVoltageConversionFactor = 0.00437f;
uint16_t UvataDuty = 0;
uint16_t CanHeadAddress = 0;


AdcTableStruct const HeadPositionTable2[] __attribute__((aligned(4))) =
{
	// from the Hot Head Resistor Value and 12-bit ADC Value spec
		// 27 entries, 4 bytes each, 108 total bytes
		// (uint16_t)adcVal, (uint16)devPos   (MUST be in order with increasing adcVals)
		{ 108, 91 },
	// max adc value for hotbed1 position
		{ 294, 26 },
	// max adc value for yoke 2 aux2
		{ 452, 16 },
	// max adc value for yoke 1 aux2
		{ 635, 21 },
	// max adc value for yoke 2 hot head 1
		{ 804, 11 },
	// max adc value for yoke 1 hot head 1
		{ 943, 22 },
	// max adc value for yoke 2 hot head 2
		{ 1082, 12 },
	// max adc value for yoke 1 hot head 2
		{ 1228, 23 },
	// max adc value for yoke 2 hot head 3
		{ 1389, 13 },
	// max adc value for yoke 1 hot head 3
		{ 1563, 24 },
	// max adc value for yoke 2 hot head 4
		{ 1751, 14 },
	// max adc value for yoke 1 hot head 4
		{ 1946, 25 },
	// max adc value for yoke 2 aux1
		{ 2140, 15 },
	// max adc value for yoke 1 aux1
		{ 2345, 92 },
	// max adc value for hotbed2 position
		{ 2544, 46 },
	// max adc value for yoke 4 aux2
		{ 2723, 36 },
	// max adc value for yoke 3 aux2
		{ 2901, 41 },
	// max adc value for yoke 4 hot head 1
		{ 3065, 31 },
	// max adc value for yoke 3 hot head 1
		{ 3200, 42 },
	// max  value for yoke 4 hot head 2
		{ 3317, 32 },
	// max adc value for yoke 3 hot head 2
		{ 3439, 43 },
	// max adc value for yoke 4 hot head 3
		{ 3557, 33 },
	// max adc value for yoke 3 hot head 3
		{ 3667, 44 },
	// max adc value for yoke 4 hot head 4
		{ 3780, 34 },
	// max adc value for yoke 3 hot head 4
		{ 3906, 45 },
	// max adc value for yoke 4 aux1
		{ 4034, 35 },
	// max adc value for yoke 3 aux1
		{ MAX_ADC12, HH_POSITION_UNPLUGGED },
	// max adc value an unplugged hothead
};

void adcInit(ADC_TypeDef *ADCx)
{
#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)
	RCC->CFGR &= CFGR_ADCPRE_Reset_Mask; // 0xffff3fff  //clock for ADC (max 14MHz --> 72/6=12MHz)
	RCC->CFGR |= RCC_PCLK2_Div6; // 0x0008000

	if (ADCx == ADC1)
		initClkAndResetAPB2(RCC_APB2Periph_ADC1);
	else if (ADCx == ADC2)
		initClkAndResetAPB2(RCC_APB2Periph_ADC2);
	else if (ADCx == ADC3)
		initClkAndResetAPB2(RCC_APB2Periph_ADC3);

// define ADC config
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // we work in single sampling mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_Init(ADCx, &ADC_InitStructure); //set config of ADCx

	ADC_Cmd(ADCx, ENABLE); //enable ADCx

	//  ADC calibration (optional, but recommended at power on)
	ADC_ResetCalibration(ADCx); // Reset previous calibration
	while (ADC_GetResetCalibrationStatus(ADCx)) ;
	ADC_StartCalibration(ADCx); // Start new calibration (ADC must be off at that time)
	while (ADC_GetCalibrationStatus(ADCx)) ;                  // Wait until calibration is complete
}
	
void adc_init(void)
{
	adcInit(ADC1);
	uint8_t i = 0;
	for (uint8_t i = 0; i < ADC_CHANNEL_NUM; i++)
	{	
		adc_config(AdcChannelTable[i].Channel);
		//initialize pin for channel
		if (AdcChannelTable[i].Pin)//if pin def is 0, skip pin init
		{
			pinInit(AdcChannelTable[i].Pin); 
		}
	}
	
	// it needs to set first channel in pulling mode, first
	adc_config(AdcChannelTable[0].Channel);
}

void adc_start()
{
	// it need to call this function if it is in DMA mode
}

float ScaledADCData[ADC_CHANNEL_NUM] = { 0 }; //converted adc buffer values
uint16_t ADC_Work_Channel_Index = 0; //used to walk thru the channels and update the working variables.
adcStruct ADC_Channel[ADC_CHANNEL_NUM];
adcStruct *ADC_Work_Channel;


uint32_t sum = 0; 
uint16_t raw = 0; 
uint16_t low = 0; 
uint16_t high = 0; 
uint16_t i = 0; //persistant variables
float adc_conversionFactor = 3.3 / 4096;


void ProcessRawADC_Data(void)
{
	if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != RESET)
	{
		// Read ADC raw value
		RawADCDataBuffer[ADC_Work_Channel_Index] = ADC_GetConversionValue(ADC1); 
		SmoothDataUsingOlympicVotingAverage();	
	}
	
}

void SmoothDataUsingOlympicVotingAverage(void)
{
	float stuntfloat = 0;
	ADC_Work_Channel = &ADC_Channel[ADC_Work_Channel_Index];
	ADC_Work_Channel->adcRaw = RawADCDataBuffer[ADC_Work_Channel_Index]; //update last reading
	//now we need to plug into the 10 reading buffer for smoothing.
	ADC_Work_Channel->sampleIndex++; //point to next place to enter the data in smoothing array
	if (ADC_Work_Channel->sampleIndex > 9)ADC_Work_Channel->sampleIndex = 0; //limit to 10 entries
	ADC_Work_Channel->sampleHistory[ADC_Work_Channel->sampleIndex] = ADC_Work_Channel->adcRaw; //plug in the latest reading.
	//at this point, channel.sampleHistory has the raw data to be smoothed.

	{
		// history buffer is full, so enough good adc values to proceed

		// this code does olympic voting (toss high and low and then average the rest)
		// the ADC_NUM_SAMPLES must be equal to (2^n) + 2 and ADC_SHIFT_FOR_AVG
		// must equal "n", as the code will shift to get the average instead of divide. set temporary
		// variables to record highest and lowest values as each of the ADC_NUM_SAMPLES is inspected
		// at the same time, record the sum of all ADC_NUM_SAMPLES samples.  when done looking at all values,
		// subtract the high and low from the sum and then calculate the average of the remaining sum.
		int32_t sum, raw, low, high, i;
		low = 0x7fffffff; // MAXINT
		high = 0x80000000; // MININT
		sum = 0;
		for (i = 0; i < ADC_NUM_SAMPLES; i++)
		{
			raw = ADC_Work_Channel->sampleHistory[i];
			sum += raw; // keep running total
			if (raw < low) low = raw; // update the lowest reading
			if (raw > high)high = raw; // update the highest reading
		}
		sum -= (low + high); // sum is now the total of the middle N values

		//next we will shift by n to effect a divide by 2^n to get the average of the 2^n remaining samples
		ADC_Work_Channel->adcAvg = (sum >> ADC_SHIFT_FOR_AVG); // update the RAW average
		//ADC_Work_Channel->convAvg = ScaledADCData[ADC_Work_Channel_Index] = (float)(((float)ADC_Work_Channel->adcAvg * 3.3) / 4095);
		stuntfloat = ((float)ADC_Work_Channel->adcAvg) * adcConversionFactor;
		ADC_Work_Channel->convVolt = stuntfloat;
		ADC_Work_Channel->convAvg = convertRtdDataFromRawADCValue(AdcChannelTable[ADC_Work_Channel_Index].ConvertionTable, ADC_Work_Channel->adcAvg);
	}
	// setup next conversion so data will be ready for the next call in ~10ms
	if (ADC_Work_Channel_Index == 0) CanHeadAddress = convertRawAdcToCanHeadAddress(ADC_Work_Channel->adcRaw); 
	if (ADC_Work_Channel_Index == 1) laserTemperature = ADC_Work_Channel->convAvg;
	if (ADC_Work_Channel_Index == 2) UvataVoltage = ADC_Work_Channel->adcAvg * UvataVoltageConversionFactor;
	if (ADC_Work_Channel_Index == 3) CurrentSetPoint = ADC_Work_Channel->convVolt * 2;;
	
	ADC_Work_Channel_Index++;
	if (ADC_Work_Channel_Index >= ADC_CHANNEL_NUM) ADC_Work_Channel_Index = 0; //keep in range
	adc_config(AdcChannelTable[ADC_Work_Channel_Index].Channel);

}
uint16_t convertRawAdcToCanHeadAddress(uint16_t rawValue)
{
	uint16_t headaddressToReturn = 0xff;
	for (int count = 0; count < 32; count++)
	{
		if (HeadPositionTable2[count].adcRaw > rawValue)
		{
			headaddressToReturn = (uint16_t) HeadPositionTable2[count].value;
			break;
		}
	}
	return headaddressToReturn;
}


