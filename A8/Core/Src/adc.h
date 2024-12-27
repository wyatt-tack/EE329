/**
  ******************************************************************************
  * @file           : adc.h
  * project			: EE329 Lab A8
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 11/6/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for adc.h
  *
  ******************************************************************************
  */
#ifndef SRC_ADC_H_
#define SRC_ADC_H_

#include "stm32l4xx_hal.h"

void ADC_init(void);
void ADC1_2_IRQHandler(void);

extern volatile uint16_t ADC_Samples [20];
extern volatile uint16_t ADC_SampleCount;

#endif /* SRC_ADC_H_ */
