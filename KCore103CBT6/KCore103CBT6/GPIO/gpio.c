#include "gpio.h"

void gpio_init()
{
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	pinInit(PIN_LED_50);
	pinInit(PIN_LED_100);
	pinInit(PIN_LED_150);
	pinInit(PIN_LED_175);
}