/**
  ******************************************************************************
  * @file           : i2c.h
  * project			: EE329 Lab A9
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/21/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	i2c header for defines and function prototypes
  *
  ******************************************************************************
  */

#ifndef INC_I2C_H_
#define INC_I2C_H_

/* Created defines and function prototypes -----------------------------------*/
#include "delay.h"
#define EEPROM_ADDRESS 0x54
#define EEPROM_MEMORY_ADDR 0xf7f5

void I2C_Init (void);
uint8_t I2C_Read (uint8_t devAddr, uint16_t addr);
void I2C_Write (uint8_t devAddr, uint16_t addr, uint8_t data);

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

#endif /* INC_I2C_H_ */
