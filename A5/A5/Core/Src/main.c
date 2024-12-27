/**
  ******************************************************************************
  * @file           : main.c
  * project			: EE329 Lab A5
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/12/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Device uses spi to control an external DAC. Keypad at port D is used to
  *	control SPI1 line at GPIO port E. Typing into keypad produces output
  *	entered on DAC.
  *
  ******************************************************************************
  */

#include "main.h"
#include "spi.h"
#include "delay.h"
#include "keypad.h"


int main(void)
{
	//Initialize clock, keypad, spi config
	HAL_Init();
	SystemClock_Config();
	SysTick_Init();
	Keypad_Config();
	DAC_init();
	uint16_t mvolt, volt, mvolt100, mvolt10;

	while (1){
	//poll for mv digits or reset
	volt = Key_Poll();
	if (volt == 0xA) continue;
	mvolt100 = Key_Poll();
	if (mvolt100 == 0xA) continue;
	mvolt10 = Key_Poll();
	if (mvolt10 == 0xA) continue;
	//set up total mV count and write
	mvolt = (volt*1000) + (mvolt100*100) + (mvolt10*10);
	DAC_write(mvolt);
	}
}



//---------------------------------------------------------------------------

void DAC_init(void){
	SPI_init();
}

void DAC_write(int mvolts){
	uint16_t data = DAC_volt_conv(mvolts);
	SPI1->DR = data;
	while (!(SPI1->SR & SPI_SR_TXE));
	while (SPI1->SR & SPI_SR_BSY);

}
int DAC_volt_conv(int mvolts){
	//determines if mvolts requested is above/below 1/2 threshold
	mvolts = mvolts*10000/9981 + 1; //trendline from calibration
	if(mvolts < 2048){
		uint16_t data = (mvolts * 4095)/2048;
		return (data | (0x3 << 12));
	}
	else if(mvolts <= 3300 ){
		uint16_t data = (mvolts * 4095)/(2*2048);
		return (data | (0x1 << 12));
	}
	else{
		return (0x1FFF);
	}

}

//---------------------------------------------------------------------------

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

