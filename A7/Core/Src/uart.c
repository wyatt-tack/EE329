/**
  ******************************************************************************
  * @file           : uart.c
  * project			: EE329 Lab A4
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/13/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Functions for UART module, set up as LPUART1 through GPIOG.
  *	PTG-7 -> Tx
  *	PTG-8 -> Rx
  *
  ******************************************************************************
  */

#include "uart.h"


// -----------------------------------------------------------------------------
void UART_Init(void){

	//Power and Clock
	PWR->CR2 |= (PWR_CR2_IOSV);              // power avail on PG[15:2] (LPUART1)
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOGEN);   // enable GPIOG clock
	RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN; // enable LPUART clock bridge
	//GPIO Ports - AF8, no PU/PD, fast (despite uart being slow)
	GPIOG->MODER   &= ~(GPIO_MODER_MODE7 | GPIO_MODER_MODE8);
	GPIOG->MODER   |=  (GPIO_MODER_MODE7_1 | GPIO_MODER_MODE8_1);
	GPIOG->OTYPER  &= ~(GPIO_OTYPER_OT7 | GPIO_OTYPER_OT8);
	GPIOG->PUPDR   &= ~(GPIO_PUPDR_PUPD7 | GPIO_PUPDR_PUPD8);
	GPIOG->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED7_Pos) | (3 << GPIO_OSPEEDR_OSPEED8_Pos));
	GPIOG->AFR[0] &= ~(0x000F << GPIO_AFRL_AFSEL7_Pos);
	GPIOG->AFR[0] |=  (0x0008 << GPIO_AFRL_AFSEL7_Pos);
	GPIOG->AFR[1] &= ~(0x000F << GPIO_AFRH_AFSEL8_Pos);
	GPIOG->AFR[1] |=  (0x0008 << GPIO_AFRH_AFSEL8_Pos);
	//LPUART
	LPUART1->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 8-bit data
	LPUART1->CR1 |= USART_CR1_UE;                   // enable LPUART1
	LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);  // enable xmit & recv
	LPUART1->CR1 |= USART_CR1_RXNEIE;        // enable LPUART1 recv interrupt
	LPUART1->ISR &= ~(USART_ISR_RXNE);       // clear Recv-Not-Empty flag
	LPUART1->BRR = (BAUD_RATE);
	/* USER: set baud rate register (LPUART1->BRR) */
	NVIC->ISER[2] = (1 << (LPUART1_IRQn & 0x1F));   // enable LPUART1 ISR
	__enable_irq();

}

void LPUART_Print( const char* message ) {
   uint16_t iStrIdx = 0;
   while ( message[iStrIdx] != 0 ) {
      while(!(LPUART1->ISR & USART_ISR_TXE)) // wait for empty xmit buffer
         ;
      LPUART1->TDR = message[iStrIdx];       // send this character
	iStrIdx++;                             // advance index to next char
   }
}

void LPUART_Print_Char (uint8_t charRecv){
	while( !(LPUART1->ISR & USART_ISR_TXE) );// wait for empty TX buffer
		LPUART1->TDR = charRecv;  // send char to terminal
		}




