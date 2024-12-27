/**
  ******************************************************************************
  * @file           : spi.c
  * project			: EE329 Lab A4
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/13/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Functions for SPI module, set up as SPI1 through GPIOA.
  *	PTE-15 -> MOSI
  *	PTE-14 -> MISO
  *	PTE-13 -> SCK
  * PTE-12 -> NSS
  *
  ******************************************************************************
  */

#include "spi.h"


// -----------------------------------------------------------------------------

void SPI_init( void ) {
	// setup for SPI at GPIOA and SPI1, adapted from EE329 Lab Manual
	// enable clock for GPIOA & SPI1
	RCC->AHB2ENR |= (SPI_RCC);                // GPIOA: DAC NSS/SCK/SDO
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);                 // SPI1 port
	// configure AF select, push pull, no pu/pd/ fast mode
	SPIPORT->MODER   &= ~(SPI_MODER);
	SPIPORT->MODER   |=  (SPI_MODER_1);
	//SPIPORT->OTYPER  &= ~(SPI_OTYPER);
	//SPIPORT->PUPDR   &= ~(SPI_PUPDR);
	SPIPORT->OSPEEDR |=  (SPI_OSPEEDR);
	// configure AFR for SPI1 function (clear nibble, set 5)
	SPIPORT->AFR[1] &= ~(SPI_AFRCLEAR);
	SPIPORT->AFR[1] |=  (SPI_AFRSET);
   // SPI config as specified @ STM32L4 RM0351 rev.9 p.1459
   // build control registers CR1 & CR2 for SPI control of peripheral DAC
   // assumes no active SPI xmits & no recv data in process (BSY=0)
   // CR1 (reset value = 0x0000)
   SPI1->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
   SPI1->CR1 &= ~( SPI_CR1_RXONLY );          	// recv-only OFF
   SPI1->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
   SPI1->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
   SPI1->CR1 |=	 SPI_CR1_MSTR;              	// MCU is SPI controller
   // CR2 (reset value = 0x0700 : 8b data)
   SPI1->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
   SPI1->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format
   SPI1->CR2 |=	 SPI_CR2_NSSP;              	// auto-generate NSS pulse
   SPI1->CR2 |=	 SPI_CR2_DS;                	// 16-bit data
   SPI1->CR2 |=	 SPI_CR2_SSOE;        	      	// enable SS output
   // CR1
   SPI1->CR1 |=	 SPI_CR1_SPE;               	// re-enable SPI for ops
}










