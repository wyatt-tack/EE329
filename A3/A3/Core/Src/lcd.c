/**
  ******************************************************************************
  * @file           : lcd.c
  * project			: EE329 Lab A3
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/1/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Functions for interfacing and communicating to LCD display through
  *	nibble mode. Provided on behalf of EE329 lab manual. Configured to
  *	be wired through GPIO PORT A Pin0-3 as D4-D7; Pin4 as RS; Pin5 as EN
  *
  ******************************************************************************
  */

#include "lcd.h"

// ------------------------------------------------------ excerpt from lcd.c ---
void LCD_init( void )  {
	  //Port clock initialize
	  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOAEN);
	  //LCD pin initialize - Push Pull, no PU/PD, high speed
	  LCD_PORT->MODER   &= ~(LCD_MODER);
	  LCD_PORT->MODER   |=  (LCD_MODER_0);
	  LCD_PORT->OTYPER  &= ~(LCD_OTYPER);
	  LCD_PORT->PUPDR   &= ~(LCD_PUPDR);
	  LCD_PORT->OSPEEDR |=  (LCD_OSPEEDR);
	delay_us( 80000 );                    	// power-up wait 80 ms
	LCD_PORT->ODR   &= ~(LCD_RS);			// clear RS bit
	for ( int idx = 0; idx < 3; idx++ ) {  // wake up 1,2,3: DATA = 0011 XXXX
      LCD_4b_command( 0x30 );// HI 4b of 8b cmd, low nibble = X
      delay_us( 5000 );
   }
   LCD_4b_command( 0x20 ); // fcn set #4: 4b cmd set 4b mode - next 0x28:2-line
   delay_us(3000);
   LCD_command( 0x28 );	// fcn set 4b mode and 2x28 line
   delay_us( 300 );
   LCD_command( 0x10 );	//Set cursor
   delay_us( 300 );
   LCD_command( 0x0F ); // display, cursor, blink on
   delay_us( 300 );
   LCD_command(0x06);	//Entry mode
   delay_us( 300 );
   LCD_command(0x01); 	//clear display
   delay_us( 300 );
   LCD_command(0x80);	// set cursor home
   delay_us( 300 );

}

void LCD_pulse_ENA( void )  {
// ENAble line sends command on falling edge
// set to restore default then clear to trigger
   LCD_PORT->ODR   |= ( LCD_EN );         	// ENABLE = HI
   delay_us( 25 );                         // TDDR > 320 ns
   LCD_PORT->ODR   &= ~( LCD_EN );        // ENABLE = LOW
   delay_us( 20 );                         // low values flakey, see A3:p.1
}

void LCD_4b_command( uint8_t command )  {
// LCD command using high nibble only - used for 'wake-up' 0x30 commands
   LCD_PORT->ODR   &= ~( LCD_DATA_BITS ); 	// clear DATA bits
   LCD_PORT->ODR   |= ( command >> 4 );   // DATA = command
   delay_us( 15 );
   LCD_pulse_ENA( );
}

void LCD_command( uint8_t command )  {
// send command to LCD in 4-bit instruction mode
// HIGH nibble then LOW nibble, timing sensitive
   LCD_PORT->ODR   &= ~( LCD_DATA_BITS );               // isolate cmd bits
   LCD_PORT->ODR   |= ( (command>>4) & LCD_DATA_BITS ); // HIGH shifted low
   delay_us( 15 );
   LCD_pulse_ENA( );                                    // latch HIGH NIBBLE

   LCD_PORT->ODR   &= ~( LCD_DATA_BITS );               // isolate cmd bits
   LCD_PORT->ODR   |= ( command & LCD_DATA_BITS );      // LOW nibble
   delay_us( 15 );
   LCD_pulse_ENA( );                                    // latch LOW NIBBLE
}

void LCD_write_char( uint8_t letter )  {
// calls LCD_command() w/char data; assumes all ctrl bits set LO in LCD_init()
   LCD_PORT->ODR   |= (LCD_RS);       // RS = HI for data to address
   delay_us( 15 );
   LCD_command( letter );             // character to print
   LCD_PORT->ODR   &= ~(LCD_RS);      // RS = LO
}

void LCD_set_cursor( uint8_t position[2])	{
//	calls LCD_command to change cursor position
	//position formatted as [row,col] (zero indexed)
	//sets ddram address for cursor set
	LCD_PORT->ODR   &= ~(LCD_RS);
	uint8_t ddRamAdd =  (40 * position[1]);
	ddRamAdd |= (0x80);
	LCD_command(ddRamAdd);
	delay_us( 500 );
	for (int col = 0; col < position[0]; col++){
		LCD_command( 0x14 );
		delay_us( 300 );
		}
	//set address as RS_Low, data as [0x80 | address]
	//address defined as 0x00-0x0F, 0x40-0x4F
}

void LCD_write_string( uint8_t 	writeData[])	{
//	calls LCD_write_char in row long for loop
	for (uint8_t indexCol = 0; indexCol < 16; indexCol++ ){
			LCD_write_char(writeData[indexCol]);
			delay_us(60);
		}
	}

