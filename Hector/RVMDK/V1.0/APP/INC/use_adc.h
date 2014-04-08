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
#ifndef __USE_ADC_H
#define __USE_ADC_H

#ifdef __cplusplus		  //∑¿÷πC++”Ô∑®”Îc≥ÂÕª
 extern "C" {
#endif

#include "globle.h"

void User_ADC_Configuration(void);

//for return ADC value
u32 get_ADC1ConvertedValue(void);

#ifdef __cplusplus
}
#endif

#endif
