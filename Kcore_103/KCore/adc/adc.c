#include "main.h"
#include "adc.h"
#include "ADC_Tables.h"
#include "AD_Channel_Definitions.h"
uint16_t   RawADCDataBuffer[ADC_NUM_CHANNELS] = { 0 };

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
	for (uint8_t i = 0; i < ADC_NUM_CHANNELS; i ++)
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

float ScaledADCData[ADC_NUM_CHANNELS] = { 0 }; //converted adc buffer values
uint16_t ADC_Work_Channel_Index = 0; //used to walk thru the channels and update the working variables.
adcStruct ADC_Channel[ADC_NUM_CHANNELS];
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
		SmoothDataUsingOlympicVotingAverage();	
	}
	
}

void SmoothDataUsingOlympicVotingAverage(void)
{
	ADC_Work_Channel = &ADC_Channel[ADC_Work_Channel_Index];
	// ADC_Work_Channel->adcRaw += RawADCDataBuffer[ADC_Work_Channel_Index]; //update last reading
	ADC_Work_Channel->adcRaw += ADC_GetConversionValue(ADC1); // read ADC value (range 0 to 4095
	ADC_Work_Channel->adcRaw = ADC_Work_Channel->adcRaw >> 1; //average from last reading
	//now we need to plug into the 10 reading buffer for smoothing.
	ADC_Work_Channel->sampleHistory[ADC_Work_Channel->sampleIndex] = ADC_Work_Channel->adcRaw; //plug in the latest reading.
	ADC_Work_Channel->sampleIndex++; //point to next place to enter the data in smoothing array
	if (ADC_Work_Channel->sampleIndex > 9)
	{
		ADC_Work_Channel->sampleIndex = 0; //limit to 10 entries	
		  // history buffer is full, so enough good adc values to proceed
			// this code does olympic voting (toss high and low and then average the rest)
			// the ADC_NUM_SAMPLES must be equal to (2^n) + 2 and ADC_SHIFT_FOR_AVG
			// must equal "n", as the code will shift to get the average instead of divide. set temporary
			// variables to record highest and lowest values as each of the ADC_NUM_SAMPLES is inspected
			// at the same time, record the sum of all ADC_NUM_SAMPLES samples.  when done looking at all values,
			// subtract the high and low from the sum and then calculate the average of the remaining sum.
		
		low = 0xffff; // MAXINT
		high = 0; // MININT
		sum = 0;
		for (i = 0; i < ADC_NUM_SAMPLES; i++)
		{
			raw = ADC_Work_Channel->sampleHistory[i];
			sum += raw; // keep running total
			if (raw < low) low = raw; // update the lowest reading
			if (raw > high)high = raw; // update the highest reading
		}
		sum -= (low + high); // sum is now the total of the 8 middle N values
		sum = sum >> ADC_SHIFT_FOR_AVG; //divide by 8
		ADC_Work_Channel->adcAvg += sum;
		ADC_Work_Channel->adcAvg = ADC_Work_Channel->adcAvg >> 1; //divide by 2, so now it is the AVERAGE of the last 2 readings

		ADC_Work_Channel->convAvg = AdcConvertValue(AdcChannelTable[ADC_Work_Channel_Index].converstionType, ADC_Work_Channel->adcAvg, AdcChannelTable[ADC_Work_Channel_Index].ConvertionTable);
		ScaledADCData[ADC_Work_Channel_Index] = ADC_Work_Channel->convAvg;
		//		ADC_Work_Channel->convAvg = ScaledADCData[ADC_Work_Channel_Index] = (float)(((float)ADC_Work_Channel->adcAvg * 3.3) / 4095);
	}
	// setup next conversion so data will be ready for the next call in ~10ms
	ADC_Work_Channel_Index++;
	if (ADC_Work_Channel_Index >= ADC_NUM_CHANNELS) ADC_Work_Channel_Index = 0; //keep in range
	
	// it needs to setup channel after reading in pulling mode
	adc_config(AdcChannelTable[ADC_Work_Channel_Index].Channel); // set up next channel
}
