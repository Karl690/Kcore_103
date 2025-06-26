#include "main.h"
//#ifdef __cplusplus
//extern "C"
//#endif
	
uint8_t Booted = 0;
uint8_t DisplayIndex = 0;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	func_SystickCallback();
}

void Error_Handler(void)
{

}
int main(void)
{
	HAL_Init();
	
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	/* System interrupt init*/

	/** NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
	*/
	__HAL_AFIO_REMAP_SWJ_NONJTRST();

	gpio_init();
	adc_init();
	DisplayInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	SysTick_Config(SystemCoreClock / SYSTICKS_PER_SECOND);
	
	Booted = 1;
	// adc_start();
	
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
