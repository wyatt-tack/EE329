/**
  ******************************************************************************
  * @file           : lcd.h
  * project			: EE329 Lab A4
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/13/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for timer.h
  *
  ******************************************************************************
  */
#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32l4xx_hal.h"

#define PERIOD (0xFFFFFFFF) //Tim2 Counter Period

void setup_TIM2( int iDutyCycle );
void TIM2_IRQHandler(void);
void setup_MCO_CLK(void);
void setup_RNG(void);

#endif /* INC_TIMER_H_ */
