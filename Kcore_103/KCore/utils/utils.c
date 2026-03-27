#include "utils.h"
void delay_us(uint32_t us)
{
	uint32_t count = us * 12; // Approximate: 72MHz / 6 ≈ 12 loops per µs (with optimization)
    
	for (volatile uint32_t i = 0; i < count; i++)
	{
		__NOP(); // No Operation - helps with timing
	}
	
//	TIM_SetCounter(TIM1, 0); // Reset counter
//	while (TIM_GetCounter(TIM1) < us) ; // Wait until counter reaches 'us'
}


void delay_ms(uint32_t ms)
{
	for (uint32_t i = 0; i < ms; i++) {
		delay_us(1000); // 1000 µs = 1 ms
	}
}

////////////////////////////////////////////////////////////////////////////////

void delay_sec(uint32_t sec)
{
	for (int i = 0; i < sec; i++)
	{
		delay_ms(1000);
	}
}


void initClkAndResetAPB2(uint32_t periph)
{
	RCC->APB2ENR  &= ~(periph); // disable clock for peripheral
	RCC->APB2RSTR |=  (periph); // set reset for peripheral
	RCC->APB2ENR  |=  (periph); // enable clock for peripheral
	RCC->APB2RSTR &= ~(periph); // remove reset from peripheral
}