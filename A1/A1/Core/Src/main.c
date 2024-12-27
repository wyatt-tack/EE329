/**
  ******************************************************************************
  * @file           : main.c
  * project			: EE329 Lab A1
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 9/27/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Program lights up 4 LEDs connected to GPIO PTC0-3 in a binary count from 0
  *	to 15, 3 consecutive counts. Program proceeds to go to infinite loop of
  *	turning PTC0 on, calling a function, turning PTC1 on, executing a command,
  *	turning PTC1 off, returning, to turning PTC0 off. This program is used to
  *	determine delay of passing data types into function, as well as calculation
  *	time for various operations and data types.
  *
  ******************************************************************************
  */
#include "main.h"
#include <math.h>


void SystemClock_Config(void);
typedef int64_t var_type;					//definition for var_type
var_type TestFunction(var_type num);

void main(void)  {

	var_type main_var;

	HAL_Init();
	SystemClock_Config();

  // configure GPIO pins PC0-3 for:
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
		  	  | GPIO_PIN_2 | GPIO_PIN_3); // preset PC0-3 to 0


  	 // Loop 15 count 3 times
for (int smallLoop = 0; smallLoop < 3; smallLoop++){
	for(int loopCount = 0; loopCount <= 16; loopCount++){
		//if(main_var >= 15) main_var = 0;
		//main_var++;  // added to eliminate not used warning
	GPIOC->ODR = (loopCount);
	for(uint32_t delaycnt = 0; delaycnt < 40000; delaycnt++);
	}
	}

  while (1)   {   // infinite loop to avoid program exit
	  // time the test function call using PC0
	  GPIOC->BSRR = (GPIO_PIN_0);             // turn on PC0
	  main_var = TestFunction(15);            // call test function
	  GPIOC->BRR = (GPIO_PIN_0);              // turn off PC0

  }

}

var_type TestFunction(var_type num) {
  var_type test_var;  				// local variable
  GPIOC->BSRR = (GPIO_PIN_1);             // turn on PC1
//  	  test_var = num; 				      // instruction to test
//  	test_var = num + 1;
//  	test_var = num * 3;
//  	test_var = num / 3;
  	test_var = num * num;
//  	test_var = num % 10;
//  	test_var = pow(num, 3);
//  	test_var = sqrt(num);
//  	test_var = sin(num);

  GPIOC->BRR = (GPIO_PIN_1);              // turn off PC1
  return test_var;
}

//Below is system configurations
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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
