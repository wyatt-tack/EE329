/**
  ******************************************************************************
  * @file           : main.c
  * project			: Analog Can Board
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 11/13/2024
  * firmware		: J-Link SWD
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Device uses 3 Timers to sample 2 ADCs to send over 1 CAN
  *	Timer2 = 500Hz	--> Sample ADC2_Ch13, ADC2_Ch11, ADC2_Ch4
  *							--> Send Out through FDCAN
  *	Timer3 = 100Hz	--> Sample ADC1_Ch4
  *							--> Send Out through FDCAN
  *	Timer4 = 1Hz	--> Heartbeat LED at PC0
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//CAN structures
FDCAN_HandleTypeDef hfdcan1;
FDCAN_TxHeaderTypeDef TxHeader;
//flags
volatile uint8_t tim2flag;
volatile uint8_t tim3flag;
volatile uint8_t tim4flag;
volatile uint8_t adc1_flag;
volatile uint8_t adc2_flag;

int main(void)
{
	//System and heartbeat LED Config
	HAL_Init();
	SystemClock_Config();
	Led_Config();

  	  // CAN Initialization
  	  can_init();
  	  FDCAN1_Init();

  	  // ADC Initialization
  	  adc1_flag = 0;
  	  adc2_flag = 0;
  	  ADC_Init();


  	  // Timer setup and flag reset
  	  tim2flag = 0;
  	  tim3flag = 0;
  	  tim4flag = 0;
  	  TIM2_Init();	//500 Hz
  	  TIM3_Init();	//100 Hz
  	  TIM4_Init();	//1 Hz


	while (1) {

		if (tim2flag == 1)
		{
			/*	500Hz Timer
			 * 		ADC2-CH13
			 * 		ADC2-CH11
			 * 		ADC2-CH4
			 */
			tim2flag = 0;
			ADC2->SQR1 &= ~ADC_SQR1_SQ1_Msk; // Clear previous channel
			ADC2->SQR1 |= (13 << ADC_SQR1_SQ1_Pos); // Set new channel
			ADC2->CR |= ADC_CR_ADSTART; // convert voltage
			while ((adc2_flag == 0)); // wait for conversion to finish
			adc2_flag = 0;
			uint16_t adc2_ch13_value = ADC2->DR;
			FDCAN_SendMessage(ADC2CH13ID, adc2_ch13_value);

			ADC2->SQR1 &= ~ADC_SQR1_SQ1_Msk; // Clear previous channel
			ADC2->SQR1 |= (11 << ADC_SQR1_SQ1_Pos); // Set new channel
			ADC2->CR |= ADC_CR_ADSTART; // convert voltage
			while ((adc2_flag == 0)); // wait for conversion to finish
			adc2_flag = 0;
			uint16_t adc2_ch11_value = ADC2->DR;
			FDCAN_SendMessage(ADC2CH11ID, adc2_ch11_value);

			ADC2->SQR1 &= ~ADC_SQR1_SQ1_Msk; // Clear previous channel
			ADC2->SQR1 |= (4 << ADC_SQR1_SQ1_Pos); // Set new channel
			ADC2->CR |= ADC_CR_ADSTART; // convert voltage
			while ((adc2_flag == 0)); // wait for conversion to finish
			adc2_flag = 0;
			uint16_t adc2_ch4_value = ADC2->DR;
			FDCAN_SendMessage(ADC2CH4ID, adc2_ch4_value);
		}

		if (tim3flag == 1)
		{
			//	100Hz Timer for ADC1-CH4
			tim3flag = 0;
			ADC1->CR |= ADC_CR_ADSTART; // convert voltage
			while ((adc1_flag == 0)); // wait for conversion to finish
			adc1_flag = 0;
			uint16_t adc1_ch4_value = ADC1->DR;
			FDCAN_SendMessage(ADC1CH4ID, adc1_ch4_value);
		}

		if (tim4flag == 1)
		{
			// Heartbeat LED Timer = 1Hz
			tim4flag = 0;
			GPIOC->ODR ^= (GPIO_PIN_0);
		}

	}
}

//-----------------  Functions  ---------------------------

void Led_Config(void)
{		// configure GPIO pin PB14 for:
	  	// output mode, push-pull, no pull up or pull down, high speed
  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN);
  LED_PORT->MODER   &= ~(GPIO_MODER_MODE0);
  LED_PORT->MODER   |=  (GPIO_MODER_MODE0_0);
  LED_PORT->OTYPER  &= ~(GPIO_OTYPER_OT0);
  LED_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD0);
  LED_PORT->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED0_Pos);
  LED_PORT->BRR |= (GPIO_PIN_0);
}

//-----------------  HAL and System Code  ------------------
/*
 * Imported from STM32CubeIDE
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

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
