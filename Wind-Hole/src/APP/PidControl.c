#include "PidControl.h"


void PidInit(PidObject* pid, const float desired, const float KP,const float KI,const float KD, int Max, int Min)
{
	pid->error     = 0;
	pid->prevError = 0;
	pid->integ     = 0;
	pid->deriv     = 0;
	pid->desired = desired;
	pid->kp = KP;
	pid->ki = KI;
	pid->kd = KD;
	pid->iLimit = DEFAULT_PID_INTEGRATION_LIMIT; //默认pid的积分限幅
	pid->MaxValue = Max;
	pid->MinValue = Min;
		
}

int error_limit;

int pidUpdate(PidObject* pid, const float error)
{
	int output;

	pid->error = error;   
	if(pid->desired>=650 &&pid->desired<=850) 
		error_limit = 750;
	else if(pid->desired>850)
		error_limit = 400;
	else if(pid->desired>450 && pid->desired <550)
		error_limit = 550;
	else if(pid->desired<450 &&pid->desired>0)
		error_limit = 450;
	else if(pid->desired<750 &&pid->desired>550)
		error_limit = 510;
	
	if(pid->error < error_limit)  //限制积分运算范围	
	  pid->integ += pid->error;
	
	
	//积分限幅
	if (pid->integ > pid->iLimit)
	{
		pid->integ = pid->iLimit;
	}
	else if (pid->integ < -pid->iLimit)
	{
		pid->integ = -pid->iLimit;
	}

	pid->deriv = pid->error - pid->prevError;


	output = pid->kp * pid->error + pid->ki * pid->integ +  pid->kd * pid->deriv;
	
	//输出限幅
	output = OutLimit(pid, output);
	
	pid->prevError = pid->error;

	pid->out = output;
	return output;
}


int PidIncrease(PidObject* pid, const float error)
{
	static int pid_out, last_error, lastlast_error, deriv ,Increase;
	
	
	deriv = error - 2*last_error + lastlast_error;
	
	Increase = pid->kp*(error - last_error) + pid->ki*error + pid->kd * deriv;
	
	pid_out += Increase;

	pid_out = OutLimit(pid, pid_out);
	
	lastlast_error = last_error;

	last_error = error;

	return pid_out;
}

int OutLimit(PidObject* pid, int OutValue)
{
	if (OutValue > pid->MaxValue)
		OutValue = pid->MaxValue;
	if (OutValue < pid->MinValue)
	 	OutValue = pid->MinValue;

	return OutValue;
}