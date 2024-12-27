/**
  ******************************************************************************
  * @file           : main.h
  * project			: EE329 Lab A4
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/9/2024
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
#define BUTTON_PORT GPIOC
#define LED_PORT GPIOB
#define BUTTON_PRESS ((((~(GPIOC->IDR)) & GPIO_PIN_13) >> 13))

void Button_Config(void);
void Button_Press(void);
void Write_Time(int timeTaken);
void Led_Config(void);
void setup_TIM2( int iDutyCycle );
void set_TIM2( int period );
void TIM2_IRQHandler(void);
void setup_MCO_CLK(void);
void setup_RNG(void);

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif
