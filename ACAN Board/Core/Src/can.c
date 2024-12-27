	/**
  ******************************************************************************
  * @file           : can.c
  * project			: Analog Can Board
  * author			: Wyatt Tack (wwt) - wtack@calpoly.edu
  * date			: 11/13/2024
  * firmware		: J-Link JTAG
  * @attention		: Copyright (c) 2024 STMicroelectronics. All rights reserved.
  ******************************************************************************
  *
  *	Code uses HAL to initialize CAN FD perephreal on STM32G4 using pins:
  *	PA-11 --> Can Rx
  *	PA-12 --> Can Tx
  *
  *	Code was built off reference examples from:
  *	HAL:
  *		https://github.com/STMicroelectronics/STM32CubeG4/blob/master/Projects/STM32G474E-EVAL/Examples/FDCAN/FDCAN_Classic_Frame_Networking/Src/main.c
  *
  *	Bare Metal:
  *		https://stackoverflow.com/questions/76688974/stm32-fdcan-communication-how-do-you-set-up-fdcan-using-low-level-programming-o
  *
  *	A very large attempt at getting bare metal to work was taken, but
  *	due to time constraint and lack of information we had to settle for HAL
  ******************************************************************************
  */
#include "can.h"
//-----------  Can Perephreal Register Setup  ----------------
/*
 * Sets GPIO at alternate function 9 for PTA-11 and PTA-12
 */
void can_init(void){
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	//--- GPIO STUFF -------------------------------------------
	GPIOA->MODER   &= ~(GPIO_MODER_MODE12 | GPIO_MODER_MODE11);
	GPIOA->MODER   |=  (GPIO_MODER_MODE12_1 | GPIO_MODER_MODE11_1);
	GPIOA->OTYPER  &= ~(GPIO_OTYPER_OT12 | GPIO_OTYPER_OT11);
	GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD12 | GPIO_PUPDR_PUPD11);
	GPIOA->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED12_Pos) | (3 << GPIO_OSPEEDR_OSPEED11_Pos));
	GPIOA->AFR[1] &= ~((0xF << GPIO_AFRH_AFSEL12_Pos) | (0xF << GPIO_AFRH_AFSEL11_Pos));
	GPIOA->AFR[1] |=  ((0x9 << GPIO_AFRH_AFSEL12_Pos) | (0x9 << GPIO_AFRH_AFSEL11_Pos));

}

 //-----------------  HAL and System Code  ------------------
 /*	Code for initializing CAN Registers. For HAL Message
  * transfer, an hfdcan structure instance has to be made,
  * which has to be set up in HAL. The registers that are
  * mapped to each HAL structure data point are commented below
  * Code adapted from HAL code imported from STM32CubeIDE
  * and example credited in header
  */

void FDCAN1_Init(void)
{

	hfdcan1.Instance = FDCAN1;
		//FDCAN1 -> CCCR 	&= ~(FDCAN_CCCR_INIT);			//Power Cycle initialization
		//FDCAN1 -> CCCR 	|= 	(FDCAN_CCCR_INIT);
		//while ((FDCAN1->CCCR & FDCAN_CCCR_INIT) == 0U);	//Wait for power cycle
		//FDCAN1 -> CCCR |= FDCAN_CCCR_CCE;	//allows for register change when CCE is high
	  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
	  	  //RCC->APB1ENR1 |= (RCC_APB1ENR1_FDCANEN) |(RCC_APB1ENR1_PWREN) ;
	  	  //RCC->CCIPR |= 	(RCC_CCIPR_FDCANSEL_1);
	  hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
	  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
	  hfdcan1.Init.AutoRetransmission = ENABLE;		//if message lost retransmit
	  hfdcan1.Init.TransmitPause = ENABLE;			//
	  hfdcan1.Init.ProtocolException = DISABLE;		//
	  hfdcan1.Init.NominalPrescaler = 1;			//FDCAN1 -> CCCR = 0x5103;
	  hfdcan1.Init.NominalSyncJumpWidth = 16;		//FDCAN1 -> NBTP = 0x1e003e0f;
	  hfdcan1.Init.NominalTimeSeg1 =63;				//FDCAN1 -> RXGFC = 0x10000;
	  hfdcan1.Init.NominalTimeSeg2 = 16;			//FDCAN1->DBTP =  0x00000433;
	  hfdcan1.Init.DataPrescaler = 1;				//FDCAN1 -> PSR = 0x70f;
	  hfdcan1.Init.DataSyncJumpWidth = 4;
	  hfdcan1.Init.DataTimeSeg1 = 5;
	  hfdcan1.Init.DataTimeSeg2 = 4;
	  hfdcan1.Init.StdFiltersNbr = 1;
	  hfdcan1.Init.ExtFiltersNbr = 0;
	  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	  	  //Error handlers for sending data to registers
	  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	  	{
		  Error_Handler();
	  	}
	  	  //once FDCAN verified, stop initializing transmit
	  	  //FDCAN1 -> CCCR &= ~(FDCAN_CCCR_CCE);
}

//-----------------  HAL TX Sending Code  ------------------

void FDCAN_SendMessage(uint16_t identifier, uint32_t TxData)
 {
	/* 	Code for sending CAN Message and packing message structure
	 * 	Assumes instance of hfdcan is initialized
	 * 	Code adapted from HAL code and example credited in header
	 */
	//FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = identifier;  	// 11-bit identifier (standard CAN frame)
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_2; 	// 2 byte data per message
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;		// remove bit reversal for endianess
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;		// structure for CAN2.0 compatibility
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;
    HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, &TxData);	// add data to RAM FIFO
    delay_us(100);
 }






