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
#ifndef __USE_PID_H
#define __USE_PID_H

#ifdef __cplusplus		  //防止C++语法与c冲突
 extern "C" {
#endif

#include "globle.h"
#include "stm32f10x.h"
#include <stdbool.h>

//#define LOCATION_PID  //use location pid

#ifdef LOCATION_PID
#define DEFAULT_KP 0.01625
#define DEFAULT_KI 0.0065
#define DEFAULT_KD 0.01056
#else
#define DEFAULT_KP 0.0
#define DEFAULT_KI 0.0
#define DEFAULT_KD 0.0
#endif

#define PID_OUT_MAX		100		//防止积分饱和
#define	PID_OUT_MIN		-100

#define	PID_ERROR_MIN	20  //允许最小误差，小于该值，不进行PID

#define	PWM_MAX	100	   //定义PWM最大最小输出值
#define	PWM_MIN 0

//for PID
typedef	struct{
		NNType	Kp;
		NNType	Ki;
		NNType	Kd;
}PID_PARAMS;

#ifndef LOCATION_PID
typedef struct{
		s32 current;
		s32 last;
		s32 prev;
	}Error;
#else
typedef struct{
		s32 current;
		s32 last;
		s32 total;
	}Error;
#endif
extern bool needPid;
extern Error error;
extern PID_PARAMS pid_params;

extern u32 targetValue;

void init_PID(void);
void WakeupPid(bool is_enable);
void set_PID_Params(NNType KP, NNType KI, NNType KD);

#ifdef BP_NET
NNType PIDCalculate(NNType Kp, NNType Ki, NNType Kd);
#else
NNType PIDCalculate(Error *error, u32 feedback);
#endif

#ifdef __cplusplus
}
#endif

#endif
