/**
  ******************************************************************************
  * @file           : main.c
  * project			: EE329 Lab A7
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 10/12/2024
  * firmware		: ST-Link V1
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Device uses UART and a VT100 terminal to act as a very simple game. WASD
  *	keys are used as control, and taking character off screen places it as if
  *	screen scrolled over. Uses a baud rate of 115200
  *
  ******************************************************************************
  */

#include "main.h"
#include "delay.h"
#include "uart.h"

volatile uint16_t cursor[2] ={0,0};

int main(void)
{
	//Initialize clock, keypad, spi config
	HAL_Init();
	SystemClock_Config();
	SysTick_Init();
	UART_Init();
	//homescreen lines and commands
	char *homescreen[] = {
			"\e[2J",		//clear screen
			"\e[10;30H",		//set cursor in middle
			"\e[1m",		//set cursor bold
			"\e[31m",		//set font red
			"EE329 ADVENTURE",	//title
			NULL};
	//execute homescreen text and bubbles graphic
	Execute(homescreen);
	Bubbles();
	//screen boarder for game
	char *gamescreen[] = {
				"\e[2J",		//clear screen
				"\e[H",
				"\e[36m",		//set font cyan
				"____________________________________________________________\e[2;0H"
				"|                                                          |\e[3;0H"
				"|                                                          |\e[4;0H"
				"|                                                          |\e[5;0H"
				"|                                                          |\e[6;0H"
				"|                                                          |\e[7;0H"
				"|                                                          |\e[8;0H"
				"|                                                          |\e[9;0H"
				"|                                                          |\e[10;0H"
				"|                                                          |\e[11;0H"
				"|                                                          |\e[12;0H"
				"|                                                          |\e[13;0H"
				"|                                                          |\e[14;0H"
				"|                                                          |\e[15;0H"
				"|                                                          |\e[16;0H"
				"|                                                          |\e[17;0H"
				"|                                                          |\e[18;0H"
				"|                                                          |\e[19;0H"
				"|__________________________________________________________|"
				"\e[10;30H",		//set cursor in middle
				"\e[1m",		//set cursor bold
				"\e[35m",		//set font purple
				"&",			//character
				NULL};
	Execute(gamescreen);
	cursor[0] = 10; //set cursor
	cursor[1] =  30;
	//boundaries and character



	while (1){
	//if collision detected, repair wall and send character to other side
		if (cursor[0] < 2) {
			LPUART_Print("\e[36m\e[D_\e[D");
			LPUART_Print("\e[17B");
			LPUART_Print("\e[1m\e[35m&");	//print char
			cursor[0] = 18;
		}
		if (cursor[0] > 18) {
			LPUART_Print("\e[36m\e[D_\e[D");
			LPUART_Print("\e[17A");
			LPUART_Print("\e[1m\e[35m&");	//print char
			cursor[0] = 2;
		}
		if (cursor[1] < 2) {
			LPUART_Print("\e[36m\e[D|\e[D");
			LPUART_Print("\e[58C");
			LPUART_Print("\e[1m\e[35m&");	//print char
			cursor[1] = 59;
		}
		if (cursor[1] > 59) {
			LPUART_Print("\e[36m\e[D|\e[D");
			LPUART_Print("\e[58D");
			LPUART_Print("\e[1m\e[35m&");	//print char
			cursor[1] = 2;
		}
	}
}
//------------------------- Movement in IRQ Handler -------------------------------
void LPUART1_IRQHandler( void  ) {
   uint8_t charRecv;
   if (LPUART1->ISR & USART_ISR_RXNE) {
      charRecv = LPUART1->RDR;
      switch ( charRecv ) {
	  case 'w':
		  LPUART_Print("\e[D \e[D");
		  LPUART_Print("\e[A"); //move up
		  LPUART_Print("\e[1m\e[35m&");	//print char
		  cursor[0] = cursor[0]-1; //move cursor
		  break;
	  case 'a':
		  LPUART_Print("\e[D \e[D");
		  LPUART_Print("\e[D"); //move left
		  LPUART_Print("\e[1m\e[35m&");	//print char
		  cursor[1] = cursor[1]-1; //move cursor
		  break;
	  case 's':
		  LPUART_Print("\e[D \e[D");
		  LPUART_Print("\e[B");	//move down
		  LPUART_Print("\e[1m\e[35m&");	//print char
		  cursor[0] = cursor[0]+1; //move cursor
		  break;
	  case 'd':
		  LPUART_Print("\e[D \e[D");
		  LPUART_Print("\e[C"); //move right
		  LPUART_Print("\e[1m\e[35m&");	//print char
		  cursor[1] = cursor[1]+1; //move cursor
		  break;
	   default:
		//LPUART_Print_Char(charRecv);  // echo char to terminal
		break;
      }
   }
}

//---------------------------------------------------------------------------
void Execute(const char *messages[]){
//executes all messages in script
	for(uint8_t idx = 0; messages[idx] != NULL; idx++){
	LPUART_Print(messages[idx]);
	}
}
void Bubbles(void){
	//Time index for home screen, bubble locations colors and time stamps

	char *bubble[] = {".","o","0","O","()","( )","*"," "};
	int timex = 0;
	int bSeed[] = {1, 3, 8, 12, 16, 18, 22, 27};
	char *sSeed[] = {"\e[11;47H\e[32m", "\e[12;25H\e[36m",  "\e[7;45H\e[35m", "\e[8;22H\e[33m",
					"\e[8;49H\e[34m",	"\e[7;25H\e[35m",  "\e[12;44H\e[36m",  "\e[13;35H\e[32m"};
	//print bubbles around middle of screen for 5 seconds
	for(int timer = 0; timer < 40; timer++){
		for (int bubblex = 0; bubblex < 8; bubblex++){
		if(timex > bSeed[bubblex] && timex < bSeed[bubblex]+8){
			LPUART_Print(sSeed[bubblex]);
			LPUART_Print(bubble[timex-bSeed[bubblex]]);
			LPUART_Print("  ");
			LPUART_Print("\e[H");
		}
		}
			timex++;
			delay_us(125000);
	}
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

