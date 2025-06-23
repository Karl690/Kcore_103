#include "main.h"
#include "timer.h"
void tim1_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
	// Configure TIM1: 1 MHz clock (1 µs tick)
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72 MHz / 72 = 1 MHz
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; // Max period (65.535 ms)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
	// Enable TIM1
	TIM_Cmd(TIM1, ENABLE);
}