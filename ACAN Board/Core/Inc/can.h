/**
  ******************************************************************************
  * @file           : can.h
  * project			: Analog Can Board
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 11/13/2024
  * firmware		: J-Link JTAG
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	CAN header for defines and function prototypes. Meant for STM32G4
  *
  ******************************************************************************
  */

#ifndef INC_CAN_H_
#define INC_CAN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "delay.h"

/* Created defines and function prototypes -----------------------------------*/
// HAL structure definitions for Message structure and CAN perephreal
extern FDCAN_TxHeaderTypeDef TxHeader;
extern FDCAN_HandleTypeDef hfdcan1;
// Function Prototypes
void can_init(void);
void FDCAN_SendMessage(uint16_t identifier, uint32_t TxData);
void FDCAN1_Init(void);

#endif /* INC_CAN_H_ */






