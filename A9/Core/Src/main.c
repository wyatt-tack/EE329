/**
  ******************************************************************************
  * @file           : main.c
  * project			: EE329 Lab A9
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/21/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Device uses I2C to write to an EEPROM connected at:
  *	 	PTB-8: SCL
  *		PTB-9: SDA
  * The device writes a byte to an address, then reads the byte at that address.
  * If data read is the same as data written, then the on board LED will turn on,
  * else the LED will turn off.
  ******************************************************************************
  */

#include "main.h"
#include "delay.h"
#include "i2c.h"



int main(void)
{
	//Initialize clock, I2C config
	HAL_Init();
	SystemClock_Config();
	SysTick_Init();
	I2C_Init();
	Led_Config();
	uint8_t rngData;
	uint16_t rngAddr;
	while (1){
		rngData = 0x15;		//8 bit data
		rngAddr = 0x1515; 		//16 bit address
		LED_PORT->BRR |= (GPIO_PIN_14);	//reset light
		I2C_Write (EEPROM_ADDRESS, rngAddr, rngData);	//write
		delay_us(5000);									//wait for data set
		if (I2C_Read (EEPROM_ADDRESS, rngAddr) == rngData)
			LED_PORT->BSRR |= (GPIO_PIN_14);	//if data set, set LED
		delay_us(10000);				//delay between samples
	}
}

//--------------------------- GPIO -----------------------------------------
void Led_Config(void)
{		// configure GPIO pin PB14 for:
	  	// output mode, push-pull, no pull up or pull down, high speed
  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOBEN);
  LED_PORT->MODER   &= ~(GPIO_MODER_MODE14);
  LED_PORT->MODER   |=  (GPIO_MODER_MODE14_0);
  LED_PORT->OTYPER  &= ~(GPIO_OTYPER_OT14);
  LED_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD14);
  LED_PORT->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED14_Pos);
  LED_PORT->BRR |= (GPIO_PIN_14);
}

//----------------------------- System --------------------------------------
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

