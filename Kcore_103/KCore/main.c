#include "main.h"
#include "canbus.h"
#ifdef __cplusplus
extern "C"
#endif

uint8_t Booted = 0;
uint8_t DisplayIndex = 0;
uint16_t lastCanHeadAddress = 0xff;
uint16_t CanWatchdogtimer = 0;//setup the canwatchdog timer for 10 second reset 
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

void registerDeviceAndCheckCommWatchdog(void)
{
	if (CanWatchdogtimer == 0)
	{
		if (CanHeadAddress == lastCanHeadAddress)
		{
			// continue trying to register once every second until we get some feedback
			payloadUnion payload;
			//LoadPayloadWithDeviceInfo(1, &payload);
			payload.u32[0] = 0x30303034;
			payload.u32[1] = 0x30304131; //hard coded uvata light pen for now
			canPackIntoTxQueue8x8(CAN_WRITE, CanHeadAddress, CAN_MSG_EVENT_MESSAGE, CAN_EVENT_DEVICE_ANNOUNCE, false, (byte *)&payload);
		
			//SET_SENT_ANNOUNCE_BIT; // log that we sent the request for later processing
			//updateBootSequence(BOOT_STAGE_SENT_ANNOUNCE);
		}
		else
		{
			lastCanHeadAddress = CanHeadAddress; //update for next check address, must be stable for 2 seconds			
		}
	}
	
}

int main(void)
{
	SetSysClockTo72MHZ_16Mhz_XTAL(); //set up pll to multiply 16mhz xtal to 144mhz/2 = 72mhz clock

	gpio_init();
	
	DisplayInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	//canInit();
	Init_ADC_DMA_Circular_Mode(); //setup adc in dma mode for autmatic conversion
	Start_ADC_IN_CONTNOUS_DMA_MODE();
	init_TIM1(); //turn on mux pwm for channels 1,2,3,4
	init_TIM3(); //turn on mux pwm for channels 5,6,7,8
	//UART1_Init_19200(); //enable uart for mux selection
	
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
		//delay_sec(1);
		//pinToggleOutput(PIN_LED_175);
	}
}


void SysTick_Handler(void)
{
	func_SystickCallback();
}
void SetSysClockTo72MHZ_16Mhz_XTAL(void)
{
	__IO uint32_t HSEStatus = 0;

	/* Enable HSE */
	RCC->CR |= RCC_CR_HSEON;

	/* Wait till HSE is ready */
	do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
	} while (HSEStatus == 0);

	if ((RCC->CR & RCC_CR_HSERDY) != RESET)
	{
		/* Enable Prefetch Buffer and set 2 wait states (required for 72 MHz) */
		FLASH->ACR |= FLASH_ACR_PRFTBE;
		FLASH->ACR &= ~FLASH_ACR_LATENCY;
		FLASH->ACR |= FLASH_ACR_LATENCY_2; // 2 wait states

		/* HCLK = SYSCLK (no division) */
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

		/* PCLK2 = HCLK (max 72 MHz) */
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

//		/* PCLK1 = HCLK / 2 (max 36 MHz) */
//		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
//
//		/* PLL config: (HSE/2) * 9 = 72 MHz */
//		RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
//		RCC->CFGR |= (RCC_CFGR_PLLSRC_HSE | 
//		              RCC_CFGR_PLLXTPRE |      // HSE/2
//		              RCC_CFGR_PLLMULL9);
		/* PLL configuration: PLLCLK = (HSE / 2) * 9 = 72 MHz */
		RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);

		/* PLLSRC = HSE (not HSI/2) */
		/* PLLXTPRE = HSE divided by 2 */
		/* PLLMULL = x9 */
		RCC->CFGR |= (RCC_CFGR_PLLSRC_HSE | 
		              RCC_CFGR_PLLXTPRE |        // <--- This is the missing bit!
		              RCC_CFGR_PLLMULL9);
		

		/* Enable PLL */
		RCC->CR |= RCC_CR_PLLON;

		/* Wait till PLL is ready */
		while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		{
		}

		/* Select PLL as system clock source */
		RCC->CFGR &= ~RCC_CFGR_SW;
		RCC->CFGR |= RCC_CFGR_SW_PLL;

		/* Wait till PLL is used as system clock source */
		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
		{
		}

		/* Optional: update SystemCoreClock variable if you use it */
		// SystemCoreClock = 72000000;
	}
	else
	{
		/* HSE failed to start - add error handling here */
		// e.g. NVIC_SystemReset(); or fallback to HSI
	}
}

