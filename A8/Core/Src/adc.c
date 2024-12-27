/**
  ******************************************************************************
  * @file           : adc.c
  * project			: EE329 Lab A8
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/28/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Functions for ADC. Initializes ADC1 at PA0 (channel 5). Interrupt
  *	flag samples for 20 samples, global array must be included in main along
  *	with global sample count. Once array fulfilled ADC stops sampling.
  *
  ******************************************************************************
  */

#include "adc.h"
#include "delay.h"


void ADC_init(void) {
RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;         // turn on clock for ADC
// power up & calibrate ADC
ADC123_COMMON->CCR |= (1 << ADC_CCR_CKMODE_Pos); // clock source = HCLK/1
ADC1->CR &= ~(ADC_CR_DEEPPWD);             // disable deep-power-down
ADC1->CR |= (ADC_CR_ADVREGEN);             // enable V regulator - see RM 18.4.6
delay_us(20);                              // wait 20us for ADC to power up
ADC1->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_5);    // PA0=ADC1_IN5, single-ended
ADC1->CR &= ~(ADC_CR_ADEN | ADC_CR_ADCALDIF); // disable ADC, single-end calib
ADC1->CR |= ADC_CR_ADCAL;                  // start calibration
while (ADC1->CR & ADC_CR_ADCAL) {;}        // wait for calib to finish
// enable ADC
ADC1->ISR |= (ADC_ISR_ADRDY);              // set to clr ADC Ready flag
ADC1->CR |= ADC_CR_ADEN;                   // enable ADC
while(!(ADC1->ISR & ADC_ISR_ADRDY)) {;}    // wait for ADC Ready flag
ADC1->ISR |= (ADC_ISR_ADRDY);              // set to clr ADC Ready flag
// configure ADC sampling & sequencing
ADC1->SQR1  |= (5 << ADC_SQR1_SQ1_Pos);    // sequence = 1 conv., ch 5
ADC1->SMPR1 |= (0x7 << ADC_SMPR1_SMP5_Pos);  // ch 5 sample time = 640.5 clocks
ADC1->CFGR  &= ~( ADC_CFGR_CONT  |         // single conversion mode
                  ADC_CFGR_EXTEN |         // h/w trig disabled for s/w trig
                  ADC_CFGR_RES   );        // 12-bit resolution
// configure & enable ADC interrupt
ADC1->IER |= ADC_IER_EOCIE;                // enable end-of-conv interrupt
ADC1->ISR |= ADC_ISR_EOC;                  // set to clear EOC flag
NVIC->ISER[0] = (1<<(ADC1_2_IRQn & 0x1F)); // enable ADC interrupt service
__enable_irq();                            // enable global interrupts
// configure GPIO pin PA0
RCC->AHB2ENR  |= (RCC_AHB2ENR_GPIOAEN);    // connect clock to GPIOA
GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL0); 			// clear alt. function select
GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL0_Pos); 	// choose AF 7 (PA0=ADC1_IN5)
GPIOA->MODER  |= (GPIO_MODER_MODE0);       // analog mode for PA0 (set MODER last)
ADC1->CR |= ADC_CR_ADSTART;
}


void ADC1_2_IRQHandler(void){
	if(ADC1->ISR & ADC_ISR_EOC){
		uint16_t adcData = ADC1->DR;
		if(ADC_SampleCount <= 19){						//if array not full
			ADC_Samples [ADC_SampleCount] = adcData;	//fill array
			ADC_SampleCount++;							//incriment to next sample
			ADC1->CR |= ADC_CR_ADSTART;					//start next conv
		}
		else {
			ADC_SampleCount = 0;						//if array full reset index
			ADC1->ISR &= ~(ADC_ISR_EOC);				//clear flag
		}
	}
}
