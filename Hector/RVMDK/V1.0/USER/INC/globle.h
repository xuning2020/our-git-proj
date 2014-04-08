#ifndef __GLOBLE_H
#define __GLOBLE_H

//#define DEBUG_OUT  //open log out for debug, release please mask it

#define USE_FLOAT  //use type

#ifdef	USE_LONG_DOUBLE
#define	NNType long double
#endif

#ifdef	USE_DOUBLE
#define	NNType	double
#endif

#ifdef	USE_FLOAT
#define	NNType	float
#endif

#define FAULT 0xFFFFFFFF	
#define DEFAULT_PWM 0

#endif
