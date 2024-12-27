/**
  ******************************************************************************
  * @file           : main.h
  * project			: EE329 Lab A1
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 9/27/2024
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif
