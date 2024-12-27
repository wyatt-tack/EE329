/**
  ******************************************************************************
  * @file       : main.c
  * project    	: EE329 Lab A4
  * author		: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date		: 10/9/2024
  * firmware	: ST-Link V1
  * @attention	: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  * Device uses interrupts to act as a reaction timer game. User is prompted to
  * press button, and then press again once the LED lights after a randomly
  * generated set of seconds. Once pressed, interrupts are used to monitor
  * reaction speed.
  * LED defined at GPIOB PTB14 (on board LED3 RED)
  * Button defined at GPIOC PTC13 (on board B1)
  * LCD defined at GPIOA PTA0-3 (4 bit data), PTA4-5 (RS and EN)
  *
  ******************************************************************************
  */

#include "main.h"
#include "lcd.h"
#include "delay.h"

uint32_t usReaction = 0;
uint8_t myflag = 0;

int main(void)
{
	//Initialize clock and delay/LCD configs
	HAL_Init();
	SystemClock_Config();
	SysTick_Init();
	Led_Config();
	Button_Config();
	LCD_init();
	setup_RNG();
	setup_TIM2(0xFFFFFFFF);
	//-------------------------- LCD VARIABLES -----
	//Key Points/openings on LCD
	uint8_t origin[2] = {0,0};
	uint8_t line2[2] = {0,1};
	uint8_t openingLine1[16] = "EE 329 A4 REACT ";
	uint8_t openingLine2[16] = "PUSH SW TO TRIG ";
	uint8_t openingLine3[16] = "     Ready?     ";
	uint8_t openingLine4[16] = "  Too Fast Dude ";
	uint32_t timeTaken = 0;
	uint8_t breakFlag;


	//----------------------- MAIN EXECUTION -----
	while (1){
//Opening Sequence
	myflag = 0;
	breakFlag = 0;
	LED_PORT->BRR |= (GPIO_PIN_14);
	LCD_set_cursor(origin);
	LCD_write_string(openingLine1);
	LCD_set_cursor(line2);
	LCD_write_string(openingLine2);
//Button Pushed, get ready
	Button_Press();
	LCD_set_cursor(line2);
	LCD_write_string(openingLine3);
//Set random timer
	set_TIM2 (((RNG->DR)% 0x1312D00) + 0x7A1200); //Set timer for random 0-8Seconds
	while (myflag == 0){
	    if(!BUTTON_PRESS){		//if pressed too early
	        breakFlag = 1;
	        LCD_set_cursor(line2);
 	        LCD_write_string(openingLine4);
	        while(!BUTTON_PRESS);	//once released break and skip rest of code
	        break;
	    }
	}
	if(!breakFlag){ //end sequence only if button wasn't early
//Random Time Finished
	LED_PORT->BSRR |= (GPIO_PIN_14); //Light On
	usReaction = 0;
	set_TIM2 (999); //Set timer for counting
	while (BUTTON_PRESS) if(usReaction > 10000000) break; //wait for 10S or keypress
	timeTaken = usReaction;	//log reaction time
	while (!BUTTON_PRESS); //wait for button release
	Write_Time(timeTaken);
	}
//Wait for button to replay
	Button_Press();
	}
}

//---------------------------------------------------------------------------
void Write_Time(int timeTaken){
	//Takes in time to press button in uS and parses it to data, then writes
	//results onto LCD screen
	//Parse total time into small times
		uint8_t Sec = timeTaken/1000000;
		uint8_t MSecHun = timeTaken/100000 - 10*Sec;
		uint8_t MSecTen = timeTaken/10000 - 100*Sec - 10*MSecHun;
		uint8_t MSecOne = timeTaken/1000 - 1000*Sec - 100*MSecHun - 10*MSecTen;
	//Write Screen
		uint8_t line2[2] = {0,1};
		uint8_t endLine1[16] = "TIME = N.MMM s  ";
		LCD_set_cursor(line2);
		LCD_write_string(endLine1);
	//Write Data
		uint8_t clockSec[2] = {7,1};
		uint8_t clockMSecHun[2] = {9,1};
		uint8_t clockMSecTen[2] = {10,1};
		uint8_t clockMSecOne[2] = {11,1};
		uint8_t clockEnd[2] = {15,1};
		LCD_set_cursor(clockSec);
		LCD_write_char(Sec + 0x30);
		LCD_set_cursor(clockMSecHun);
		LCD_write_char(MSecHun + 0x30);
		LCD_set_cursor(clockMSecTen);
		LCD_write_char(MSecTen + 0x30);
		LCD_set_cursor(clockMSecOne);
		LCD_write_char(MSecOne + 0x30);
		LCD_set_cursor(clockEnd);
		LCD_write_char(' ');
		uint8_t endLine2[16] = "  Too Slow Dude ";
		if(Sec>=10){
		     LCD_set_cursor(line2);
		     LCD_write_string(endLine2);
		}

}

//-------------------------- ISR -----------------------------------------

void TIM2_IRQHandler(void) {
   if (TIM2->SR & TIM_SR_CC1IF) {      // triggered by CCR1 event ...
      TIM2->SR &= ~(TIM_SR_CC1IF);     // manage the flag
      //myflag = 1;     // Flip GPIO each 400 ticks
      //usReaction = usReaction + 250;
      //TIM2->EGR |= TIM_EGR_UG;
   }

   if (TIM2->SR & TIM_SR_UIF) {        // triggered by ARR event ...
	   TIM2->SR &= ~(TIM_SR_UIF);       // manage the flag
	   usReaction = usReaction + 250;	//incriment count
	   myflag = 1;					//set flag for main
	   myflag = 1;
   }
}

//---------------------------------------------------------------------------
void setup_RNG(void) {
   RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;   // enable clock for RNG
   RCC->CRRCR |= (RCC_CRRCR_HSI48ON);
   RNG->CR |= (RNG_CR_RNGEN | RNG_CR_IE);
   RNG->SR |= (RNG_SR_DRDY);       	// enable RNG
}

void set_TIM2( int period ) {
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
   TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);  // enable event gen, rcv CCR1
   TIM2->ARR = period;                   			// ticks for ARR Period
   TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);       // clr IRQ flag in status reg
   NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // set NVIC interrupt: 0x1F
   __enable_irq();                                 // global IRQ enable
   TIM2->CR1 |= TIM_CR1_CEN;						// start TIM2 CR1

}

void setup_TIM2( int period ) {
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
   TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);  // enable event gen, rcv CCR1
   TIM2->ARR = period;
		   //PERIOD;           		                  // ARR = T = counts @4MHz
   //TIM2->CCR1 = iDutyCycle;                        // ticks for duty cycle
   TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);       // clr IRQ flag in status reg
   NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // set NVIC interrupt: 0x1F
   __enable_irq();                                 // global IRQ enable
   TIM2->CR1 |= TIM_CR1_CEN;						// start TIM2 CR1

}
/*
void setup_MCO_CLK(void) {
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
*/

//--------------------------- GPIO -----------------------------------------

void Button_Press(void){
	while (BUTTON_PRESS)delay_us(100);
	delay_us(100);
	while (!BUTTON_PRESS)delay_us(100);
	delay_us(100);
}

void Button_Config(void)
{		// configure GPIO pin PC13 for:
	  	// Button pin initialize - Input, on board PU
	RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN);
	BUTTON_PORT->MODER   &= ~(GPIO_MODER_MODE13);
	BUTTON_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD13);
}

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

//--------------------------------- SYSTEM -----------------------------------
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


