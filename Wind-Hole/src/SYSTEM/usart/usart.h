#ifndef _USART_H
#define _USART_H


#define usart1_printf
//#define usart2_printf

#include "stdio.h"
#include "led.h"
#include "sys.h"
extern unsigned char  USART_RX_BUF[9]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 


void USART1_Configuration(void);
void USART2_Configuration(void);

#endif