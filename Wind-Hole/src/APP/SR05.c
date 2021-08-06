#include "SR05.h"

u8 i=0;
double s=0;
double finals=0;
double alls=0;
u32 indata=0;

void SR05_Init(void)
	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  

}
	
void SR05_start(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
}

double get_distance(void)
{
	alls=0;
	s=0;

	for(i=0;i<5;i++)
	{
		SR05_start();
		//OLED_ShowString(0,6,"cap success",12);
		if(TIM2_CH1_CAPTURE_STA&0x80) 
		{
			indata=TIM2_CH1_CAPTURE_STA&0x3f;
			indata*=0xffff; 
			indata+=TIM2_CH1_CAPTURE_VAL;
			s=(float)indata*34/2000;  //
			TIM2_CH1_CAPTURE_STA=0; 
		}	
		alls=alls+s;
	}		
	finals=(float)alls/5;		
	return finals;
}