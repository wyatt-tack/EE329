/**
  ******************************************************************************
  * @file           : spi.h
  * project			: EE329 Lab A4
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/13/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for spi.h
  *
  ******************************************************************************
  */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32l4xx_hal.h"

#define SPIPORT GPIOE
#define SPI_RCC RCC_AHB2ENR_GPIOEEN
#define SPI_MODER (GPIO_MODER_MODE12 | GPIO_MODER_MODE13 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15)
#define SPI_MODER_1 (GPIO_MODER_MODE12_1 | GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1)
#define SPI_OTYPER (GPIO_OTYPER_OT12 | GPIO_OTYPER_OT13 | GPIO_OTYPER_OT14 | GPIO_OTYPER_OT15)
#define SPI_PUPDR (GPIO_PUPDR_PUPD12 | GPIO_PUPDR_PUPD13 | GPIO_PUPDR_PUPD14 | GPIO_PUPDR_PUPD15)
#define SPI_OSPEEDR ((3 << GPIO_OSPEEDR_OSPEED12_Pos) | (3 << GPIO_OSPEEDR_OSPEED13_Pos)| (3 << GPIO_OSPEEDR_OSPEED14_Pos) | (3 << GPIO_OSPEEDR_OSPEED15_Pos))
#define SPI_AFRCLEAR ((0x000F << GPIO_AFRH_AFSEL12_Pos) | (0x000F << GPIO_AFRH_AFSEL13_Pos)| (0x000F << GPIO_AFRH_AFSEL14_Pos) | (0x000F << GPIO_AFRH_AFSEL15_Pos))
#define SPI_AFRSET ((0x0005 << GPIO_AFRH_AFSEL12_Pos) | (0x0005 << GPIO_AFRH_AFSEL13_Pos) | (0x0005 << GPIO_AFRH_AFSEL14_Pos) | (0x0005 << GPIO_AFRH_AFSEL15_Pos))

void SPI_init( void );

#endif /* INC_SPI_H_ */
