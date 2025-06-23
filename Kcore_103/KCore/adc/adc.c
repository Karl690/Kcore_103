#include "main.h"
#include "adc.h"

#define CovertionTableIndexSize 16
float ConvertionTable[CovertionTableIndexSize] = {1, 1, 1, 1, 1, 1, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 1};
ADC_ChannelDef AdcChannelTable[ADC_NUM_CHANNELS] = {
	{ ADC_CHANNEL_POSITION, PIN_ADC_POSITION, ConvertionTable },
	{ ADC_CHANNEL_RTD1, PIN_ADC_RTD1_IN, ConvertionTable },
	{ ADC_CHANNEL_RTD2, PIN_ADC_RTD2_IN, ConvertionTable },
} ;

uint16_t   RawADCDataBuffer[ADC_NUM_CHANNELS] = { 0 };


void adc_config(uint8_t channel, pinType pin) {
	//define type of analog to digital conversion
	// make copy of ADC_RegularChannelConfig focus on rank=1; ADC_SampleTime_71Cycles5, and limited input channel
	// ONLY GOOD TO CHAN 9
	// ADC_RegularChannelConfig(ADC1, adcp->inputChannel, 1, ADC_SampleTime_71Cycles5);
	
	// initialize the ADC GPIO pin
	pinInit(pin);
	
	// initialize the ADC channel
	uint32_t tmpreg1, tmpreg2;
	tmpreg1 = ADC1->SMPR2; // Get the old register value

#define SMPR2_SMP_Set               ((uint32_t)0x00000007)
	tmpreg2 = SMPR2_SMP_Set << (3 * channel); // Calculate the mask to clear
	tmpreg1 &= ~tmpreg2; // Clear the old channel sample time
	tmpreg2 = (uint32_t)ADC_SampleTime_71Cycles5 << (3 * channel); // Calculate the mask to set
	tmpreg1 |= tmpreg2; // Set the new channel sample time
	ADC1->SMPR2 = tmpreg1; // Store the new register value
#define RANK_1  1
	tmpreg1 = ADC1->SQR3; // Get the old register value
#define SQR3_SQ_Set                 ((uint32_t)0x0000001F)
	tmpreg2 = SQR3_SQ_Set << (5 * (RANK_1 - 1)); // Calculate the mask to clear
	tmpreg1 &= ~tmpreg2; // Clear the old SQx bits for the selected rank
	tmpreg2 = (uint32_t)channel << (5 * (RANK_1 - 1)); // Calculate the mask to set
	tmpreg1 |= tmpreg2; // Set the SQx bits for the selected rank
	ADC1->SQR3 = tmpreg1; // Store the new register value

	//ADC_Cmd (ADC1, ENABLE);                     // enable ADC1
#define CR2_ADON_Set                ((uint32_t)0x00000001)
	ADC1->CR2 |= CR2_ADON_Set;

	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);     // start actual conversion
#define CR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)
	ADC1->CR2 |= CR2_EXTTRIG_SWSTART_Set;
	
}

void adc_init(void)
{
	uint8_t i = 0;
	for (uint8_t i = 0; i < ADC_NUM_CHANNELS; i ++)
	{	
		adc_config(AdcChannelTable[i].Channel, AdcChannelTable[i].Pin);
	}
	
}

void adc_start()
{
	// it need to call this function if it is in DMA mode
}

float ScaledADCData[ADC_NUM_CHANNELS] = { 0 }; //converted adc buffer values
uint16_t ADC_Work_Channel_Index = 0; //used to walk thru the channels and update the working variables.
adcStruct ADC_Channel[ADC_NUM_CHANNELS];
adcStruct *ADC_Work_Channel;


uint16_t sum = 0; 
uint16_t raw = 0; 
uint16_t low = 0; 
uint16_t high = 0; 
uint16_t i = 0; //persistant variables
float adc_conversionFactor = 3.3 / 4096;


void ProcessRawADC_Data(void)
{
	SmoothDataUsingOlympicVotingAverage();
}

void SmoothDataUsingOlympicVotingAverage(void)
{
	ADC_Work_Channel = &ADC_Channel[ADC_Work_Channel_Index];
	ADC_Work_Channel->adcRaw += RawADCDataBuffer[ADC_Work_Channel_Index]; //update last reading
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

		ADC_Work_Channel->convAvg = ScaledADCData[ADC_Work_Channel_Index] = (float)((float)ADC_Work_Channel->adcAvg * adc_conversionFactor);
		//		ADC_Work_Channel->convAvg = ScaledADCData[ADC_Work_Channel_Index] = (float)(((float)ADC_Work_Channel->adcAvg * 3.3) / 4095);
	}
	// setup next conversion so data will be ready for the next call in ~10ms
	ADC_Work_Channel_Index++;
	if (ADC_Work_Channel_Index >= ADC_NUM_CHANNELS) ADC_Work_Channel_Index = 0; //keep in range

}