//void SetSysClockTo72MHZ_16Mhz_XTAL(void)
//{
//	__IO uint32_t HSEStatus = 0; // GB XXX ADDED
// 
//	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
//	/* Enable HSE */    
//	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
// 
//	/* Wait till HSE is ready and if Time out is reached exit */
//	do
//	{
//		HSEStatus = RCC->CR & RCC_CR_HSERDY;
//	} while (HSEStatus == 0);  // wait forever;  // GB XXX ADDED
//
//	if ((RCC->CR & RCC_CR_HSERDY) != RESET)
//	{
//		HSEStatus = (uint32_t)0x01;
//	}
//	else
//	{
//		HSEStatus = (uint32_t)0x00;
//	}  
//
//	if (HSEStatus == (uint32_t)0x01)
//	{
//		/* Enable Prefetch Buffer */
//		FLASH->ACR |= FLASH_ACR_PRFTBE;
//
//		/* Flash 2 wait state */
//		FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
//		FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    
//
// 
//		/* HCLK = SYSCLK */
//		RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
//      
//		/* PCLK2 = HCLK */
//		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
//    
//		/* PCLK1 = HCLK */
//		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
//
//  
//		/*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
//		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
//		                                    RCC_CFGR_PLLMULL));
//		
//		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9); //works for 16mhz xtal RCC_CFGR_PLLMULL4
//		//RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL4); //works for 16mhz xtal
//
//		/* Enable PLL */
//		RCC->CR |= RCC_CR_PLLON;
//
//		/* Wait till PLL is ready */
//		while ((RCC->CR & RCC_CR_PLLRDY) == 0)
//		{
//		}
//    
//		/* Select PLL as system clock source */
//		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
//		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    
//
//		/* Wait till PLL is used as system clock source */
//		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
//		{
//		}
//	}
//	else
//	{
//		/* If HSE fails to start-up, the application will have wrong clock 
//	       configuration. User can add here some code to deal with this error */
//		// GB XXX ADDED - NVIC_SystemReset();
//	}
//}
void init_TIM1()
{
	//will set up all 4 outputs for timer3 to activate the mux channels 5,6,7,8
		
			// 1. Enable Clocks
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Enable TIM1 Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable GPIOA Clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable Alternate Function Clock

	// 2. Configure PA2 as Alternate Function Push-Pull (50MHz)
	//		// Clear CNF2[1:0] and MODE2[1:0] for PA2 (CRL register)
	//		GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
	//		// Set MODE2 = 11 (Output 50MHz), CNF2 = 10 (AF Push-Pull)
	//		GPIOA->CRL |= (GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1 | GPIO_CRL_CNF2_1);
	//		pinInit(PIN_A8_Tim1_CH1);//(PIN_PORT_A | PIN_NUM_08 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch1
	//		pinInit(PIN_A9_Tim1_CH2); //(PIN_PORT_A | PIN_NUM_09 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch1
	//		pinInit(PIN_A10_Tim1_CH3); //(PIN_PORT_A | PIN_NUM_10 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch1
	//		pinInit(PIN_A11_Tim1_CH4); //
	GPIOA->CRH &= ~(0xFFFFFFFF); // Clear PA8–PA11
	GPIOA->CRH |= 0xBBBBBBBB; // 50 MHz AF PP for PA[15:8]
		// 3. TIM1 configuration
	TIM1->PSC = 35; // Prescaler = 71 → timer clock = 72 MHz / 72 = 1 MHz
	TIM1->ARR = 99; // Auto-reload = 99 → period = 100 ticks → 10 kHz

	// PWM Mode 1 for all channels (OCxM = 110)
	TIM1->CCMR1 = (6 << 4) | (6 << 12); // CH1 & CH2: PWM mode 1
	TIM1->CCMR2 = (6 << 4) | (6 << 12); // CH3 & CH4: PWM mode 1

	// Enable preload for smooth duty updates
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
	TIM1->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;

	// Polarity active high (default), enable all channels
	TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E  | TIM_CCER_CC2P | TIM_CCER_CC4P;

	// Initial duties (0..99)
	TIM1->CCR1 = 5; // 50%
	TIM1->CCR2 = 10; // 25%
	TIM1->CCR3 = 15; // 75%
	TIM1->CCR4 = 20; // 10%

	// 7. Enable Timer
	TIM1->CR1 |= TIM_CR1_ARPE; // Auto-reload preload enable
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->CR1 |= TIM_CR1_CEN; // Counter Enable
}

////////////////////////////////////////////////////////////////////////////////
/*Timer2 used for pwm generation on PA2  Tim2_Ch3
 *used with driving laser duty, inkjet pulse, uvata duty
 **/

