/**
  ******************************************************************************
  * @file           : main.h
  * project			: Analog Can Board
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 11/13/2024
  * firmware		: J-Link SWD
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for C and stm32 headers/hal
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "can.h"
#include "adc.h"
#include "timer.h"
#include "delay.h"

#define ADC2CH13ID 	0x773
#define ADC2CH11ID 	0x7f2
#define ADC2CH4ID 	0x708
#define ADC1CH4ID	0x74b
#define LED_PORT GPIOC

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config(void);
void Led_Config(void);




#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
