#pragma once
#include "stm32f10x_conf.h"
#include "stm32f10x_misc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_can.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"

#include "gpio/pins.h"
#include "gpio/pinout.h"
#include "timer/timer.h"
#include "gpio/gpio.h"
#include "display/display.h"
#include "display/gui.h"
#include "adc/adc.h"

#include "taskmanager/taskmanager.h"
#include "utils/utils.h"

#define IWDG_KR_KEY_Reload    ((uint16_t)0xAAAA)
#define IWDG_KR_KEY_Enable    ((uint16_t)0xCCCC)

#define DELAY_TIMER							TIM1

extern uint8_t Booted;