void init_TIM2()
{
	//for laser, inkjet, uvata duty pwm
	// STM32F103C8 TIM2 Channel 3 Output on PA2
// Assumes 72MHz System Clock (standard BluePill)
		// 1. Enable Clocks
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable GPIOA Clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable Alternate Function Clock

	// 2. Configure PA2 as Alternate Function Push-Pull (50MHz)
	// Clear CNF2[1:0] and MODE2[1:0] for PA2 (CRL register)
	GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
	// Set MODE2 = 11 (Output 50MHz), CNF2 = 10 (AF Push-Pull)
	GPIOA->CRL |= (GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1 | GPIO_CRL_CNF2_1);

	// 3. Configure Timer Timebase
	//TIM2->PSC = 72 - 1; // Prescaler: 72MHz / 72 = 1MHz timer clock (1us per tick)
	TIM2->PSC = 40; // Prescaler: 72MHz / 72 = 1MHz timer clock (1us per tick)
	TIM2->ARR = 1000 - 1; // Auto-Reload: 1000 ticks = 1ms period = 1kHz PWM frequency

	// 4. Configure Timer Channel 3 for PWM Mode
	// CCMR2 register: Channel 3 is in high byte. 
	// OC3M = 110 (PWM Mode 1)
	TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear PWM mode bits
	TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // OC3M = 110
	TIM2->CCMR2 |= TIM_CCMR2_OC3PE; // Enable Preload for Channel 3

	// 5. Configure Output Polarity and Enable
	TIM2->CCER |= TIM_CCER_CC3E; // Enable Channel 3 Output
	TIM2->CCER &= ~TIM_CCER_CC3P; // Active High polarity

	// 6. Set Duty Cycle (Example: 50% = 500 / 1000)
	TIM2->CCR3 = 1;

	// 7. Enable Timer
	TIM2->CR1 |= TIM_CR1_ARPE; // Auto-reload preload enable
	TIM2->CR1 |= TIM_CR1_CEN; // Counter Enable
}

////////////////////////////////////////////////////////////////////////////////
//#define PIN_A6_Tim3_CH1         (PIN_PORT_A | PIN_NUM_06 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch1
//#define PIN_A7_Tim3_CH2         (PIN_PORT_A | PIN_NUM_07 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch2
//#define PIN_B0_Tim3_CH3         (PIN_PORT_B | PIN_NUM_00 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch3
//#define PIN_B0_Tim3_CH4         (PIN_PORT_B | PIN_NUM_01 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch4
void init_TIM3(void)
{
	//will set up all 4 outputs for timer3 to activate the mux channels 5,6,7,8
		
			// 1. Enable Clocks
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable TIM2 Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable GPIOA Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enable GPIOB Clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable Alternate Function Clock

	// 2. Configure PA2 as Alternate Function Push-Pull (50MHz)
	//		// Clear CNF2[1:0] and MODE2[1:0] for PA2 (CRL register)
	//		GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
	//		// Set MODE2 = 11 (Output 50MHz), CNF2 = 10 (AF Push-Pull)
	//		GPIOA->CRL |= (GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1 | GPIO_CRL_CNF2_1);
	pinInit(PIN_A6_Tim3_CH1); //(PIN_PORT_A | PIN_NUM_06 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch1
	pinInit(PIN_A7_Tim3_CH2); //(PIN_PORT_A | PIN_NUM_07 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch2
	pinInit(PIN_B0_Tim3_CH3); //(PIN_PORT_B | PIN_NUM_00 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch3
	pinInit(PIN_B1_Tim3_CH4); //(PIN_PORT_B | PIN_NUM_01 | PIN_INIT_LOW    | AF_OPP_50MHZ)   // TIM3 Ch4

	// 2. Configure GPIO as AF Push-Pull 50 MHz
	    // PA6 = TIM3_CH1, PA7 = TIM3_CH2
	    //		GPIOA->CRL &= ~(0xFF << 24); // Clear PA6 & PA7
	    //		GPIOA->CRL |=  (0xBB << 24); // 50 MHz AF PP (MODE=11, CNF=10)
	    //
	    //		// PB0 = TIM3_CH3, PB1 = TIM3_CH4
	    //		GPIOB->CRL &= ~(0xFF << 0); // Clear PB0 & PB1
	    //		GPIOB->CRL |=  (0xBB << 0); // 50 MHz AF PP

	    		// 3. TIM3 configuration
	TIM3->PSC = 20; // Prescaler = 71 → timer clock = 72 MHz / 72 = 1 MHz
	TIM3->ARR = 99; // Auto-reload = 99 → period = 100 ticks → 10 kHz

	// PWM Mode 1 for all channels (OCxM = 110)
	TIM3->CCMR1 = (6 << 4) | (6 << 12); // CH1 & CH2: PWM mode 1
	TIM3->CCMR2 = (6 << 4) | (6 << 12); // CH3 & CH4: PWM mode 1

	// Enable preload for smooth duty updates
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
	TIM3->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;

	// Polarity active high (default), enable all channels
	TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E  | TIM_CCER_CC2P | TIM_CCER_CC4P;

	// Initial duties (0..99)
	TIM3->CCR1 = 5; // 50%
	TIM3->CCR2 = 10; // 25%
	TIM3->CCR3 = 15; // 75%
	TIM3->CCR4 = 20; // 10%

	// 7. Enable Timer
	TIM3->CR1 |= TIM_CR1_ARPE; // Auto-reload preload enable
	TIM3->CR1 |= TIM_CR1_CEN; // Counter Enable
}
