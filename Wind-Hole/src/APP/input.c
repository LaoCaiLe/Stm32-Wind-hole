#include "input.h"

u16 TIM2_CH1_CAPTURE_STA; //输入捕获状态 
u16 TIM2_CH1_CAPTURE_VAL;//输入捕获值
/*******************************************************************************
* 函 数 名         : TIM5_CH1_Input_Init
* 函数功能		   : TIM5_CH1输入捕获初始化函数
* 输    入         : arr：自动重装载值
					 psc：预分频系数
* 输    出         : 无
*******************************************************************************/
void TIM2_CH1_Input_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能TIM5时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//管脚设置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //设置下拉输入模式
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //通道1
	TIM_ICInitStructure.TIM_ICFilter=0x00;  //滤波
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//捕获极性
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; //分频系数
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//直接映射到TI1
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
		
	TIM_Cmd(TIM2,ENABLE); //使能定时器
}

void TIMEOPEN()
{
	TIM_SetCounter(TIM2,0);//清除计数t=0
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
}	

void TIMECLOSE()
{
	TIM_Cmd(TIM2,DISABLE); //关闭定时器2
}

/*******************************************************************************
* 函 数 名         : TIM5_IRQHandler
* 函数功能		   : TIM5中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM2_IRQHandler(void)
{
if((TIM2_CH1_CAPTURE_STA&0x80)==0) //还未成功捕获
	{
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)) //发生更新中断
		{
			if(TIM2_CH1_CAPTURE_STA&0X40)//捕获到了高电平
			{
				if((TIM2_CH1_CAPTURE_STA&0x3f)==0x3f) //高电平时间太长
				{
					TIM2_CH1_CAPTURE_STA|=0x80; //标志一次捕获成功
					TIM2_CH1_CAPTURE_VAL=0xffff;
				}
				else
				{
					TIM2_CH1_CAPTURE_STA++;
				}
			}
		}
		if(TIM_GetITStatus(TIM2,TIM_IT_CC1)) //发生捕获中断
		{
			if(TIM2_CH1_CAPTURE_STA&0X40)//捕获到了低电平
			{
				TIM2_CH1_CAPTURE_STA|=0x80; //成功捕获一次高电平
				TIM2_CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //设置上升沿捕获
			}
			else
			{
				TIM2_CH1_CAPTURE_STA=0;
				TIM2_CH1_CAPTURE_VAL=0;
				TIM2_CH1_CAPTURE_STA|=0x40; //捕获到高电平 标志
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0); //定时器初值为0
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling); //设置下降沿捕获
				TIM_Cmd(TIM2,ENABLE);
			}
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1|TIM_IT_Update);
}


