#include "loop.h"

PidObject PidDistance; 	//pid�ṹ�嶨��
float BallDistance = 0;	//С��߶�
char disbuff[20];

int16_t PwmMotor=0;	//���pid���ֵ
int16_t key_press;	//����
int32_t time1=0,time2;
int32_t tnow; 			//ϵͳ��ǰʱ��
int8_t arrow_y = 0;	//oled����ͷ��ʾ�±�

bool Mode_BallCheck = 0;					//���С��ģʽ
bool Mode_BallCheck_period0 = 1;	//���С��ģʽ�׶�0
bool Mode_BallCheck_period1 = 0;	//���С��ģʽ�׶�1
bool Mode_BallCheck_period2 = 0;	//���С��ģʽ�׶�2

int error_size = 50;	//�趨��Χ50cm
bool Mode_Updown = 0;					//С���Զ��任�߶�ģʽ
bool Mode_Updown_period1 = 1;	//С���Զ��任�߶�ģʽ�׶�1
bool Mode_Updown_period2 = 0;	//С���Զ��任�߶�ģʽ�׶�2
bool Mode_Updown_period3 = 0;	//С���Զ��任�߶�ģʽ�׶�3
bool Mode_Updown_period4 = 0;	//С���Զ��任�߶�ģʽ�׶�4
bool Mode_Updown_period5 = 0;	//С���Զ��任�߶�ģʽ�׶�5

bool Mode_SetHigh =  0;	//�ֶ��趨�߶�ģʽ

bool Pid_Enable  = 0;		//pid������־
bool Oled_Enable = 1;		//oled��ͷ��ʾ��־
bool Key_Enable  = 1;		//�����¼�1������־
bool Key2_Enable = 0;		//�����¼�2������־


/****4ms������****/
static void Duty_4ms(void)
{ 	
	BallDistance = USART_RX_BUF[2]+USART_RX_BUF[3]*256;	//�������
	BallDistance = 1045 - BallDistance;	
}

/****8ms������****/
static void Duty_8ms(void)
{   
	//�������ݵ���λ��
	Test_Send_User(PidDistance.desired, BallDistance, TIM3->CCR1,Mode_BallCheck_period0,Mode_BallCheck_period1,Mode_BallCheck_period2,PidDistance.desired - BallDistance,PidDistance.integ,PidDistance.integ*PidDistance.ki,0);

}

