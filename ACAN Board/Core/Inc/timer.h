/* -----------------------------------------------------------------------------
* function : Timers
* INs      :
* OUTs     :
* action   : sets up 3 timers for 500Hz, 100Hz, and 1Hz
* author   : Mohummad Elgassier (ME)
* version  :
* date     : 241204
* -------------------------------------------------------------------------- */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_


#include "stm32g4xx_hal.h"


void TIM2_Init(void);
void TIM3_Init(void);
extern volatile uint8_t tim2flag;
extern volatile uint8_t tim3flag;
extern volatile uint8_t tim4flag;


#endif /* INC_TIMER_H_ */
