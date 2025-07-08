#ifdef USE_ADC
#include "adc_1xx.h"

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;
void adc_1xx_init(int channels) {
	
	hadc.Instance = ADC1;
	hadc.Init.ScanConvMode = ENABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = channels;
	if (HAL_ADC_Init(&hadc) != HAL_OK)
	{
		Error_Handler();
	}
	ADC_ChannelConfTypeDef sConfig = { 0 };

	// Configure ADC channels (example: PA0 and PA1)
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
}

void adc_1xx_channel_config(uint8_t channel, uint8_t rank)
{
	ADC_ChannelConfTypeDef sConfig = { 0 };

	sConfig.Channel = channel;
	sConfig.Rank = rank;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

void adc_1xx_dma_config()
{
	__HAL_RCC_DMA1_CLK_ENABLE();
    
	hdma_adc.Instance = DMA1_Channel1;
	hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc.Init.Mode = DMA_CIRCULAR;
	hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
	if (HAL_DMA_Init(&hdma_adc) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&hadc, DMA_Handle, hdma_adc);

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void adc_1xx_start(uint16_t* adcValues, uint32_t channel)
{
	if (HAL_ADC_Start_DMA(&hadc, (uint32_t*)adcValues, channel) != HAL_OK)
	{
		Error_Handler();
	}
}
#endif