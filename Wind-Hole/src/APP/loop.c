#include "loop.h"

PidObject PidDistance; 	//pid结构体定义
float BallDistance = 0;	//小球高度
char disbuff[20];

int16_t PwmMotor=0;	//电机pid输出值
int16_t key_press;	//按键
int32_t time1=0,time2;
int32_t tnow; 			//系统当前时间
int8_t arrow_y = 0;	//oled屏箭头显示下标

bool Mode_BallCheck = 0;					//检测小球模式
bool Mode_BallCheck_period0 = 1;	//检测小球模式阶段0
bool Mode_BallCheck_period1 = 0;	//检测小球模式阶段1
bool Mode_BallCheck_period2 = 0;	//检测小球模式阶段2

int error_size = 50;	//设定误差范围50cm
bool Mode_Updown = 0;					//小球自动变换高度模式
bool Mode_Updown_period1 = 1;	//小球自动变换高度模式阶段1
bool Mode_Updown_period2 = 0;	//小球自动变换高度模式阶段2
bool Mode_Updown_period3 = 0;	//小球自动变换高度模式阶段3
bool Mode_Updown_period4 = 0;	//小球自动变换高度模式阶段4
bool Mode_Updown_period5 = 0;	//小球自动变换高度模式阶段5

bool Mode_SetHigh =  0;	//手动设定高度模式

bool Pid_Enable  = 0;		//pid开启标志
bool Oled_Enable = 1;		//oled箭头显示标志
bool Key_Enable  = 1;		//按键事件1开启标志
bool Key2_Enable = 0;		//按键事件2开启标志


/****4ms服务函数****/
static void Duty_4ms(void)
{ 	
	BallDistance = USART_RX_BUF[2]+USART_RX_BUF[3]*256;	//距离计算
	BallDistance = 1045 - BallDistance;	
}

/****8ms服务函数****/
static void Duty_8ms(void)
{   
	//发送数据到上位机
	Test_Send_User(PidDistance.desired, BallDistance, TIM3->CCR1,Mode_BallCheck_period0,Mode_BallCheck_period1,Mode_BallCheck_period2,PidDistance.desired - BallDistance,PidDistance.integ,PidDistance.integ*PidDistance.ki,0);

}

