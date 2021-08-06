
#include "sys_time.h"

uint16_t time_count_20us=0;//20us ��������
uint16_t time_count_ms=0;//ms��������
uint16_t time_count_s=0;
uint16_t time_count_min=0;
uint16_t time_count_h=0;
/**
��ʱ����ʼ��
Ϊ����Ӧ���ṩϵͳʱ��
ʹ��
**/
void sys_time_init(uint32_t interrupt_time_us )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4ʱ��
	uint16_t per,psc;
	psc=72-1;
	per=interrupt_time_us;
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn ;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
}



/**
20us����
**/
void count_time_per_20us(void)
{
	time_count_20us++;
	if(time_count_20us>=50)
	{
		time_count_20us=0;
		time_count_ms++;
		if(time_count_ms>=1000)
		{
			// time_count_ms=0;
			// time_count_s++;
			// if(time_count_s>=60)
			// {
			// 	time_count_s=0;
			// 	time_count_min++;
			// 	if(time_count_min>=60)
			// 	{
			// 		time_count_min=0;
			// 		time_count_h++;
			// 		if(time_count_h>=24)time_count_h=0;
			// 	}
			// }
		}
	}
}

/**
5ms����
**/
void count_time_per_5ms(void)
{
	time_count_ms+=5;
	if(time_count_ms>=1000)
	{
		time_count_ms=0;
		time_count_s++;
		if(time_count_s>=60)
		{
			time_count_s=0;
			time_count_min++;
			if(time_count_min>=60)
			{
				time_count_min=0;
				time_count_h++;
				if(time_count_h>=24)time_count_h=0;
			}
		}
	}
}

uint32_t get_sys_time_us(void)
{
	uint32_t this_time=0;
	if(time_count_20us)
	{
		this_time+=time_count_20us*20;
	}
	if(time_count_ms)
	{
		this_time+=time_count_ms*1000;
	}
	if(time_count_s)
	{
		this_time+=time_count_s*1000000;
	}
	if(time_count_min)
	{
		this_time+=time_count_min*60*1000000;
	}
	if(time_count_h)
	{
		this_time+=time_count_h*60*60*1000000;
	}
	return this_time;
}

uint32_t get_sys_time_ms(void)
{
	 uint32_t this_time=0;
	 if(time_count_ms)
	 {
		 this_time+=time_count_ms;
	 }
	 if(time_count_s)
	 {
		  this_time+=time_count_s*1000;
	 }
	 if(time_count_min)
	 {
		 this_time+=time_count_min*60*1000;
	 }
	 if(time_count_h)
	 {
		 this_time+=time_count_h*60*60*1000;
	 }
	 return this_time;
}

uint32_t get_sys_time_s(void)
{
	uint32_t this_time=0;
	if(time_count_ms>500)
	{
		this_time+=1;
	}
	if(time_count_s)
	{
		this_time+=time_count_s;
	}
	if(time_count_min)
	{
		this_time+=time_count_min*60;
	}
	if(time_count_h)
	{
		this_time+=time_count_h*60*60;
	}
	return this_time;
}

uint32_t get_sys_time_min(void)
{
	uint32_t this_time=0;
	if(time_count_s>30)
	{
		this_time+=1;
	}
	if(time_count_min)
	{
		this_time+=time_count_min;
	}
	if(time_count_h)
	{
		this_time+=time_count_h*60;
	}
	return this_time;
}

uint32_t get_sys_time_h(void)
{
	uint32_t this_time=0;
	if(time_count_min>30)
	{
		this_time+=1;
	}
	if(time_count_h)
	{
		this_time+=time_count_h;
	}
	return this_time;
}

void TIM4_IRQHandler(void) //TIM4_IRQHandler(void) 20us�ж�
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
		count_time_per_20us();
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}

