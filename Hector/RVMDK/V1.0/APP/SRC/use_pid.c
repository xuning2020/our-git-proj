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
#include "use_pid.h"
#include "use_uart.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define	ABS(n)	((n>0)?(n):(-n))

bool needPid = false;
Error error = {0};
PID_PARAMS pid_params = {DEFAULT_KP, DEFAULT_KI, DEFAULT_KD};

void init_PID(void)
{
	memset(&error, 0, sizeof(Error));
	memset(&pid_params, 0, sizeof(PID_PARAMS));
}

void WakeupPid(bool is_enable)
{
	needPid = is_enable;
}

void set_PID_Params(NNType KP, NNType KI, NNType KD)
{
	pid_params.Kp = KP;
	pid_params.Ki = KI;
	pid_params.Kd = KD; 
}

#ifdef BP_NET
NNType PIDCalculate(NNType Kp, NNType Ki, NNType Kd)
{
	NNType iInPid;
	if (needPid)
		iInPid = Kp * (error.current - error.last) + Ki * error.current + Kd * (error.current - 2*error.last + error.prev);
	else 
		return FAULT;
	error.prev = error.last;
	error.last = error.current;

	return iInPid;
}
#else

#ifdef LOCATION_PID
NNType PIDCalculate(Error *error, u32 feedback)
{
	NNType iInPid;

	error->current =  targetValue - feedback;
	error->total += error->current;

	if(ABS(error->current) < PID_ERROR_MIN)
	{
		WakeupPid(false);
		return 0;
	}else{
		WakeupPid(true);
	}

	if (needPid)
	{
		s32 error_temp = error->current - error->last;  //减少浮点型计算
		iInPid = pid_params.Kp * error->current + pid_params.Ki * error->total + pid_params.Kd * error_temp;
	}
	if(iInPid > PID_OUT_MAX)
		iInPid = PID_OUT_MAX;
	else if(iInPid < PID_OUT_MIN)
		iInPid = PID_OUT_MIN;

	#ifdef DEBUG_OUT
	printf("PIDCalculate: error.current=%d, error.last=%d, error.total=%d\n\r", error->current,  error->last, error->total);
	printf("PIDCalculate:iInPid=%0.2lf\n\r", iInPid);
	#endif

	error->last = error->current;

	return iInPid;
}
#else
NNType PIDCalculate(Error *error, u32 feedback)
{
	NNType iInPid;

	error->current =  targetValue - feedback;

	if(ABS(error->current) < PID_ERROR_MIN)
	{
		WakeupPid(false);
		return 0;
	}else{
		WakeupPid(true);
	}

	if (needPid)
	{
	
		s32 temp_error_1 = error->current - error->last;	 //减少浮点运算
		s32 temp_error_2 = error->current - 2*error->last + error->prev;
		iInPid = pid_params.Kp * temp_error_1 + pid_params.Ki * error->current + pid_params.Kd * temp_error_2;
	
	  	//iInPid = pid_params.Kp*(2.45*error->current - 3.5*error->last + 1.25*error->prev);  //Z-N整定，增量PID简化

		//iInPid = pid_params.Kp * error->current - pid_params.Ki * error->last + pid_params.Kd * error->prev;
	}
	#ifdef DEBUG_OUT
	printf("PIDCalculate: error.current=%d, error.last=%d, error.prev=%d\n\r", error->current,  error->last, error->prev);
	printf("PIDCalculate:iInPid=%0.2lf\n\r", iInPid);
	#endif

	if(iInPid > PID_OUT_MAX)
		iInPid = PID_OUT_MAX;
	else if(iInPid < PID_OUT_MIN)
		iInPid = PID_OUT_MIN;

	error->prev = error->last;
	error->last = error->current;

	return iInPid;
}
#endif

#endif
