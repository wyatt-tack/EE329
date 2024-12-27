/* -----------------------------------------------------------------------------
* function : Timers
* INs      :
* OUTs     :
* action   : sets up 3 timers for 500Hz, 100Hz, and 1Hz
* author   : Mohummad Elgassier (ME)
* version  :
* date     : 241204
* -------------------------------------------------------------------------- */

#include "timer.h"


void TIM2_Init(void) {
	// Enable TIM2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// Set prescaler and auto-reload for 500 Hz
	TIM2->PSC = (170000000 / 1000000) - 1; // Prescaler for 500 kHz
	TIM2->ARR = 999;                      // Auto-reload for 500 Hz
	TIM2->DIER |= TIM_DIER_UIE;           // Enable update interrupt
	TIM2->CR1 |= TIM_CR1_CEN;             // Start timer

	// Enable TIM2 interrupt in NVIC
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 0);
}

void TIM3_Init(void) {
	// Enable TIM3 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;

	// Set prescaler and auto-reload for 100 Hz
	TIM3->PSC = (170000000 / 200000) - 1; // Prescaler for 100 kHz
	TIM3->ARR = 999;                      // Auto-reload for 100 Hz
	TIM3->DIER |= TIM_DIER_UIE;           // Enable update interrupt
	TIM3->CR1 |= TIM_CR1_CEN;             // Start timer

	// Enable TIM3 interrupt in NVIC
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 1);
}
void TIM4_Init(void) {
	// Enable TIM4 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;

	// Set prescaler and auto-reload for 1 Hz
	TIM4->PSC = (170000000 / 1000) - 1; // Prescaler for 1 kHz
	TIM4->ARR = 999;                      // Auto-reload for 1 Hz
	TIM4->DIER |= TIM_DIER_UIE;           // Enable update interrupt
	TIM4->CR1 |= TIM_CR1_CEN;             // Start timer

	// Enable TIM4 interrupt in NVIC
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 1);
}


void TIM2_IRQHandler(void) {
	if (TIM2->SR & TIM_SR_UIF) { // Check update interrupt flag
		TIM2->SR &= ~TIM_SR_UIF; // Clear the interrupt flag

		tim2flag = 1;

	}
}

void TIM3_IRQHandler(void) {
	if (TIM3->SR & TIM_SR_UIF) { // Check update interrupt flag
		TIM3->SR &= ~TIM_SR_UIF; // Clear the interrupt flag

		tim3flag = 1;

	}
}

void TIM4_IRQHandler(void) {
	if (TIM4->SR & TIM_SR_UIF) { // Check update interrupt flag
		TIM4->SR &= ~TIM_SR_UIF; // Clear the interrupt flag

		tim4flag = 1;

	}
}
