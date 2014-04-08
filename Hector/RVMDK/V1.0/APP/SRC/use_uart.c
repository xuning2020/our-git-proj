/**
  ******************************************************************************
  * @author  haitao zhou
  * @version V3.5.0
  * @date    2014/3/9
  * @brief   uart for printf	
  ******************************************************************************
  * @attention
  *
  *	THE PRESENT FIRMWARE WHICH IS FOR HECTOR PROJECT 
  * <h2><center>&copy; COPYRIGHT 2014 haitao zhou</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "use_uart.h"
#include "use_pwm.h"
#include <stdio.h>

#ifdef 	PRINTF_USE_UART1
	#define USART_PRINTF USART1
	#define GPIO_PRINTF_TX  GPIO_Pin_9	   //PA.09
	#define GPIO_PRINTF_RX  GPIO_Pin_10	   //PA.10
#else
	#define USART_PRINTF USART2
	#define GPIO_PRINTF_TX  GPIO_Pin_2	   //PA.02
	#define GPIO_PRINTF_RX  GPIO_Pin_3	   //PA.03
#endif

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
 	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART_PRINTF, (uint8_t) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART_PRINTF, USART_FLAG_TC) == RESET)
  {}

  return ch;
} 


/* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
 */
static void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	/*
	USART_ClockInitTypeDef USART_ClockInitStructure; 

	USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
  	USART_ClockInitStructure.USART_CPOL = USART_CPOL_High;
  	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
  	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
  	USART_ClockInit(USART_PRINTF, &USART_ClockInitStructure);
	*/
	USART_InitStructure.USART_BaudRate = 115200;
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART_PRINTF, &USART_InitStructure);
	USART_ITConfig(USART_PRINTF, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART_PRINTF, ENABLE);
}

static void USART_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	#ifdef 	PRINTF_USE_UART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	#else
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	#endif

	/* Configure USARTx Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_PRINTF_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USARTx Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_PRINTF_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void USART_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  	/* Enable the USARTy Interrupt */
	#ifdef 	PRINTF_USE_UART1
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	#else
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	#endif
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}


void User_USART_Configuration(void)
{
	USART_GPIO_Configuration();
	USART_NVIC_Configuration();
	USART_Configuration();
}
