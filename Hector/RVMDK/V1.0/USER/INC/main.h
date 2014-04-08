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
 	ErrorStatus HSEStartUpStatus;//�����ⲿ���پ�������״̬ö�ٱ���
 	RCC_DeInit();//��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
 	RCC_HSEConfig(RCC_HSE_ON); //���ⲿ���پ���
 	HSEStartUpStatus = RCC_WaitForHSEStartUp();//�ȴ��ⲿ����ʱ��׼����
 	if(HSEStartUpStatus == SUCCESS)//�ⲿ����ʱ���Ѿ�׼���
    {
   		RCC_HCLKConfig(RCC_SYSCLK_Div1);//����AHB(HCLK)ʱ��=SYSCLK
   		RCC_PCLK2Config(RCC_HCLK_Div1); //����APB2(PCLK2)��=AHBʱ��
   		RCC_PCLK1Config(RCC_HCLK_Div2);//����APB1(PCLK1)��=AHB 1/2ʱ��
      	RCC_ADCCLKConfig(RCC_PCLK2_Div4);//����ADCʱ��=PCLK2 1/4
      
   		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
   		//����PLLʱ�� == �ⲿ���پ���ʱ��*9

      	RCC_PLLCmd(ENABLE);//ʹ��PLLʱ��
   		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) //�ȴ�PLLʱ�Ӿ���
       	{
       	}
      	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//����ϵͳʱ�� = PLLʱ��
  
      	while(RCC_GetSYSCLKSource() != 0x08) //���PLLʱ���Ƿ���Ϊϵͳʱ��
       	{
       	}
  	}
}

#ifdef __cplusplus
}
#endif

#endif
