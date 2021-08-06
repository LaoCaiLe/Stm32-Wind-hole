#ifndef _PWM_H
#define _PWM_H
 
#include "stm32f10x.h"
#include "stm32f10x_tim.h" 


void TIM3_PWM_Init(u16 per,u16 psc);

void Motor_close(void);
int ABS(int value);

#endif
