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
#include "use_pwm.h"
#include <stdbool.h>
#include <stdio.h>

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

u16 TimerPeriod = 1000;
u16	pwm_div = 72;
u16 Channel1Pulse = DEFAULT_PWM * 10;

static void PWM_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	 /* GPIOA Configuration: Channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  	/* GPIOB Configuration: Channel 1N as alternate function push-pull */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
		
}

static void PWM_TIM_Configuration(void)
{
	
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = pwm_div;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

}

void User_PWM_Configuration(void)
{
	 PWM_GPIO_Configuration();
	 PWM_TIM_Configuration();
}

void PWM_Enable(bool enable)
{
	if(enable == true){
		TIM_Cmd(TIM1, ENABLE);
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
	}else{
		TIM_Cmd(TIM1, DISABLE);
		TIM_CtrlPWMOutputs(TIM1, DISABLE);
	}
		
}

void PWM_SetFrequency(u16 Freq)
{
	if(50 == Freq){
	TIM_PrescalerConfig(TIM1, 72, 1);
	TIM_SetAutoreload(TIM1, 19999);
	}else {
	//TimerPeriod = Freq;  //cal TimerPeriod
	//pwm_div = Freq;	   //cal pwm_div
	//TIM_PrescalerConfig(TIM1, pwm_div, 1);
	//TIM_SetAutoreload(TIM1, TimerPeriod);
	//do cal freq to set ARR and PSC register
	}
}

void PWM_SetDuty(NNType Duty)
{
	u16 temp = (u16)(Duty * 10); //Duty * TimerPeriod / 100;
	TIM_SetCompare1(TIM1, temp);
	#ifdef DEBUG_OUT
	printf("the duty is %0.2lf, and CCR1 is %d\n\r", Duty, temp);
	#endif
}