/****20ms������****/
static void Duty_20ms(void)
{	
	if(Mode_Updown)	//С���Զ��任�߶�ģʽ
	{
		Pid_Enable = 1;
		if(Mode_Updown_period1)  //ִ�н׶�1
		{			
			PidDistance.desired = 800;	//���ø߶�80cm

			if(ABS(PidDistance.error) < error_size)  //�����5cm��Χ��������ʱ
			{
				time1++;
			}
			else
			{
				time1 = 0;
			}
			if(time1>250)   //С��ά��5sʱ�˳��׶�1 250*25ms =5s
			{
				time1 = 0;
				Mode_Updown_period1 = 0;	//�رս׶�1
				Mode_Updown_period2 = 1;	//�����׶�2
			}
		}
		
		if(Mode_Updown_period2)  //ִ�н׶�2
		{
			PidDistance.desired = 400;	//���ø߶�40cm
			
			if(!time2)  
			{
				PidDistance.integ= 20000;//����������,��С���ֱ��͵�Ӱ��
				time2 = 1;
				
			}
			if(ABS(PidDistance.error) < error_size)	 //�����5cm��Χ��������ʱ
			{
				
				time1++;
			}
			else
			{
				time1 = 0;
			}

			if(time1 > 250)	//С��ά��5sʱ�˳��׶�2 250*25ms =5s
			{
				time1 = 0;
				time2 = 0;
				Mode_Updown_period2 = 0;	//�رս׶�2
				Mode_Updown_period3 = 1;	//�����׶�3
				
			}
		}
		
		if(Mode_Updown_period3)	//ִ�н׶�3
		{
			
			PidDistance.desired = 800;	//���ø߶�80cm
			if(!time2)		
			{
				PidDistance.integ= 7000;	//�����������С���ֱ��͵�Ӱ��
				time2 =1 ;
			}
			if(ABS(PidDistance.error) < error_size)	//�����5cm��Χ��������ʱ
			{
				time1++;
			}
			else
			{
				time1 = 0;
			}
		
		
			if(time1 > 250) //С��ά��5sʱ�˳��׶�3 250*25ms =5s
			{
				time1 = 0;
				time2 = 0;
				Mode_Updown_period3 = 0;	//�رս׶�3
				Mode_Updown_period4 = 1;	//�����׶�4
			}
		}
		
		if(Mode_Updown_period4)//ִ�н׶�4
		{
			PidDistance.desired = 400;	//���ø߶�40cm
			if(!time2)
			{
				PidDistance.integ= 20000; //�����������С���ֱ��͵�Ӱ��
				time2 =1 ;
			}
			
			if(ABS(PidDistance.error) < error_size)	//�����5cm��Χ��������ʱ
			{
				time1++;
			}			
			else
			{
				time1 = 0;
			}
			
			
			if(time1>250) //С��ά��5sʱ�˳��׶�3 250*25ms =5s
			{
				time1 = 0;
				time2 = 0;	
				Mode_Updown_period4 = 0;	//�رս׶�4
				Mode_Updown_period5 = 1;	//�����׶�5
				Key_Enable =1;			
			}
		}
		
		if(Mode_Updown_period5)
		{
			// Pid_Enable = 0;
			// time1++;
			// TIM_SetCompare1(TIM3, 1000); //��С��ɳ��綴
			// if(time1 > 70)
			// {
				Motor_close();  //�رյ��
				Mode_Updown = 0;
				Mode_Updown_period5 = 0;
				Mode_Updown_period1 = 1;
				Pid_Enable = 0;
				// Oled_Enable = 1;
				// Key_Enable = 1;
			// }
		}
	}
	
	if(Mode_BallCheck) //С����ģʽ
	{	
		if(Mode_BallCheck_period0)  //ִ�н׶�0
		{
			Pid_Enable = 0;
			TIM_SetCompare1(TIM3, 1300); //���������С�򴵳�
			if(BallDistance<800&&tnow-time1>1000)   		//���С���Ƿ��ѳ���
			{		
				Mode_BallCheck_period0 = 0;
				Mode_BallCheck_period1 = 1; 
				Motor_close();//С������رյ��
				delay_ms(100);
				time1 = 0;
			}
		}
		
		if(Mode_BallCheck_period1)	//ִ�н׶�1
		{
			Motor_close();
			Mode_BallCheck_period0 = 0;
			if(BallDistance>800)  //����Ƿ���С�����綴
			{
				TIM_SetCompare1(TIM3, 1700);
				delay_ms(20);				//����һ��ʱ���ռ�ձȲ���С���½��������
				Pid_Enable = 1;			//����pid����
				PidDistance.desired = 800;	//�趨�߶�
				Mode_BallCheck_period1 = 0;
				Mode_BallCheck_period2 = 1;
			
			}
		}
		
		if(Mode_BallCheck_period2)	//ִ�н׶�2
		{
			if(ABS(PidDistance.error) < 50)	//�������С��5cm��ʱ��
			{
				time1++;
			}
			else
			{
				time1 = 0;
			}
			if(time1>250)		//�ж�С���Ƿ�ά��5s
			{
				Pid_Enable = 0;	//С��ά��5s�ر�pid����
				Motor_close();	//�رյ��
				Mode_BallCheck_period2 = 0;
				Mode_BallCheck_period0 = 1;
				Mode_BallCheck = 0;		//��ģʽ����
				Oled_Enable = 1;
				Key_Enable = 1;
			}
		}
	}
	
	if(Mode_SetHigh)	//�趨�߶�ģʽ
	{	
		//OLED_Clear(); //OLED����
		OLED_ShowString(0,0,"Set Height:",10);
		OLED_Float(90, 0, PidDistance.desired,1);//��ʾ�߶��趨ֵ
		Oled_Enable = 0;
		int key_press2;
	
		
		if(Key2_Enable)
		{
			key_press2 = KEY_Scan(0);	
			
			switch(key_press2)
			{
				//���ϰ��� ���Ӹ߶��趨ֵ
				case KEY_UP:	PidDistance.desired+=50; //�趨ֵ+50
								PidDistance.desired = (PidDistance.desired>1000? 950:PidDistance.desired);
								break;	
				
				//���°��� ��С�߶��趨ֵ
				case KEY_DOWN:	PidDistance.desired-=50;//�趨ֵ-50
								PidDistance.desired = (PidDistance.desired<100? 100:PidDistance.desired); 
								break;	
				
				//���󰴼� ����pid
				case KEY_LEFT:	Pid_Enable = 1; 
												break; 
				
				//���Ұ��� �˳���ģʽ
				case KEY_RIGHT:	Pid_Enable = 0; 
												Key2_Enable = 0;
												Key_Enable = 1;
												Mode_SetHigh = 0;
												Motor_close();
												OLED_Clear();
												OLED_Display_Init();
												Oled_Enable = 1;
												break;
			}			
		}		
	}	
	
}

/****40ms������****/
static void Duty_40ms(void)
{
	if(BallDistance<1300&&BallDistance>0)	//���ڴ�������ʱ���ܼ��ʧ�󣬵����ʧ��ʱ������pid����
	{	
		if(Pid_Enable)
		{
			PwmMotor = pidUpdate(&PidDistance, -(BallDistance - PidDistance.desired));		//pid����	
			PwmMotor = PwmMotor+400;	//400Ϊ����˷�С��������ռ�ձ�
      TIM_SetCompare1(TIM3, PwmMotor);				
		}
	}
	else
	{
    Pid_Enable  = 0;
		Motor_close();
	}

	//����1�¼�
	if(Key_Enable)
	{
		key_press = KEY_Scan(0);	
		switch(key_press)
		{
			//���ϰ��� ��ͷ�±�����
			case KEY_UP:	OLED_ShowChar(110,arrow_y,' ',10);
										OLED_ShowChar(118,arrow_y,' ',10);
										arrow_y-=2; if(arrow_y<0) arrow_y =4;
											break;
			
			//���°��� ��ͷ�±�����
			case KEY_DOWN:	OLED_ShowChar(110,arrow_y,' ',10);
											OLED_ShowChar(118,arrow_y,' ',10);
											arrow_y+=2; if(arrow_y>4) arrow_y =0;
											break;
			
			//���󰴼� ѡ���ģʽ
			case KEY_LEFT:	switch(arrow_y)
							{
								case 0: Mode_Updown = 1; 	Mode_BallCheck = 0;  Mode_SetHigh = 0;		  break;
								case 2: Mode_Updown = 0; 	Mode_BallCheck = 1;  Mode_SetHigh = 0;	time1=tnow;		break;
								case 4: Mode_Updown = 0; 	Mode_BallCheck = 0;  Mode_SetHigh = 1;		 
												OLED_Clear();   	
												Key2_Enable = 1;	//������һ�������¼�
												Key_Enable = 0;				  
												break;
							}
							break;	
			
			//���Ұ��� �˳���ģʽ
			case KEY_RIGHT:	Pid_Enable = 0; 
											
											Mode_Updown = 0;
											Mode_BallCheck = 0;
											Mode_SetHigh = 0;
											Motor_close();
											OLED_Clear();
											OLED_Display_Init();
											Oled_Enable = 1;
											break;
		}
	}
	
	
}

/****200ms������****/
static void Duty_200ms(void)
{
	static bool x=0;
  led = !led; //led��˸
	// OLED_ShowNum(74, 0, pwmConfig2.dutyCycle, 1, 10);
	// OLED_ShowNum(74, 2, BallDistance, 1, 10);
	// OLED_ShowNum(40, 4, PidDistance.kp, 0,10);
	if(Oled_Enable)
	{
		//�±���˸��ʾ
		if(x)
		{
			OLED_ShowString(110,arrow_y,(u8*)"<-",10);
			x=0;
		}
		else 
		{
			OLED_ShowChar(110,arrow_y,' ',10);
			OLED_ShowChar(118,arrow_y,' ',10);
			x=1;
		}
		
	}
	sprintf(disbuff,"Height:%.1f",BallDistance);
  OLED_ShowString(0,6,(u8*)disbuff,10);

}

//ϵͳ�������ã�������ִͬ�����ڵġ��̡߳�
static sched_task_t sched_tasks[] =
{

    {Duty_200ms, 200, 0},
    {Duty_40ms, 40, 0},
	{Duty_20ms, 20, 0},
    {Duty_8ms, 8, 0},
    {Duty_4ms, 4, 0},
    
};
//�������鳤�ȣ��ж��߳�����
#define TASK_NUM (sizeof(sched_tasks) / sizeof(sched_task_t))

//��������ŵ�main������while(1)�У���ͣ�ж��Ƿ����߳�Ӧ��ִ��
void Loop_Run(void)
{
    while (1)
    { 
        u8 index = 0;
        //ѭ���ж������̣߳��Ƿ�Ӧ��ִ��
        for (index = 0; index < TASK_NUM; index++)
        {
            //��ȡϵͳ��ǰʱ�䣬��λMS --
            // tnow = getSysTickCnt();
            tnow = time_count_ms;
            //�����жϣ������ǰʱ���ȥ��һ��ִ�е�ʱ�䣬���ڵ��ڸ��̵߳�ִ�����ڣ���ִ���߳�
            if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
            {
                //�����̵߳�ִ��ʱ�䣬������һ���ж�
                sched_tasks[index].last_run = tnow;
                //ִ���̺߳�����ʹ�õ��Ǻ���ָ��
                sched_tasks[index].task_func(); 
            }
        }
    }
}

