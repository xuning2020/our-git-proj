/**
  ******************************************************************************
  * @author  haitao zhou
  * @version V3.5.0
  * @date    2014/3/9
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  *	THE PRESENT FIRMWARE WHICH IS FOR HECTOR PROJECT 
  * <h2><center>&copy; COPYRIGHT 2014 haitao zhou</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define	ABS(n)	((n>0)?(n):(-n))

u32 targetValue;
NNType outputValue = DEFAULT_PWM;

void System_Config(void)
{
	System_RCC_Config();		
	User_USART_Configuration();
	User_ADC_Configuration();
	User_PWM_Configuration();	
}

int my_abs(int n);
NNType adc_to_duty(NNType adc_range);

/* Private functions ---------------------------------------------------------*/
int main(void)
{
	u32 adc_temp;
	targetValue = 2000;

	System_Config();
	init_PID();

	#ifdef DEBUG_OUT
	printf("\r\nsystem config\n\r");
	printf("targetValue=%d\n\r", targetValue);
	#endif 

//	WakeupPid(true);

#if 0 //1 //debug no use
	if (100 < ABS((int)error.current))
	{
		set_PID_Params(0.9, 0.0, 0.0);

		do
		{
			adc_temp = 	get_ADC1ConvertedValue();
			#ifdef 	LOCATION_PID
			outputValue = adc_to_duty(PIDCalculate(&error, adc_temp));
			#else
			outputValue += adc_to_duty(PIDCalculate(&error, adc_temp));
			#endif
			if(outputValue >= 100)
				outputValue = 100;
			else if(outputValue <= 0)
				outputValue = 0;

			PWM_SetDuty(outputValue);
			adc_temp = get_ADC1ConvertedValue();
			error.current = (NNType)targetValue - (NNType)adc_temp;
			printf("%d\n", adc_temp);
			//printf("%d\n", my_abs((int)error.current));
		} while(50 < ABS((int)error.current));
	}
#endif
	//set_PID_Params(DEFAULT_KP, DEFAULT_KI, DEFAULT_KD);
	set_PID_Params(0.02, 0.0, 0.0);

	//error.current = 0;
	//error.last = 0;
	//error.prev = 0;
	printf(" %d, %d, %d\n", error.current, error.last, error.prev);

	while(1)
	{
#if 1
		adc_temp = 	get_ADC1ConvertedValue();
		#ifdef 	LOCATION_PID
			outputValue += PIDCalculate(&error, adc_temp);
		#else
			outputValue += PIDCalculate(&error, adc_temp);
		#endif

		if(outputValue >= PWM_MAX)
			outputValue = PWM_MAX;
		else if(outputValue <= PWM_MIN)
			outputValue = PWM_MIN;

		#ifdef DEBUG_OUT
		printf("outputValue=%0.2lf\n\r", outputValue);
		#endif 

		PWM_SetDuty(outputValue);
		delay_ms(100);
		//delay_ms(1);
	
		printf("%d\n", adc_temp);
	


#else	  //for debug
	PWM_SetDuty(2);
	delay_ms(1000);
	adc_temp = get_ADC1ConvertedValue();
	printf("%d\n", adc_temp);
#endif
	}

	return 0;
}

/*
int my_abs(int n)
{
	if (n < 0)
		return -n;
	return n;

	//return n * ( (n>>31<<1) +1);
}*/

//bool frist_time = true;
NNType adc_to_duty(NNType adc_range)
{
	NNType	adc_range_temp;
	//if(frist_time == true)
	//{
		//frist_time = false;
		adc_range_temp = (adc_range / 32); //100 * adc_range / (3700-500);
		if(adc_range_temp > PID_OUT_MAX)
			adc_range_temp = PID_OUT_MAX;
		else if(adc_range_temp < PID_OUT_MIN)
			adc_range_temp = PID_OUT_MIN;	
		return adc_range_temp; //100 * adc_range / (3700-500);
	//}
	
	//return adc_range;

}
