#include "serial.h"
#include "stm32f10x_usart.h"
#include "taskmanager/taskmanager.h"
#include "main.h"
uint32_t muxIndexer = 0;


void UART1_Init(uint32_t baudrate)
{
	// 1. Enable clocks
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Enable USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enable GPIOB clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable AFIO for remapping

	// 2. Remap USART1 to PB6/PB7
	AFIO->MAPR |= AFIO_MAPR_USART1_REMAP; // Remap USART1 (PB6=TX, PB7=RX)

	// 3. Configure PB6 (TX) and PB7 (RX) as Alternate Function
	// PB6: Alternate function push-pull (TX)
	GPIOB->CRL &= ~(0xF << 24); // Clear bits for PB6
	GPIOB->CRL |=  (0xB << 24); // 0b1011 = AF Push-Pull, 50MHz

	// PB7: Input floating (RX)  or Pull-up if you prefer
	GPIOB->CRL &= ~(0xF << 28); // Clear bits for PB7
	GPIOB->CRL |=  (0x4 << 28); // 0b0100 = Floating Input

	// 4. Configure USART1
	// Disable USART first
	USART1->CR1 &= ~USART_CR1_UE;

	// Baud rate calculation for 72 MHz PCLK2
	// USARTDIV = PCLK2 / (16 * baudrate)
	//uint32_t pclk2 = 72000000; // Assuming 72 MHz system clock
	//uint32_t usartdiv = (pclk2 + (baudrate / 2)) / (16 * baudrate); // rounded
	
		// 4. Configure USART1 (assuming 72 MHz system clock)
	// 72 000 000 / 19200 / 16 = 234.375 → mantissa 234, fraction 6
	USART1->BRR = (234 << 4) | 6; // 19200 baud @ 72 MHz

	//USART1->BRR = usartdiv;

	// USART configuration: 8N1, TX+RX enabled
	USART1->CR1 = 0;
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // Transmitter + Receiver enable
	USART1->CR1 |= USART_CR1_UE; // Enable USART
}
void checkUart1forMuxCommand()
{
	char receivedChar = 0;
	if (USART1->SR & USART_SR_RXNE) // Check if data is received
	{
		receivedChar = USART1->DR; // Read received character (also clears RXNE flag)
		// Process receivedChar as needed (e.g., store in buffer, parse command, etc.)
		USART1->DR = receivedChar;//echo for now
		//return;//end short for now until we get good echo
		
		switch (receivedChar)
		{
		case '0': muxIndexer = 0; break;
		case '1': muxIndexer = 1; break;
		case '2': muxIndexer = 2; break;
		case '3': muxIndexer = 3; break;
		default: break; //Ignore other characters
		}
		Set_Active_Mux_Channel(muxIndexer);
	}
	
}

	

	
	
	
	
	
	
	
