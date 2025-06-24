#include "adc_1xx.h"
void adc_config(uint8_t channel) {
	//define type of analog to digital conversion
	// make copy of ADC_RegularChannelConfig focus on rank=1; ADC_SampleTime_71Cycles5, and limited input channel
	// ONLY GOOD TO CHAN 9
	// ADC_RegularChannelConfig(ADC1, adcp->inputChannel, 1, ADC_SampleTime_71Cycles5);
	
	
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
