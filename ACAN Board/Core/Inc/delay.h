/**
  ******************************************************************************
  * @file           : delay.h
  * project			: EE329 Lab A3
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/1/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for delay.h
  *
  ******************************************************************************
  */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "stm32g4xx_hal.h"
void SysTick_Init(void);
void delay_us(const uint32_t time_us);

#endif /* INC_DELAY_H_ */