/****20ms服务函数****/
static void Duty_20ms(void)
{	
	if(Mode_Updown)	//小球自动变换高度模式
	{
		Pid_Enable = 1;
		if(Mode_Updown_period1)  //执行阶段1
		{			
			PidDistance.desired = 800;	//设置高度80cm

			if(ABS(PidDistance.error) < error_size)  //误差在5cm范围内启动计时
			{
				time1++;
			}
			else
			{
				time1 = 0;
			}
			if(time1>250)   //小球维持5s时退出阶段1 250*25ms =5s
			{
				time1 = 0;
				Mode_Updown_period1 = 0;	//关闭阶段1
				Mode_Updown_period2 = 1;	//开启阶段2
			}
		}
		
		if(Mode_Updown_period2)  //执行阶段2
		{
			PidDistance.desired = 400;	//设置高度40cm
			
			if(!time2)  
			{
				PidDistance.integ= 20000;//给定积分项,减小积分饱和的影响
				time2 = 1;
				
			}
			if(ABS(PidDistance.error) < error_size)	 //误差在5cm范围内启动计时
			{
				
				time1++;
			}
			else
			{
				time1 = 0;
			}

			if(time1 > 250)	//小球维持5s时退出阶段2 250*25ms =5s
			{
				time1 = 0;
				time2 = 0;
				Mode_Updown_period2 = 0;	//关闭阶段2
				Mode_Updown_period3 = 1;	//开启阶段3
				
			}
		}
		
		if(Mode_Updown_period3)	//执行阶段3
		{
			
			PidDistance.desired = 800;	//设置高度80cm
			if(!time2)		
			{
				PidDistance.integ= 7000;	//给定积分项，减小积分饱和的影响
				time2 =1 ;
			}
			if(ABS(PidDistance.error) < error_size)	//误差在5cm范围内启动计时
			{
				time1++;
			}
			else
			{
				time1 = 0;
			}
		
		
			if(time1 > 250) //小球维持5s时退出阶段3 250*25ms =5s
			{
				time1 = 0;
				time2 = 0;
				Mode_Updown_period3 = 0;	//关闭阶段3
				Mode_Updown_period4 = 1;	//开启阶段4
			}
		}
		
		if(Mode_Updown_period4)//执行阶段4
		{
			PidDistance.desired = 400;	//设置高度40cm
			if(!time2)
			{
				PidDistance.integ= 20000; //给定积分项，减小积分饱和的影响
				time2 =1 ;
			}
			
			if(ABS(PidDistance.error) < error_size)	//误差在5cm范围内启动计时
			{
				time1++;
			}			
			else
			{
				time1 = 0;
			}
			
			
			if(time1>250) //小球维持5s时退出阶段3 250*25ms =5s
			{
				time1 = 0;
				time2 = 0;	
				Mode_Updown_period4 = 0;	//关闭阶段4
				Mode_Updown_period5 = 1;	//开启阶段5
				Key_Enable =1;			
			}
		}
		
		if(Mode_Updown_period5)
		{
			// Pid_Enable = 0;
			// time1++;
			// TIM_SetCompare1(TIM3, 1000); //让小球飞出风洞
			// if(time1 > 70)
			// {
				Motor_close();  //关闭电机
				Mode_Updown = 0;
				Mode_Updown_period5 = 0;
				Mode_Updown_period1 = 1;
				Pid_Enable = 0;
				// Oled_Enable = 1;
				// Key_Enable = 1;
			// }
		}
	}
	
	if(Mode_BallCheck) //小球检测模式
	{	
		if(Mode_BallCheck_period0)  //执行阶段0
		{
			Pid_Enable = 0;
			TIM_SetCompare1(TIM3, 1300); //开启电机让小球吹出
			if(BallDistance<800&&tnow-time1>1000)   		//检测小球是否已出洞
			{		
				Mode_BallCheck_period0 = 0;
				Mode_BallCheck_period1 = 1; 
				Motor_close();//小球出洞关闭电机
				delay_ms(100);
				time1 = 0;
			}
		}
		
		if(Mode_BallCheck_period1)	//执行阶段1
		{
			Motor_close();
			Mode_BallCheck_period0 = 0;
			if(BallDistance>800)  //检测是否有小球进入风洞
			{
				TIM_SetCompare1(TIM3, 1700);
				delay_ms(20);				//开启一段时间高占空比不让小球下降过大距离
				Pid_Enable = 1;			//开启pid控制
				PidDistance.desired = 800;	//设定高度
				Mode_BallCheck_period1 = 0;
				Mode_BallCheck_period2 = 1;
			
			}
		}
		
		if(Mode_BallCheck_period2)	//执行阶段2
		{
			if(ABS(PidDistance.error) < 50)	//计算误差小于5cm的时间
			{
				time1++;
			}
			else
			{
				time1 = 0;
			}
			if(time1>250)		//判断小球是否维持5s
			{
				Pid_Enable = 0;	//小球维持5s关闭pid控制
				Motor_close();	//关闭电机
				Mode_BallCheck_period2 = 0;
				Mode_BallCheck_period0 = 1;
				Mode_BallCheck = 0;		//该模式结束
				Oled_Enable = 1;
				Key_Enable = 1;
			}
		}
	}
	
	if(Mode_SetHigh)	//设定高度模式
	{	
		//OLED_Clear(); //OLED清屏
		OLED_ShowString(0,0,"Set Height:",10);
		OLED_Float(90, 0, PidDistance.desired,1);//显示高度设定值
		Oled_Enable = 0;
		int key_press2;
	
		
		if(Key2_Enable)
		{
			key_press2 = KEY_Scan(0);	
			
			switch(key_press2)
			{
				//向上按键 增加高度设定值
				case KEY_UP:	PidDistance.desired+=50; //设定值+50
								PidDistance.desired = (PidDistance.desired>1000? 950:PidDistance.desired);
								break;	
				
				//向下按键 减小高度设定值
				case KEY_DOWN:	PidDistance.desired-=50;//设定值-50
								PidDistance.desired = (PidDistance.desired<100? 100:PidDistance.desired); 
								break;	
				
				//向左按键 开启pid
				case KEY_LEFT:	Pid_Enable = 1; 
												break; 
				
				//向右按键 退出该模式
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

/****40ms服务函数****/
static void Duty_40ms(void)
{
	if(BallDistance<1300&&BallDistance>0)	//由于传感器有时可能检测失误，当检测失误时不开启pid控制
	{	
		if(Pid_Enable)
		{
			PwmMotor = pidUpdate(&PidDistance, -(BallDistance - PidDistance.desired));		//pid计算	
			PwmMotor = PwmMotor+400;	//400为电机克服小球重力的占空比
      TIM_SetCompare1(TIM3, PwmMotor);				
		}
	}
	else
	{
    Pid_Enable  = 0;
		Motor_close();
	}

	//按键1事件
	if(Key_Enable)
	{
		key_press = KEY_Scan(0);	
		switch(key_press)
		{
			//向上按键 箭头下标向上
			case KEY_UP:	OLED_ShowChar(110,arrow_y,' ',10);
										OLED_ShowChar(118,arrow_y,' ',10);
										arrow_y-=2; if(arrow_y<0) arrow_y =4;
											break;
			
			//向下按键 箭头下标向下
			case KEY_DOWN:	OLED_ShowChar(110,arrow_y,' ',10);
											OLED_ShowChar(118,arrow_y,' ',10);
											arrow_y+=2; if(arrow_y>4) arrow_y =0;
											break;
			
			//向左按键 选择该模式
			case KEY_LEFT:	switch(arrow_y)
							{
								case 0: Mode_Updown = 1; 	Mode_BallCheck = 0;  Mode_SetHigh = 0;		  break;
								case 2: Mode_Updown = 0; 	Mode_BallCheck = 1;  Mode_SetHigh = 0;	time1=tnow;		break;
								case 4: Mode_Updown = 0; 	Mode_BallCheck = 0;  Mode_SetHigh = 1;		 
												OLED_Clear();   	
												Key2_Enable = 1;	//开启另一个按键事件
												Key_Enable = 0;				  
												break;
							}
							break;	
			
			//向右按键 退出该模式
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

/****200ms服务函数****/
static void Duty_200ms(void)
{
	static bool x=0;
  led = !led; //led闪烁
	// OLED_ShowNum(74, 0, pwmConfig2.dutyCycle, 1, 10);
	// OLED_ShowNum(74, 2, BallDistance, 1, 10);
	// OLED_ShowNum(40, 4, PidDistance.kp, 0,10);
	if(Oled_Enable)
	{
		//下标闪烁显示
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

//系统任务配置，创建不同执行周期的“线程”
static sched_task_t sched_tasks[] =
{

    {Duty_200ms, 200, 0},
    {Duty_40ms, 40, 0},
	{Duty_20ms, 20, 0},
    {Duty_8ms, 8, 0},
    {Duty_4ms, 4, 0},
    
};
//根据数组长度，判断线程数量
#define TASK_NUM (sizeof(sched_tasks) / sizeof(sched_task_t))

//这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
void Loop_Run(void)
{
    while (1)
    { 
        u8 index = 0;
        //循环判断所有线程，是否应该执行
        for (index = 0; index < TASK_NUM; index++)
        {
            //获取系统当前时间，单位MS --
            // tnow = getSysTickCnt();
            tnow = time_count_ms;
            //进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
            if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
            {
                //更新线程的执行时间，用于下一次判断
                sched_tasks[index].last_run = tnow;
                //执行线程函数，使用的是函数指针
                sched_tasks[index].task_func(); 
            }
        }
    }
}

