/**
  ******************************************************************************
  * @file           : lcd.h
  * project			: EE329 Lab A3
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/1/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for lcd.h
  *
  ******************************************************************************
  */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "delay.h"
#include "stm32l4xx_hal.h"

#define LCD_MODER (0x03FFF)
#define LCD_MODER_0 (0x01555)
#define LCD_OTYPER (0x07F)
#define LCD_PUPDR (0x03FFF)
#define LCD_OSPEEDR (0x03FFF)
#define LCD_PORT GPIOA
#define LCD_EN GPIO_PIN_5	//Pin 5
#define LCD_RS GPIO_PIN_4	//Pin 4
#define LCD_DATA_BITS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
							//Pins 11-14
							//pull down R/W (set as only outputs)
void LCD_init( void );
void LCD_pulse_ENA( void );
void LCD_4b_command( uint8_t command );
void LCD_command( uint8_t command );
void LCD_write_char( uint8_t letter );
void LCD_set_cursor( uint8_t position[2]);
void LCD_write_string( uint8_t writeData[] );

#endif /* INC_LCD_H_ */
