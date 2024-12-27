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


#include "adc.h"
#include "delay.h"
#include "main.h"

void ADC_Init(void) {
	// Turn on the clock of ADC 1 and ADC 2
	RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;
	//RCC->CCIPR |= RCC_CCIPR_ADC12SEL_1;
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN);

	/* --------------------------------------------------------------------------
	 * Power and Calibrate ADCs
	 * --------------------------------------------------------------------------
	 */

	// PWR & CALIB. ADC1
	ADC12_COMMON->CCR |= ADC_CCR_CKMODE_0; // clock source = HCLK/1
	ADC1->CR &= ~(ADC_CR_DEEPPWD); // disable deep-power-down
	ADC1->CR |= (ADC_CR_ADVREGEN); // enable V regulator - see RM 18.4.6
	delay_us(20); // wait 2ms for ADC to power up
	ADC1->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_4); // PA0=ADC1_IN5, single-ended
	ADC1->CR &= ~(ADC_CR_ADEN | ADC_CR_ADCALDIF); // disable ADC, single-end calib
	ADC1->CR |= ADC_CR_ADCAL; // start calibration

	while (ADC1->CR & ADC_CR_ADCAL)
		; // wait for calib to finish

	ADC2->CR &= ~(ADC_CR_DEEPPWD); // disable deep-power-down
	ADC2->CR |= (ADC_CR_ADVREGEN); // enable V regulator - see RM 18.4.6
	delay_us(20); // wait 2ms for ADC to power up
	ADC2->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_13 |
							ADC_DIFSEL_DIFSEL_11 |
							ADC_DIFSEL_DIFSEL_4); // PA0=ADC2_IN5, single-ended
	ADC2->CR &= ~(ADC_CR_ADEN | ADC_CR_ADCALDIF); // disable ADC, single-end calib
	ADC2->CR |= ADC_CR_ADCAL; // start calibration

	while (ADC2->CR & ADC_CR_ADCAL)

	/* --------------------------------------------------------------------------
	 * Enable ADCs
	 * --------------------------------------------------------------------------
	 */

	// ENABLE ADC1
	ADC1->ISR |= (ADC_ISR_ADRDY); // set to clr ADC Ready flag
	ADC1->CR |= ADC_CR_ADEN; // enable ADC

	while(!(ADC1->ISR & ADC_ISR_ADRDY)) {;} // wait for ADC Ready flag

	ADC1->ISR |= (ADC_ISR_ADRDY); // set to clr ADC Ready flag

	// ENABLE ADC2
	ADC2->ISR |= (ADC_ISR_ADRDY); // set to clr ADC Ready flag
	ADC2->CR |= ADC_CR_ADEN; // enable ADC

	while(!(ADC2->ISR & ADC_ISR_ADRDY)) {;} // wait for ADC Ready flag

	ADC2->ISR |= (ADC_ISR_ADRDY); // set to clr ADC Ready flag

	/* --------------------------------------------------------------------------
	 * Configure ADCs' Sampling & Sequencing
	 * --------------------------------------------------------------------------
	 */

	// CONFIGURE ADC1 SAMPLING & SEQUENCING
	ADC1->SQR1 |= (4 << ADC_SQR1_SQ1_Pos); // Set Channel 4 as sequence 1
	ADC1->CR &= ~(ADC_CR_ADSTART | ADC_CR_JADSTART);
	ADC1->SMPR1 |= (7 << ADC_SMPR1_SMP4_Pos); // ch 4 sample time = 6.5 clocks
	ADC1->CFGR &= ~( ADC_CFGR_CONT	| // single conversion mode
						  ADC_CFGR_EXTEN 	| // h/w trig disabled for s/w trig
						  ADC_CFGR_RES); // 12-bit resolution

	// CONFIGURE ADC2 SAMPLING & SEQUENCING
	ADC2->SQR1 |= (13 << ADC_SQR1_SQ1_Pos |
						11 << ADC_SQR1_SQ2_Pos |
						4 	<< ADC_SQR1_SQ3_Pos ); // Set Channel 4 as sequence 1
	ADC2->CR &= ~(ADC_CR_ADSTART | ADC_CR_JADSTART);
	ADC2->SMPR2 |= (7 << ADC_SMPR2_SMP13_Pos |
						 7 << ADC_SMPR2_SMP11_Pos ); // ch 4 sample time = 6.5 clocks
	ADC2->SMPR1 |= (7 << ADC_SMPR1_SMP4_Pos );
	ADC2->CFGR &= ~( ADC_CFGR_CONT	| // single conversion mode
						  ADC_CFGR_EXTEN 	| // h/w trig disabled for s/w trig
						  ADC_CFGR_RES); // 12-bit resolution


	/* --------------------------------------------------------------------------
	 * Configure & Enable ADCs' Interrupts
	 * --------------------------------------------------------------------------
	 */

	// ADC1 INTERRUPT CONFIGURE & ENABLE
	ADC1->CR &= ~(ADC_CR_ADSTART | ADC_CR_JADSTART);
	ADC1->IER |= ADC_IER_EOCIE; // enable end-of-conv interrupt
	ADC1->ISR |= ADC_ISR_EOC; // set to clear EOC flag

	// ADC2 INTERRUPT CONFIGURE & ENABLE
	ADC2->CR &= ~(ADC_CR_ADSTART | ADC_CR_JADSTART);
	ADC2->IER |= ADC_IER_EOCIE; // enable end-of-conv interrupt
	ADC2->ISR |= ADC_ISR_EOC; // set to clear EOC flag

	NVIC->ISER[0] = (1<<(ADC1_2_IRQn & 0x1F)); // enable ADC interrupt service
	__enable_irq(); // enable global interrupts

	/* --------------------------------------------------------------------------
	 * Configure GPIO Pins for ADC1 and ADC2
	 * 100Hz ADC1-CH4  - PTA3 (PA3) (40Hz Sallen Key)	1DOF 	0x74b
	 *
	 * 500Hz ADC2-CH13 - PTA5 (PA5) (225Hz Sallen Key)	1D0F	0x773
	 * 500Hz ADC2-CH11 - PTC5 (PC5) (Buffer)			Aero 	0x7f2
	 * 500Hz ADC2-CH4  - PTA7 (PA7) (Buffer)			POT		0x708
	 * --------------------------------------------------------------------------
	 */

	// SETUP ADC1 PIN
	GPIOA->MODER &= ~(GPIO_MODER_MODE3); // clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE3); // analog mode for PA3 (set MODER last)

	// SETUP ADC2 PINS
	// PA7
	GPIOA->MODER &= ~(GPIO_MODER_MODE5); // clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE5); // analog mode for PA5 (set MODER last)
	// PC5
	GPIOC->MODER &= ~(GPIO_MODER_MODE5); // clear mode bits
	GPIOC->MODER |= (GPIO_MODER_MODE5); // analog mode for PC5 (set MODER last)
	// PA7
	GPIOA->MODER &= ~(GPIO_MODER_MODE7); // clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE7); // analog mode for PA7 (set MODER last)

	//ADC1->CR |= ADC_CR_ADSTART; // start 1st conversion
}

void ADC1_2_IRQHandler(void) {
	if (ADC1->ISR & ADC_ISR_EOC) { // Check if conversion is complete
		ADC1->ISR |= ADC_ISR_EOC;  // Clear the interrupt flag
		adc1_flag = 1;
	}
	if (ADC2->ISR & ADC_ISR_EOC) { // Check if conversion is complete
		ADC2->ISR |= ADC_ISR_EOC;  // Clear the interrupt flag
		adc2_flag = 1;
	}
}
