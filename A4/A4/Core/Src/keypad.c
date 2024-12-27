/**
  ******************************************************************************
  * @file           : keypad.c
  * project			: EE329 Lab A3
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/7/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Keypad pulling function source file provided on behalf of the EE329 lab
  *	manual. Adapted from EE329 lab manual. Currently attached to GPIO PORT D
  *
  ******************************************************************************
  */
#include "keypad.h"

// ------------------------------------- modified from excerpt from keypad.c ---
void Keypad_Config(void){//must be manually changed if separate GPIO port is used
											   //set for port D as current config
	  //Port clock initialize
	  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIODEN);
	  //Column pin initialize - Push Pull, no PU/PD, high speed
	  COL_PORT->MODER   &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1
			   	   	   		| GPIO_MODER_MODE2);
	  COL_PORT->MODER   |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0
	 	   	   				| GPIO_MODER_MODE2_0);
	  COL_PORT->OTYPER  &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1
			  	  	  	| GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
	  COL_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1
			  	  	  	| GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	  COL_PORT->OSPEEDR |=  ((GPIO_OSPEEDR_OSPEED0) |
	                      (GPIO_OSPEEDR_OSPEED1) |
	                      (GPIO_OSPEEDR_OSPEED2));
	  //Row pin initialize - Input, pull down
	  ROW_PORT->MODER   &= ~(GPIO_MODER_MODE3 | GPIO_MODER_MODE4
	  			   	   	 | GPIO_MODER_MODE5 | GPIO_MODER_MODE6);
	  ROW_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD3 | GPIO_PUPDR_PUPD4
	  			  	  	| GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6);
	  ROW_PORT->PUPDR   |= (GPIO_PUPDR_PUPD3_1 | GPIO_PUPDR_PUPD4_1
	  			  	  	| GPIO_PUPDR_PUPD5_1 | GPIO_PUPDR_PUPD6_1);
}

// -----------------------------------------------------------------------------
int Keypad_IsAnyKeyPressed(void) {
// drive all COLUMNS HI; see if any ROWS are HI
// return true if a key is pressed, false if not
   COL_PORT->BSRR = COL_PINS;         	      // set all columns HI
   for ( uint16_t idx=0; idx<SETTLE; idx++ )   	// let it settle
      ;
   if ((ROW_PORT->IDR & ROW_PINS) != 0 ) {	// got a keypress!
	   for ( uint16_t idx=0; idx < SETTLE; idx++ ){
		   if ((ROW_PORT->IDR & ROW_PINS) == 0 ) return( 0 );
	   	   }				// if key held for 20ms then return 1 (debounce)
	   return( 1 );
   	   }
   else
      return( 0 );                          // nope.
}

// -----------------------------------------------------------------------------
int Keypad_WhichKeyIsPressed(void) {
// detect and encode a pressed key at {row,col}
// assumes a previous call to Keypad_IsAnyKeyPressed() returned TRUE
// verifies the Keypad_IsAnyKeyPressed() result (no debounce here),
// determines which key is pressed and returns the encoded key ID

   int8_t iRow=0, iCol=0, iKey=0;  // keypad row & col index, key ID result
   int8_t bGotKey = 0;             // bool for keypress, 0 = no press

   COL_PORT->BSRR = COL_PINS;                       	 // set all columns HI
   for ( iRow = 0; iRow < NUM_ROWS; iRow++ ) {      	 // check all ROWS
      if ( ROW_PORT->IDR & (BIT0_ROW << iRow) ) {      	 // keypress in iRow!!
         COL_PORT->BRR = ( COL_PINS );            	 // set all cols LO
         for ( iCol = 0; iCol < NUM_COLS; iCol++ ) {   // 1 col at a time
            COL_PORT->BSRR = ( BIT0_COL << (iCol) );     // set this col HI
            if ( ROW_PORT->IDR & (BIT0_ROW << iRow) ) {    // keypress in iCol!!
               bGotKey = 1;
               break;                                  // exit for iCol loop
            }
         }
         if ( bGotKey )
            break;
      }
   }
   //	encode {iRow,iCol} into LED word : row 1-3 : numeric, ‘1’-’9’
   //	                                   row 4   : ‘*’=10, ‘0’=15, ‘#’=12
   //                                    no press: send NO_KEYPRESS
   if ( bGotKey ) {
      iKey = ( iRow * NUM_COLS ) + iCol + 1;  // handle numeric keys ...
 	if ( iKey == KEY_ZERO )                 //    works for ‘*’, ‘#’ too
         iKey = CODE_ZERO;
 	return( iKey );                         // return encoded keypress
   }
   return( NO_KEYPRESS );                     // unable to verify keypress
}

// -----------------------------------------------------------------------------
uint8_t Key_Poll(void) {
	// polls key and returns value once key is inputed and let go
	uint8_t currentKeyValue;
	while(!Keypad_IsAnyKeyPressed());
		  if (Keypad_WhichKeyIsPressed() > 0 && Keypad_WhichKeyIsPressed() < 16){
			  currentKeyValue = Keypad_WhichKeyIsPressed();
			  if (currentKeyValue == 0xF ) currentKeyValue = 0; //zero position
		  }
		  else
			  currentKeyValue = (0xF);		//error flag
     while(Keypad_IsAnyKeyPressed());
		  return currentKeyValue;
		  //wait till key is let go
}

