/**
  ******************************************************************************
  * @file           : main.c
  * project			: EE329 Lab A3
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/5/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Timer device, uses Keypad to enter numbers for timer, and on pressing #
  *	will count down until timer reached, signaling LED until # is pressed again.
  *	pressing * at any time will go to timer enter mode.
  *	Keypad defined at GPIOD Col PTD0-2, Row PTD3-6
  *	LED defined at GPIOC PTC0
  *	LCD defined at GPIOA PTA0-3 (4 bit data), PTA4-5 (RS and EN)
  *
  ******************************************************************************
  */

#include "main.h"
#include "keypad.h"
#include "lcd.h"
#include "delay.h"


int main(void)
{
	//Initialize clock and delay configs
	HAL_Init();
	SystemClock_Config();
	SysTick_Init();
	//Initialize keypad and LCD
	Keypad_Config();
	LCD_init();
	Led_Config();

//display:
//"EE 329 A3 TIMER "
//"*=SET #=GO 00:00"
	uint8_t origin[2] = {0,0};
	uint8_t line2[2] = {0,1};
	uint8_t openingLine1[16] = "EE 329 A3 TIMER ";
	uint8_t openingLine2[16] = "*=SET #=GO 00:00";
//Initialize variables and cursor locations for each digit
	uint8_t clockMin10[2] = {11,1};
	uint8_t clockMin1[2] = {12,1};
	uint8_t clockSec10[2] = {14,1};
	uint8_t clockSec1[2] = {15,1};
	uint8_t currentKeyValue;
	uint8_t min10, min1;
	uint8_t sec10, sec1;


  while (1)
  {
// reset LED and poll keypad until * pressed
	  currentKeyValue = 0;
		LCD_set_cursor(origin);
		LCD_write_string(openingLine1);
		LCD_set_cursor(line2);
		LCD_write_string(openingLine2);
	  GPIOC->BRR = (GPIO_PIN_0);
	  while(Key_Poll() != STAR_KEY);
// set time for each digit on clock
	  	min10= 0xF; min1= 0xF;	//reset values
	  	sec10= 0xF; sec1= 0xF;
	  //Set 10*min
	  	LCD_set_cursor(clockMin10);
	  	while(min10 < 0 || min10 > 5) {
	  		if (currentKeyValue == STAR_KEY) break;
	  		currentKeyValue = Key_Poll();
	  		min10 = currentKeyValue;
	  	}
	  	LCD_write_char(min10+ 0x30);
	  //Set 1*min
	  	LCD_set_cursor(clockMin1);
	  	while(min1 < 0 || min1 > 9){
	  		if (currentKeyValue == STAR_KEY) break;
	  		currentKeyValue = Key_Poll();
	  		min1 = currentKeyValue;
	  	}
	  	LCD_write_char(min1+ 0x30);
	  //Set 10*sec
	  	LCD_set_cursor(clockSec10);
	  	while(sec10 < 0 || sec10 > 5){
	  		if (currentKeyValue == STAR_KEY) break;
	  		currentKeyValue = Key_Poll();
	  		sec10 = currentKeyValue;
	  	}
	  	LCD_write_char(sec10+ 0x30);
	  //Set 1*sec
	  	LCD_set_cursor(clockSec1);
	  	while(sec1 < 0 || sec1 > 9){
	  		if (currentKeyValue == STAR_KEY) break;
	  		currentKeyValue = Key_Poll();
	  		sec1 = currentKeyValue;
	  	}
	  	LCD_write_char(sec1+ 0x30);
//wait for # start key
	  	while(currentKeyValue != POUND_KEY){
	  		if (currentKeyValue == STAR_KEY) break;
	  		currentKeyValue = Key_Poll();
	  	}
//clock loop
	  	//min 10s
	  	for (min10++; min10>0; min10--){
	  		LCD_set_cursor(clockMin10);
	  		LCD_write_char(min10-1 + 0x30);
	  		//min 1s
	  		for (min1++; min1>0; min1--){
	  			LCD_set_cursor(clockMin1);
	  			LCD_write_char(min1-1 + 0x30);
	  			//sec 10s
	  			for (sec10++; sec10>0; sec10--){
	  				LCD_set_cursor(clockSec10);
	  				LCD_write_char(sec10-1 + 0x30);
	  				//sec 1s
	  				for (sec1++; sec1>0; sec1--){
	  					LCD_set_cursor(clockSec1);
	  					LCD_write_char(sec1-1 + 0x30);
	  					//~1s loop that polls keypad if * pressed
	  					for (uint32_t timeDelay = 0; timeDelay < 1000; timeDelay++){
	  						delay_us(930);	//adjust delay based on calibration
	  						if(currentKeyValue == STAR_KEY) break;
	  						currentKeyValue = Keypad_WhichKeyIsPressed();
	  					}
	  				}//roll overs from 10s place
	  				sec1 = 9;
	  				if(currentKeyValue == STAR_KEY) break;
	  			}
	  			sec10 = 5;
	  			if(currentKeyValue == STAR_KEY) break;
  	  	  	 }
  	  	  	 min1 = 9;
  	  	  	 if(currentKeyValue == STAR_KEY) break;
	  	}
//timer done turn on LED
	  	GPIOC->BSRR |= (GPIO_PIN_0);
//wait till # or * pressed to reset
	  	while (!(currentKeyValue == STAR_KEY || currentKeyValue == POUND_KEY))currentKeyValue = Key_Poll();
  }
}


//---------------------------------------------------------------------------
void Led_Config(void)
{		// configure GPIO pins PC0-3 for:
	  	// output mode, push-pull, no pull up or pull down, high speed
	  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN);
	  GPIOC->MODER   &= ~(GPIO_MODER_MODE0);
	  GPIOC->MODER   |=  (GPIO_MODER_MODE0_0);
	  GPIOC->OTYPER  &= ~(GPIO_OTYPER_OT0);
	  GPIOC->PUPDR   &= ~(GPIO_PUPDR_PUPD0);
	  GPIOC->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED0_Pos);
	  GPIOC->BRR |= (GPIO_PIN_0);
}

// System
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
