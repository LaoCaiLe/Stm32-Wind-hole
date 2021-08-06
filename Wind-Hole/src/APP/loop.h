/*
 * @Author: your name
 * @Date: 2020-08-25 09:24:16
 * @LastEditTime: 2020-09-11 21:50:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm32F407模板\src\USER\loop.h
 */
#ifndef _LOOP_H
#define _LOOP_H

#include "stm32f10x.h"
#include "sys.h"
#include "SysTick.h"
// #include "VL53L0.h"
// #include "VL53L0iic.h"
#include "oled.h"
#include "led.h"
#include "PidControl.h"
#include "ANO_Data_Transfer.h"
#include "usart.h"
#include "stdio.h"
#include "pwm.h"
#include "loop.h"
#include "sys_time.h"
#include "key.h"
#include "input.h"
#include "SR05.h"

typedef struct
{
	void(*task_func)(void);
	uint16_t interval_ticks;//执行时间间隔
	uint32_t last_run;//最后一次运行时间
}sched_task_t;

void Loop_Run(void );

#endif


