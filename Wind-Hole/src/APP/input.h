#ifndef _input_H
#define _input_H

#include "stm32f10x_tim.h"
//#include "sys.h"


extern u16 TIM2_CH1_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM2_CH1_CAPTURE_VAL;//���벶��ֵ

void TIMECLOSE(void);
void TIMEOPEN(void);
void TIM2_CH1_Input_Init(u16 arr,u16 psc);
	
#endif
