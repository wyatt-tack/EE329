/**
  ******************************************************************************
  * @file           : main.h
  * project			: EE329 Lab A8
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 11/6/2024
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

#include <stdint.h>

/* Created defines and function prototypes -----------------------------------*/


#define RESISTOR (10113) 	//in milliohms
#define IBASE (1153)		//in microamps
#define BUTTON_PORT GPIOC
#define LED_PORT GPIOB
#define BUTTON_PRESS ((((~(GPIOC->IDR)) & GPIO_PIN_13) >> 13))
void Led_Config(void);
void Button_Config(void);
void Output_Config(void);

uint16_t maxVal (uint16_t* nums );
uint16_t minVal (uint16_t* nums );
uint16_t avgVal (uint16_t* nums );
void HomeScreen(void);
void sendHexData(uint16_t minVal, uint16_t maxVal, uint16_t avgVal);
void sendVoltData(uint16_t minVal, uint16_t maxVal, uint16_t avgVal);
void parseSendHex(uint16_t value, uint8_t row);
void parseSendVolt(uint16_t value, uint8_t row);
void sendCurrentData(uint16_t value);

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"


/* Exported functions prototypes ---------------------------------------------*/


void SystemClock_Config(void);
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif
