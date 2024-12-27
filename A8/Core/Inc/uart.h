/**
  ******************************************************************************
  * @file           : uart.h
  * project			: EE329 Lab A4
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/13/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	main header for defines for uart.h
  *
  ******************************************************************************
  */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32l4xx_hal.h"

#define BAUD_RATE (8889) //256 * 4MHz /  115.2kb/s = 8888.8

void UART_Init(void);
void LPUART_Print( const char* message );
void LPUART_Print_Char (uint8_t charRecv);


#endif /* INC_UART_H_ */
