/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 Fall 2024
 *******************************************************************************
 * @file    : [filename].c
 * @brief   : [Brief description of the purpose of this file]
 * @project : EE 329 F'24 [Assignment Number]
 * @authors : Jacob Larson - jlarso27@calpoly.edu
 * @version : 1.0
 * @date    : [YYYY-MM-DD]
 * @compiler: STM32CubeIDE v.X.X.X (Build: XXXXXX)
 * @target  : NUCLEO-L4A6ZG
 * @clocks  : [Clock Configuration, e.g., 4 MHz MSI to AHB2]
 * @attention: (c) 2024 Cal Poly EE Department. All rights reserved.
 *
 *******************************************************************************
 * DESCRIPTION:
 * [Detailed description of what the module does, any dependencies, etc.]
 *******************************************************************************
 * REVISION HISTORY:
 * 1.0 [YYYY-MM-DD] JL - Initial creation
 *******************************************************************************
 * TODO:
 * - [Optional list of features to add or bugs to fix]
 ******************************************************************************/
/* USER CODE END Header */
#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32g4xx_hal.h"

void ADC_Init(void);
void ADC1_2_IRQHandler(void);

extern  volatile  uint8_t adc1_flag;
extern  volatile  uint8_t adc2_flag;

// ADC1 - 100Hz on PA3 (CH4)
#define ADC1_CHANNEL_4_PIN         GPIO_PIN_3       // Pin number
#define ADC1_CHANNEL_4_PORT        GPIOA            // GPIO Port
#define ADC1_CHANNEL_4_NUMBER      ADC_CHANNEL_4    // ADC Channel

// ADC2 - 500Hz on PA5 (CH13)
#define ADC2_CHANNEL_13_PIN        GPIO_PIN_5       // Pin number
#define ADC2_CHANNEL_13_PORT       GPIOA            // GPIO Port
#define ADC2_CHANNEL_13_NUMBER     ADC_CHANNEL_13   // ADC Channel

// ADC2 - 500Hz on PC5 (CH11)
#define ADC2_CHANNEL_11_PIN        GPIO_PIN_5       // Pin number
#define ADC2_CHANNEL_11_PORT       GPIOC            // GPIO Port
#define ADC2_CHANNEL_11_NUMBER     ADC_CHANNEL_11   // ADC Channel

// ADC2 - 500Hz on PA7 (CH4)
#define ADC2_CHANNEL_4_PIN         GPIO_PIN_7       // Pin number
#define ADC2_CHANNEL_4_PORT        GPIOA            // GPIO Port
#define ADC2_CHANNEL_4_NUMBER      ADC_CHANNEL_4    // ADC Channel


#endif /* INC_ADC_H_ */
