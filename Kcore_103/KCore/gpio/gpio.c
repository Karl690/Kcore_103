#include "gpio.h"
#include "pinout.h"
#include "pins.h"
void gpio_init()
{
	initClkAndResetAPB2(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2ENR_AFIOEN);
	
	AFIO->MAPR &= 0xf8ffffff;
	AFIO->MAPR |= 0x02000000; //FOR JTAG
	
	pinInit(PIN_LED_50);
	pinInit(PIN_LED_100);
	pinInit(PIN_LED_150);
	pinInit(PIN_LED_175);
}