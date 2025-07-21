#include "main.h"
#ifdef __cplusplus
extern "C"
#endif
	
uint8_t Booted = 0;
uint8_t DisplayIndex = 0;
void SystemClock_Config(void);

void Error_Handler(void)
{

}
int main(void)
{
	HAL_Init();
	
	// __HAL_RCC_AFIO_CLK_ENABLE();
	// __HAL_RCC_PWR_CLK_ENABLE();

	/* System interrupt init*/

	/** NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
	*/
	// __HAL_AFIO_REMAP_SWJ_NONJTRST();

	SystemClock_Config();
	gpio_init();
	adc_init();
	//MX_USB_DEVICE_Init();
	DisplayInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	SysTick_Config(SystemCoreClock / SYSTICKS_PER_SECOND);
	
	Booted = 1;
	adc_start();
	
	for (;;)
	{	
		switch (DisplayIndex)
		{
			case 0:
				UpdateScreen(LcdVarsTable);
				break;
			case 1:
				UpdateScreen(TaskTimeTable1);
				break;
			case 2:
				UpdateScreen(TaskTimeTable2);
				break;
		default:
			break;
		}
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	  /**Initializes the CPU, AHB and APB busses clocks 
	  */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}

	  /**Configure the Systick interrupt time 
	  */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick 
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}