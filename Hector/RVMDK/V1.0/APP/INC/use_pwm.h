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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USE_PWM_H
#define __USE_PWM_H

#ifdef __cplusplus		  //��ֹC++�﷨��c��ͻ
 extern "C" {
#endif

#include "globle.h"
#include <stdbool.h>


void User_PWM_Configuration(void);
void PWM_Enable(bool enable);
void PWM_SetFrequency(u16 Freq);
void PWM_SetDuty(NNType Duty);

#ifdef __cplusplus
}
#endif

#endif
