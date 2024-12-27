/**
  ******************************************************************************
  * @file           : main.h
  * project			: EE329 Lab A9
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/21/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for C and stm32 headers/hal
  *
  ******************************************************************************
  */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Created defines and function prototypes -----------------------------------*/


/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
#define EEPROM_ADDRESS 0x54
#define LED_PORT GPIOB

void Led_Config(void);
void SystemClock_Config(void);
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif
