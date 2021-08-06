#include "input.h"

u16 TIM2_CH1_CAPTURE_STA; //���벶��״̬ 
u16 TIM2_CH1_CAPTURE_VAL;//���벶��ֵ
/*******************************************************************************
* �� �� ��         : TIM5_CH1_Input_Init
* ��������		   : TIM5_CH1���벶���ʼ������
* ��    ��         : arr���Զ���װ��ֵ
					 psc��Ԥ��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void TIM2_CH1_Input_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ��TIM5ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//�ܽ�����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //������������ģʽ
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //ͨ��1
	TIM_ICInitStructure.TIM_ICFilter=0x00;  //�˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//������
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; //��Ƶϵ��
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ֱ��ӳ�䵽TI1
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
		
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��
}

void TIMEOPEN()
{
	TIM_SetCounter(TIM2,0);//�������t=0
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
}	

void TIMECLOSE()
{
	TIM_Cmd(TIM2,DISABLE); //�رն�ʱ��2
}

/*******************************************************************************
* �� �� ��         : TIM5_IRQHandler
* ��������		   : TIM5�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM2_IRQHandler(void)
{
if((TIM2_CH1_CAPTURE_STA&0x80)==0) //��δ�ɹ�����
	{
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)) //���������ж�
		{
			if(TIM2_CH1_CAPTURE_STA&0X40)//�����˸ߵ�ƽ
			{
				if((TIM2_CH1_CAPTURE_STA&0x3f)==0x3f) //�ߵ�ƽʱ��̫��
				{
					TIM2_CH1_CAPTURE_STA|=0x80; //��־һ�β���ɹ�
					TIM2_CH1_CAPTURE_VAL=0xffff;
				}
				else
				{
					TIM2_CH1_CAPTURE_STA++;
				}
			}
		}
		if(TIM_GetITStatus(TIM2,TIM_IT_CC1)) //���������ж�
		{
			if(TIM2_CH1_CAPTURE_STA&0X40)//�����˵͵�ƽ
			{
				TIM2_CH1_CAPTURE_STA|=0x80; //�ɹ�����һ�θߵ�ƽ
				TIM2_CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //���������ز���
			}
			else
			{
				TIM2_CH1_CAPTURE_STA=0;
				TIM2_CH1_CAPTURE_VAL=0;
				TIM2_CH1_CAPTURE_STA|=0x40; //���񵽸ߵ�ƽ ��־
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0); //��ʱ����ֵΪ0
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling); //�����½��ز���
				TIM_Cmd(TIM2,ENABLE);
			}
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1|TIM_IT_Update);
}


