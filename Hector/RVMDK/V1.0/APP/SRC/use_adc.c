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
	*初始化DMA通道
	*DMA1 Channel1 DMA2 Channel15
********************************************************************************/
static void ADC_DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

  	/* DMA1 channel1 configuration ----------------------------------------------*/
  	DMA_DeInit(DMA1_Channel1);	//将DMA的通道x寄存器重设为缺省值
  	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			//该参数用以定义DMA外设基地址
 	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue;	//该参数用以定义DMA内存基地址
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA_DIR规定了外设是作为数据传输的目的地还是来源  DMA_DIR_PeripheralSRC 外设作为数据传输的来源	 DMA_DIR_PeripheralDST 外设作为数据传输的目的地
  	DMA_InitStructure.DMA_BufferSize = 1;	//定义指定DMA通道的DMA缓存的大小，单位为数据单位。根据传输方向，数据单位等于结构中参数DMA_PeripheralDataSize或者参数DMA_MemoryDataSize的值。
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//用来设定外设地址寄存器递增与否
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;				//用来设定内存地址寄存器递增与否 DMA_MemoryInc_Disable
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//设定了外设数据宽度  此处定义为16位
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//设定了外设数据宽度 此处为16位
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//设置了DMA的工作模式 DMA_Mode_Circular 工作在循环缓存模式 DMA_Mode_Normal 工作在正常缓存模式
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//设定DMA通道x的软件优先级	  此为高优先级
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//使能DMA通道的内存到内存传输 此处为DMA通道x没有设置为内存到内存传输
  	DMA_Init(DMA1_Channel1, &DMA_InitStructure);			//根据DMA_InitStruct中指定的参数初始化DMA的通道x寄存器 通道11 
  	/* Enable DMA1 channel1 */
  	DMA_Cmd(DMA1_Channel1, ENABLE); //使能指定的通道x

}

/*******************************************************************************
	*初始化ADC通道
	*ADC1 Channel14	 ADC2 Channel13 ADC3 Channel12
********************************************************************************/
static void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	/* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//设置ADC工作在独立或者双ADC模式 ADC1和ADC2工作在独立模式
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//规定了模数转换工作在扫描模式（多通道）还是单次（单通道）模式。可以设置这个参数为ENABLE或者DISABLE。
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//规定了模数转换工作在连续还是单次模式
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//定义了使用外部触发来启动规则通道的模数转换 本条为转换由软件而不是外部触发启

  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//规定了ADC数据向左边对齐还是向右边对齐  此处为ADC数据右对齐
  	ADC_InitStructure.ADC_NbrOfChannel = 1;						//规定了顺序进行规则转换的ADC通道的数目
  	ADC_Init(ADC1, &ADC_InitStructure);							//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
  	/* ADC1 regular channels configuration */ 
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5); //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间   
  
  	/* Enable ADC1 DMA */
  	ADC_DMACmd(ADC1, ENABLE);		//使能或者失能指定的ADC的DMA请求

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
		
	//PC5 作为模拟通道10输入引脚                         
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

