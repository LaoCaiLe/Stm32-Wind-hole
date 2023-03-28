/*
 * ************************************************
 * 
 * STM32 blink demo
 * 
 * CPU:     STM32F103C8
 * FREQ:    72MHz
 * PIN:     PA0
 * 
 * ************************************************
*/

#include "stm32f10x.h"
#include "sys.h"
#include "SysTick.h"
#include "oled.h"
#include "led.h"
#include "PidControl.h"
#include "ANO_Data_Transfer.h"
#include "usart.h"
#include "pwm.h"
#include "loop.h"
#include "sys_time.h"
#include "key.h"

extern PidObject PidDistance;

int main()
{
    SysTick_Init(72);	
    led_init();                  //PC13
    TIM3_PWM_Init(6000,9);       //PA6 电机PWM
    KEY_Init();         //按键初始化
    sys_time_init(20);  //定时器初始化
    USART1_Configuration();      //PA9,PA10 //上位机串口
    USART2_Configuration();      //PA2,PA3  接收测距模块串口
    PidInit(&PidDistance,700,0.8,0.0125,7,2000,0);   //位置式pid
    //PidInit(&PidDistance,600,0.8,0.0125,7,2000,0);    ///增量式pid

    OLED_Init();       //PB8,PB9 OLED初始化
    OLED_Clear();
    OLED_Display_Init();
    while(1)
	{

		Loop_Run();
    }
}
