#ifndef sys_time_H
#define sys_time_H
#include "stm32f10x.h"
#include "stm32f10x_tim.h"


extern uint16_t time_count_20us;//20us 计数变量
extern uint16_t time_count_ms;//ms计数变量
extern uint16_t time_count_s;
extern uint16_t time_count_min;
extern uint16_t time_count_h;

void sys_time_init(uint32_t interrupt_time_us );
void count_time_per_20us(void);//20us计数
void count_time_per_5ms(void);//5ms计数
uint32_t get_sys_time_us(void);
uint32_t get_sys_time_ms(void);
uint32_t get_sys_time_s(void);
uint32_t get_sys_time_min(void);
uint32_t get_sys_time_h(void);

#endif
