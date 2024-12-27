/**
  ******************************************************************************
  * @file           : keypad.h
  * project			: EE329 Lab A2
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 9/27/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for keypad.h
  *
  ******************************************************************************
  */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_
#include "stm32l4xx_hal.h"

//-------------------------------- KEYPAD Defines ------------------------------
#define COL_PORT GPIOD
#define COL_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2)
#define ROW_PORT GPIOD
#define ROW_PINS (GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6)
#define SETTLE 1900 //defined as time for 20ms loop instruction (*5.5us)

#define BIT0_COL GPIO_PIN_0 //defined as first bit for columns
#define BIT0_ROW GPIO_PIN_3 //defined as first bit for rows
#define NUM_COLS 3
#define NUM_ROWS 4
#define NO_KEYPRESS 0x0
#define KEY_ZERO 11
#define CODE_ZERO 0xF

//--------------------------------- function prototypes ------------------------
void Keypad_Config(void);
int Keypad_IsAnyKeyPressed(void);
int Keypad_WhichKeyIsPressed(void);


#endif /* INC_KEYPAD_H_ */
