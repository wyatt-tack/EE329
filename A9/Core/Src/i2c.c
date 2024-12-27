	/**
  ******************************************************************************
  * @file           : i2c.c
  * project			: EE329 Lab A9
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/21/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Device uses I2C on GPIO Port B. Meant for an EEPROM data transfer, in an
  *	Device, Add1, Add2, Data. Write format, and a Device, Add1, Add2...
  *	Device, Data format for reading.
  *
  *	PTB-8: SCL
  *	PTB-9: SDA
  *
  ******************************************************************************
  */
#include "i2c.h"

void I2C_Init (void){
//initialize GPIO pins to have AF set to I2C functions
	//Power and Clock
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);   // enable GPIOG clock
	//GPIO Ports - AF8, no PU/PD, open drain, fast
	GPIOB->MODER   &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
	GPIOB->MODER   |=  (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
	GPIOB->OTYPER  |= (GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	GPIOB->PUPDR   &= ~(GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
	GPIOB->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED8_Pos) | (3 << GPIO_OSPEEDR_OSPEED9_Pos));
	GPIOB->AFR[1] &= ~((0x000F << GPIO_AFRH_AFSEL8_Pos) | (0x000F << GPIO_AFRH_AFSEL9_Pos));
	GPIOB->AFR[1] |=  ((0x0004 << GPIO_AFRH_AFSEL8_Pos) | (0x0004 << GPIO_AFRH_AFSEL9_Pos));
//configure I2C functions and specific EEPROM data transactions
	// Configure I2C
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;  // enable I2C bus clock
	I2C1->CR1   &= ~( I2C_CR1_PE );        // put I2C into reset (release SDA, SCL)
	I2C1->CR1   &= ~( I2C_CR1_ANFOFF );    // filters: enable analog
	I2C1->CR1   &= ~( I2C_CR1_DNF );       // filters: disable digital
	I2C1->TIMINGR = 0x00000509;            // 16 MHz SYSCLK timing from CubeMX
	I2C1->CR2   |=  ( I2C_CR2_AUTOEND );   // auto send STOP after transmission
	I2C1->CR2   &= ~( I2C_CR2_ADD10 );     // 7-bit address mode
	I2C1->CR1   |=  ( I2C_CR1_PE );        // enable I2C
	//set address for EEPROM to be continually used
	}

uint8_t I2C_Read (uint8_t devAddr, uint16_t addr){
	//build start write address[2] mode
	I2C1->CR1 	|=  ( I2C_CR1_PE );		//enable I2C
	I2C1->CR2   &= ~( I2C_CR2_SADD );      // clear device address
	I2C1->CR2   |=  ( devAddr << (I2C_CR2_SADD_Pos+1) ); // device addr SHL 1
	I2C1->CR2   &= ~( I2C_CR2_RD_WRN );    // set WRITE mode
	I2C1->CR2   &= ~( I2C_CR2_NBYTES );    // clear Byte count
	I2C1->CR2   |=  ( 2 << I2C_CR2_NBYTES_Pos); // write 2 bytes (2 addr)
	I2C1->CR2   |=    I2C_CR2_START;
	// send data
	while(!(I2C1->ISR & I2C_ISR_TXIS)) ;   	// wait for start condition to transmit
	I2C1->TXDR = (addr >> 8); 				// xmit MSByte of address
	while(!(I2C1->ISR & I2C_ISR_TXE)) ;   	// wait for start condition to transmit
	I2C1->TXDR = (addr & 0xFF);				// xmit LSByte of address
	while(!(I2C1->ISR & I2C_ISR_STOPF));   	// wait for stop condition to transmit
	I2C1->CR1 &= ~( I2C_CR1_PE );		//disable I2C
	delay_us(5);						//wait for 5000ms to cycle power
	I2C1->CR1 |=  ( I2C_CR1_PE );		//enable I2C
	// build start read data[1] mode
	I2C1->CR2   |= ( I2C_CR2_RD_WRN );    // set READ mode
	I2C1->CR2   &= ~( I2C_CR2_SADD );      // clear device address
	I2C1->CR2   |=  ( devAddr << (I2C_CR2_SADD_Pos+1) ); // device addr SHL 1
	I2C1->CR2   &= ~( I2C_CR2_NBYTES );    // clear Byte count
	I2C1->CR2   |=  ( 1 << I2C_CR2_NBYTES_Pos); // read 1 byte
	I2C1->CR2   |=    I2C_CR2_START;
	// read data
	while(!(I2C1->ISR & I2C_ISR_RXNE)) ;		// wait for received data to be copied in
	uint8_t data = I2C1->RXDR;
	while(!(I2C1->ISR & I2C_ISR_STOPF));
	I2C1->CR1 	&= ~( I2C_CR1_PE );		//disable I2C
	return data;
}

void I2C_Write (uint8_t devAddr, uint16_t addr, uint8_t data){
	//build start write address[2], data[1] mode
	I2C1->CR1 	|=  ( I2C_CR1_PE );		//enable I2C
		I2C1->CR2   &= ~( I2C_CR2_SADD );      // clear device address
		I2C1->CR2   |=  ( devAddr << (I2C_CR2_SADD_Pos+1) ); // device addr SHL 1
	I2C1->CR2   &= ~( I2C_CR2_RD_WRN );    	// set WRITE mode
	I2C1->CR2   &= ~( I2C_CR2_NBYTES );    	// clear Byte count
	I2C1->CR2   |=  ( 3 << I2C_CR2_NBYTES_Pos); // write 3 bytes (2 addr, 1 data)
	I2C1->CR2   |=    I2C_CR2_START;	//start data transfer
	//send data
	while(!(I2C1->ISR & I2C_ISR_TXIS)) ;   	// wait for start condition to transmit
	I2C1->TXDR = (addr >> 8); 				// xmit MSByte of address
	while(!(I2C1->ISR & I2C_ISR_TXE)) ;   	// wait for start condition to transmit
	I2C1->TXDR = (addr & 0xFFFF);			// xmit LSByte of address
	while(!(I2C1->ISR & I2C_ISR_TXE)) ;	   	// wait for Txdata to transmit
	I2C1->TXDR = (data);
	while(!(I2C1->ISR & I2C_ISR_TXE)) ; 	// wait for Txdata to transmit
	while(!(I2C1->ISR & I2C_ISR_STOPF));   	// wait for stop condition to transmit
	I2C1->CR1 	&= ~( I2C_CR1_PE );		//disable I2C
}




