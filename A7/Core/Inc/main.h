/**
  ******************************************************************************
  * @file           : main.h
  * project			: EE329 Lab A5
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/12/2024
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
void Execute(const char *messages[]);
void Bubbles(void);
void LPUART1_IRQHandler( void  );

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif
