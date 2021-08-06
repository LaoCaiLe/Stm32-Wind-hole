#ifndef _MONITOR_H
#define _MONITOR_H

#include "stm32f10x.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )



void Test_Send_User(uint16_t data1, uint16_t data2, uint16_t data3,uint16_t data4,uint16_t data5,uint16_t data6,uint16_t data7,uint16_t data8,uint16_t data9,uint16_t data10);

void usart1_send_char(u8 c);


#endif