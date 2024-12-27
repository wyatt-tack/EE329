/**
  ******************************************************************************
  * @file           : lcd.c
  * project			: EE329 Lab A4
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/13/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Use of timer for A4. Uses Tim2 Timer with IRQ handler, connection of
  *	Tim2 to pin GPIOA PTA8, RNG, and setups.
  *
  ******************************************************************************
  */

#include "timer.h"

//-------------------------- ISR -----------------------------------------
void TIM2_IRQHandler(void) {
	// executes after every "event", set up as a finish in ARR or CR1
   if (TIM2->SR & TIM_SR_CC1IF) {      // triggered by CCR1 event ...
      TIM2->SR &= ~(TIM_SR_CC1IF);     // manage the flag
      //flag = 1;     // global flag
      TIM2->EGR |= TIM_EGR_UG;
   }

   if (TIM2->SR & TIM_SR_UIF) {        // triggered by ARR event ...
	   TIM2->SR &= ~(TIM_SR_UIF);       // manage the flag
   }
}


//---------------------------------------------------------------------------
void setup_RNG(void) {
	//initializes and allows for pulling a random 32 bit number from RNG->DR
   RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;   // enable clock for RNG
   RCC->CRRCR |= (RCC_CRRCR_HSI48ON);
   RNG->CR |= (RNG_CR_RNGEN | RNG_CR_IE);
   RNG->SR |= (RNG_SR_DRDY);       	// enable RNG
}

void setup_TIM2( int iDutyCycle ) {
	//sets up Tim2 to count to #define PERIOD, and for CCR1 count as argument
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
   TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);  // enable event gen, rcv CCR1
   TIM2->ARR = PERIOD;                             // ARR = T = counts @4MHz
   TIM2->CCR1 = iDutyCycle;                        // ticks for duty cycle
   TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);       // clr IRQ flag in status reg
   NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // set NVIC interrupt: 0x1F
   __enable_irq();                                 // global IRQ enable
   TIM2->CR1 |= TIM_CR1_CEN;	// start TIM2 CR1
}

void setup_MCO_CLK(void) {
	//Enables PTA-8 AF to be output of TIM2
   // Enable MCO, select MSI (4 MHz source)
   RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_MCOSEL)) | (RCC_CFGR_MCOSEL_0));
   // Configure MCO output on PA8
   RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOAEN);
   GPIOA->MODER   &= ~(GPIO_MODER_MODE8);    	// clear MODER bits
   GPIOA->MODER   |=  (GPIO_MODER_MODE8_1);	// set alternate function mode
   GPIOA->OTYPER  &= ~(GPIO_OTYPER_OT8);     	// Push-pull output
   GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD8);    	// no resistor
   GPIOA->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED8);   // high speed
   GPIOA->AFR[1]  &= ~(GPIO_AFRH_AFSEL8);    	// select MCO function
}
