/**
  ******************************************************************************
  * @file           : main.h
  * project			: EE329 Lab A3
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/5/2024
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

#define STAR_KEY (0x0A)
#define POUND_KEY (0x0C)

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
void Led_Config(void);

#ifdef __cplusplus
}
#endif

#endif
