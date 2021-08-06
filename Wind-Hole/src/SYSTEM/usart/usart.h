#ifndef _USART_H
#define _USART_H


#define usart1_printf
//#define usart2_printf

#include "stdio.h"
#include "led.h"
#include "sys.h"
extern unsigned char  USART_RX_BUF[9]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 


void USART1_Configuration(void);
void USART2_Configuration(void);

#endif