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
#include "use_adc.h"
#include "use_uart.h"
#include "delay.h"
#include <stdio.h>


#define ADC1_DR_Address    ((uint32_t)0x4001244C)

static __IO u32 ADC1ConvertedValue;

/*******************************************************************************
	*��ʼ��DMAͨ��
	*DMA1 Channel1 DMA2 Channel15
********************************************************************************/
static void ADC_DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

  	/* DMA1 channel1 configuration ----------------------------------------------*/
  	DMA_DeInit(DMA1_Channel1);	//��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
  	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			//�ò������Զ���DMA�������ַ
 	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue;	//�ò������Զ���DMA�ڴ����ַ
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA_DIR�涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ  DMA_DIR_PeripheralSRC ������Ϊ���ݴ������Դ	 DMA_DIR_PeripheralDST ������Ϊ���ݴ����Ŀ�ĵ�
  	DMA_InitStructure.DMA_BufferSize = 1;	//����ָ��DMAͨ����DMA����Ĵ�С����λΪ���ݵ�λ�����ݴ��䷽�����ݵ�λ���ڽṹ�в���DMA_PeripheralDataSize���߲���DMA_MemoryDataSize��ֵ��
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//�����趨�����ַ�Ĵ����������
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;				//�����趨�ڴ��ַ�Ĵ���������� DMA_MemoryInc_Disable
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//�趨���������ݿ��  �˴�����Ϊ16λ
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//�趨���������ݿ�� �˴�Ϊ16λ
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//������DMA�Ĺ���ģʽ DMA_Mode_Circular ������ѭ������ģʽ DMA_Mode_Normal ��������������ģʽ
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//�趨DMAͨ��x��������ȼ�	  ��Ϊ�����ȼ�
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//ʹ��DMAͨ�����ڴ浽�ڴ洫�� �˴�ΪDMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  	DMA_Init(DMA1_Channel1, &DMA_InitStructure);			//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��x�Ĵ��� ͨ��11 
  	/* Enable DMA1 channel1 */
  	DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��ָ����ͨ��x

}

/*******************************************************************************
	*��ʼ��ADCͨ��
	*ADC1 Channel14	 ADC2 Channel13 ADC3 Channel12
********************************************************************************/
static void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	/* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADC�����ڶ�������˫ADCģʽ ADC1��ADC2�����ڶ���ģʽ
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//�涨��ģ��ת��������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ�����������������ΪENABLE����DISABLE��
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//�涨��ģ��ת���������������ǵ���ģʽ
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//������ʹ���ⲿ��������������ͨ����ģ��ת�� ����Ϊת��������������ⲿ������

  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//�涨��ADC��������߶��뻹�����ұ߶���  �˴�ΪADC�����Ҷ���
  	ADC_InitStructure.ADC_NbrOfChannel = 1;						//�涨��˳����й���ת����ADCͨ������Ŀ
  	ADC_Init(ADC1, &ADC_InitStructure);							//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
  	/* ADC1 regular channels configuration */ 
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5); //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��   
  
  	/* Enable ADC1 DMA */
  	ADC_DMACmd(ADC1, ENABLE);		//ʹ�ܻ���ʧ��ָ����ADC��DMA����

	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */   
  	ADC_ResetCalibration(ADC1);
  	/* Check the end of ADC1 reset calibration register */
  	while(ADC_GetResetCalibrationStatus(ADC1));

  	/* Start ADC1 calibaration */
  	ADC_StartCalibration(ADC1);
  	/* Check the end of ADC1 calibration */
  	while(ADC_GetCalibrationStatus(ADC1));
     
  	/* Start ADC1 Software Conversion */ 
  	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}


static void ADC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	//PC5 ��Ϊģ��ͨ��10��������                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE );

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

}

void User_ADC_Configuration(void)
{
	ADC_GPIO_Configuration();
	ADC_DMA_Configuration();
	ADC_Configuration();

}

u32 get_ADC1ConvertedValue(void)
{
	u8 i;
	u32 adc_temp = 0;
	//do some thing, such as filter
	for(i=0; i<100; i++)
	{
		adc_temp += ADC1ConvertedValue;
	//	printf("the adc_temp = %d\r", adc_temp);
		delay_us(10);
	}
	adc_temp = adc_temp / 100;

	#ifdef DEBUG_OUT
	printf("the adc value = %d\r", adc_temp);
	#endif

	return	adc_temp; 
}

