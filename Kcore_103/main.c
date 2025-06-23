#include <stm32f1xx_hal.h>
#include <stm32_hal_legacy.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000); // set vector table to flash
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // Configure all 4 bits for preemption priority; no subpriority
}
