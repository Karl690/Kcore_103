#include "main.h"

#ifdef __cplusplus
extern "C"
#endif

uint8_t Booted = 0;
uint8_t DisplayIndex = 0;
void SetupIWDG(uint32_t interval)
{
	// if goal is a 5 second watchdog....CNT = 5 * 125 = 625 nom.... (could be off by a factor of 2
	// due to mfg tolerance. not worth calibrating, so double result to be safe ... so scaling
	// is 32000 / 256 * 2 = 250

	IWDG->KR = IWDG_WriteAccess_Enable; // Enable write access to IWDG_PR and IWDG_RLR register
	IWDG->PR = IWDG_Prescaler_256; // prescale from nom 32KHz clock to nom 125Hz (8ms period)
	IWDG->RLR = (uint16_t)(interval * 250); // set reload value/count
	IWDG->KR = IWDG_KR_KEY_Reload; // Reload IWDG counter
	IWDG->KR = IWDG_KR_KEY_Enable; // turn on watchdog ... can't be disable
}


int main(void)
{
	tim1_init();
	gpio_init();
	
	DisplayInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	adc_init();
	
	SysTick_Config(SystemCoreClock / SYSTICKS_PER_SECOND);
	
	Booted = 1;
	while (1)
	{
		DisplayIndex %= 3;
		switch (DisplayIndex)
		{
		case 0:UpdateScreen(LcdVarsTable); break;
		case 1:UpdateScreen(TaskTimeTable1); break;
		case 2:UpdateScreen(TaskTimeTable2); break;
		}
		delay_sec(1);
		pinToggleOutput(PIN_LED_175);
	}
}


void SysTick_Handler(void)
{
	func_SystickCallback();
}