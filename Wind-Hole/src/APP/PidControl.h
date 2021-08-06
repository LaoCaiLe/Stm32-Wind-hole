#ifndef __PID_CONTROL_H
#define __PID_CONTROL_H
#include <stdbool.h>
#define DEFAULT_PID_INTEGRATION_LIMIT 		20000.0//Ĭ��pid�Ļ����޷�
#define DEFAULT_PID_OUTPUT_LIMIT      		0.0	  //Ĭ��pid����޷���0Ϊ���޷�

typedef struct
{
	float desired;		//< set point
	float error;        //< error
	float prevError;    //< previous error
	int integ;        //< integral
	float deriv;        //< derivative
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
	float iLimit;       //< integral limit
	float MaxValue;
	float MinValue;
	float out;			//< out
} PidObject;

void PidInit(PidObject* pid, const float desired, const float KP,const float KI,const float KD,int Max, int Min);
int pidUpdate(PidObject* pid, const float error);
int PidIncrease(PidObject* pid, const float error);
int OutLimit(PidObject* pid, int OutValue);

#endif

