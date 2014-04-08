#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "globle.h"
#include "stm32f10x.h"
#include "use_uart.h"
#include "use_adc.h"
#include "use_pwm.h"
#include "use_pid.h"
#include "delay.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>



void System_RCC_Config(void)
{
 	ErrorStatus HSEStartUpStatus;//定义外部高速晶体启动状态枚举变量
 	RCC_DeInit();//复位RCC外部设备寄存器到默认值
 	RCC_HSEConfig(RCC_HSE_ON); //打开外部高速晶振
 	HSEStartUpStatus = RCC_WaitForHSEStartUp();//等待外部高速时钟准备好
 	if(HSEStartUpStatus == SUCCESS)//外部高速时钟已经准别好
    {
   		RCC_HCLKConfig(RCC_SYSCLK_Div1);//配置AHB(HCLK)时钟=SYSCLK
   		RCC_PCLK2Config(RCC_HCLK_Div1); //配置APB2(PCLK2)钟=AHB时钟
   		RCC_PCLK1Config(RCC_HCLK_Div2);//配置APB1(PCLK1)钟=AHB 1/2时钟
      	RCC_ADCCLKConfig(RCC_PCLK2_Div4);//配置ADC时钟=PCLK2 1/4
      
   		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
   		//配置PLL时钟 == 外部高速晶体时钟*9

      	RCC_PLLCmd(ENABLE);//使能PLL时钟
   		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) //等待PLL时钟就绪
       	{
       	}
      	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//配置系统时钟 = PLL时钟
  
      	while(RCC_GetSYSCLKSource() != 0x08) //检查PLL时钟是否作为系统时钟
       	{
       	}
  	}
}

#ifdef __cplusplus
}
#endif

#endif
