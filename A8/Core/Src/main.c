/**
  ******************************************************************************
  * @file           : main.c
  * project			: EE329 Lab A8
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 11/6/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Device uses ADC1 at GPIO A0 to measure relay current through
  *	established parameters along with measuring emitter voltage
  *	of the BJT used to drive the circuitry.
  *
  *	PA0	- ADC at emitter of BJT
  *	PC0	- Base driver output
  *	PC13 - Button input to drive software
  *
  *	Wire Relay coil +/- to Flyback diode anode/cathode respectively
  *	Relay coil + to 3.3V, relay - to BJT Collector
  *	BJT base to 2k resistor into PC0
  *	BJT emitter to 10 resistor into gnd
  *	PA0 at BJT emitter
  *
  ******************************************************************************
  */

#include "main.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"

volatile uint16_t ADC_Samples [20];
volatile uint16_t ADC_SampleCount;

int main(void)
{
	//Initialize clock
	HAL_Init();
	SystemClock_Config();
	SysTick_Init();
	UART_Init();
	ADC_init();
	HomeScreen();
	Button_Config();
	Output_Config();
	uint16_t minVolt, maxVolt, avgVolt, current;

	while (1){
		if(!BUTTON_PRESS)		//if pushed
			GPIOC->BSRR |= (GPIO_PIN_0);	//turn on relay
		else
			GPIOC->BRR |= (GPIO_PIN_0);
		ADC1->CR |= ADC_CR_ADSTART; 	//start ADC
		while(ADC_SampleCount <= 19);	//wait for array to fill
		delay_us(500000);
			sendHexData(minVal(ADC_Samples), maxVal(ADC_Samples), avgVal(ADC_Samples));
				minVolt = (minVal(ADC_Samples)*10000 + 30662) / 12349;
				maxVolt = (maxVal(ADC_Samples)*10000 + 30662) / 12349;
				avgVolt = (avgVal(ADC_Samples)*10000 + 30662) / 12349;
			sendVoltData(minVolt, maxVolt, avgVolt);
				current = (1000*avgVolt*1000/RESISTOR - IBASE)/1000;	//(nV/mOhms - uA) --> mA
			sendCurrentData(current);
	}
}

//--------------------------- Screen ---------------------------------------

void HomeScreen(void){
	char *homescreen[] = {
			"\e[2J",			//clear screen
			"\e[?25l",			//hide cursor
			"\e[3;5H",			//set cursor home
			"\e[33m",			//set text yellow
			"ADC counts volts \e[4;5H",
			"MIN  0000  0.000 V \e[5;5H",
			"MAX  0000  0.000 V \e[6;5H",
			"AVG  0000  0.000 V \e[7;5H",
			"coil current = 0.000 A",
			NULL};
	for(uint8_t idx = 0; homescreen[idx] != NULL; idx++)
		LPUART_Print(homescreen[idx]);
}


void sendHexData(uint16_t minVal, uint16_t maxVal, uint16_t avgVal){
	parseSendHex(minVal, 1);
	parseSendHex(maxVal, 2);
	parseSendHex(avgVal, 3);
}
void sendVoltData(uint16_t minVal, uint16_t maxVal, uint16_t avgVal){
	parseSendVolt(minVal, 1);
	parseSendVolt(maxVal, 2);
	parseSendVolt(avgVal, 3);
}
void sendCurrentData(uint16_t value){
	uint8_t valTho = (value/1000);
	uint8_t valHun = (value - valTho*1000)/100;
	uint8_t valTen = (value - valTho*1000 - valHun*100)/10;
	uint8_t valOne = (value - valTho*1000 - valHun*100 - valTen*10);
	LPUART_Print("\e[7;20H");
	LPUART_Print_Char(valTho+0x30);		//send A.BDC
		LPUART_Print("\e[C");			//skip period
	LPUART_Print_Char(valHun+0x30);
	LPUART_Print_Char(valTen+0x30);
	LPUART_Print_Char(valOne+0x30);
}

void parseSendHex(uint16_t value, uint8_t row){
	uint8_t valTho = (value/1000);
	uint8_t valHun = (value - valTho*1000)/100;
	uint8_t valTen = (value - valTho*1000 - valHun*100)/10;
	uint8_t valOne = (value - valTho*1000 - valHun*100 - valTen*10);
	switch (row){						//pick max/min/avg row
	case 1:
		LPUART_Print("\e[4;10H");
	break;
	case 2:
		LPUART_Print("\e[5;10H");
	break;
	case 3:
		LPUART_Print("\e[6;10H");
	break;
	}
	LPUART_Print_Char(valTho+0x30);		//send ABCD
	LPUART_Print_Char(valHun+0x30);
	LPUART_Print_Char(valTen+0x30);
	LPUART_Print_Char(valOne+0x30);
}

void parseSendVolt(uint16_t value, uint8_t row){
	uint8_t valTho = (value/1000);
	uint8_t valHun = (value - valTho*1000)/100;
	uint8_t valTen = (value - valTho*1000 - valHun*100)/10;
	uint8_t valOne = (value - valTho*1000 - valHun*100 - valTen*10);
	switch (row){						//pick max/min/avg row
	case 1:
		LPUART_Print("\e[4;16H");
	break;
	case 2:
		LPUART_Print("\e[5;16H");
	break;
	case 3:
		LPUART_Print("\e[6;16H");
	break;
	}
	LPUART_Print_Char(valTho+0x30);		//send A.BDC
		LPUART_Print("\e[C");			//skip period
	LPUART_Print_Char(valHun+0x30);
	LPUART_Print_Char(valTen+0x30);
	LPUART_Print_Char(valOne+0x30);
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

void Output_Config(void)
{		// configure GPIO pin PB14 for:
	  	// output mode, push-pull, pull down, high speed
  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN);
  GPIOC->MODER   &= ~(GPIO_MODER_MODE0);
  GPIOC->MODER   |=  (GPIO_MODER_MODE0_0);
  GPIOC->OTYPER  &= ~(GPIO_OTYPER_OT0);
  GPIOC->PUPDR   &= ~(GPIO_PUPDR_PUPD0);
  GPIOC->PUPDR   |= (GPIO_PUPDR_PUPD0_1);
  GPIOC->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED0_Pos);
  GPIOC->BRR |= (GPIO_PIN_0);
}

void Button_Config(void)
{		// configure GPIO pin PC13 for:
	  	// Button pin initialize - Input, on board PU
	RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN);
	BUTTON_PORT->MODER   &= ~(GPIO_MODER_MODE13);
	BUTTON_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD13);
}

//--------------------------- MATH -----------------------------------------
uint16_t maxVal (uint16_t* nums ){
	uint16_t max = nums[0];
	for (int index = 0; index < sizeof(nums)/sizeof(nums[0]); index++)
		 if (nums[index] > max)
		     max = nums[index];
	return max;
}

uint16_t minVal (uint16_t* nums ){
	uint16_t min = nums[0];
	for (int index = 0; index < sizeof(nums)/sizeof(nums[0]); index++)
		 if (nums[index] < min)
		     min = nums[index];
	return min;
}

uint16_t avgVal (uint16_t* nums ){
	uint16_t avg = 0;
	uint16_t arraySize = sizeof(nums)/sizeof(nums[0]);
	for (int index = 0; index < arraySize; index++)
		avg = avg + nums[index];
	avg = avg/arraySize;
	return avg;
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

