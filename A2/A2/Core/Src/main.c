/**
  ******************************************************************************
  * @file           : main.c
  * project			: EE329 Lab A2
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 9/27/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Takes in input from 3x4 (0-9 *#) keypad, then writes the hex value of the
  *	number to the LED array. LEDs display 0 if glitch, LEDs display 0xF for 0,
  *	0xA for *, and 0xC for #
  *
  *	Keypad defined at GPIOD Col PTC0-2, Row PTC3-6
  *	LEDs defined at GPIOC PTC0-3
  *
  ******************************************************************************
  */

#include "main.h"
#include "keypad.h"
void Led_Config(void);


int main(void)
{

	// Initialize system clock, keypad registers, led registers, and held value
  HAL_Init();
  SystemClock_Config();
  Keypad_Config();
  Led_Config();
  //uint8_t currentKeyValue = 0;
  //GPIOC->BSRR = GPIO_PIN_0;
  while (1)
  {
	  if (Keypad_IsAnyKeyPressed())
	  {

		  if (Keypad_WhichKeyIsPressed() > 0 && Keypad_WhichKeyIsPressed() < 16){
			  //currentKeyValue = Keypad_WhichKeyIsPressed();
			  //GPIOC->ODR = (currentKeyValue);
			  GPIOC->ODR = Keypad_WhichKeyIsPressed();
		  }
		  else
			  GPIOC->ODR = (0x0);

	  }
  }

}
/******************************** ledConfig ********************************/
void Led_Config(void)
{		// configure GPIO pins PC0-3 for:
	  	// output mode, push-pull, no pull up or pull down, high speed
	  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN);
	  GPIOC->MODER   &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1
			   	   	   	| GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	  GPIOC->MODER   |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0
	 	   	   			| GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);
	  GPIOC->OTYPER  &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1
			  	  	  	| GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
	  GPIOC->PUPDR   &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1
			  	  	  	| GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	  GPIOC->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED0_Pos) |
	                      (3 << GPIO_OSPEEDR_OSPEED1_Pos) |
	                      (3 << GPIO_OSPEEDR_OSPEED2_Pos) |
	                      (3 << GPIO_OSPEEDR_OSPEED3_Pos));
	  GPIOC->BRR = (GPIO_PIN_0 | GPIO_PIN_1
			  	  | GPIO_PIN_2 | GPIO_PIN_3);
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
