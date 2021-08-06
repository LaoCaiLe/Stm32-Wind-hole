#include "ANO_Data_Transfer.h"

u8 testdatatosend[50];
void Test_Send_User(uint16_t data1, uint16_t data2, uint16_t data3,uint16_t data4,uint16_t data5,uint16_t data6,uint16_t data7,uint16_t data8,uint16_t data9,uint16_t data10)	
{
	u8 _cnt=0;
    u8 i;
    u8 sum = 0;
	
	testdatatosend[_cnt++]=0xAA;
	testdatatosend[_cnt++]=0x05;
	testdatatosend[_cnt++]=0xAF;
	testdatatosend[_cnt++]=0xF1;
	testdatatosend[_cnt++]=0;
 
	testdatatosend[_cnt++]=BYTE1(data1);
	testdatatosend[_cnt++]=BYTE0(data1);
	
	testdatatosend[_cnt++]=BYTE1(data2);
	testdatatosend[_cnt++]=BYTE0(data2);
	
	testdatatosend[_cnt++]=BYTE1(data3);
	testdatatosend[_cnt++]=BYTE0(data3);
	
	testdatatosend[_cnt++]=BYTE1(data4);
	testdatatosend[_cnt++]=BYTE0(data4);
	
	testdatatosend[_cnt++]=BYTE1(data5);
	testdatatosend[_cnt++]=BYTE0(data5);
	
	testdatatosend[_cnt++]=BYTE1(data6);
	testdatatosend[_cnt++]=BYTE0(data6);
	
	testdatatosend[_cnt++]=BYTE1(data7);
	testdatatosend[_cnt++]=BYTE0(data7);
	
	testdatatosend[_cnt++]=BYTE1(data8);
	testdatatosend[_cnt++]=BYTE0(data8);
	
	testdatatosend[_cnt++]=BYTE1(data9);
	testdatatosend[_cnt++]=BYTE0(data9);
	
	testdatatosend[_cnt++]=BYTE1(data10);
	testdatatosend[_cnt++]=BYTE0(data10);
	
	
 
	testdatatosend[4] = _cnt-5;
	
		
	for( i=0;i<_cnt;i++)
		sum += testdatatosend[i];
	
	testdatatosend[_cnt++]=sum;	
	for(i=0;i<_cnt;i++) usart1_send_char(testdatatosend[i]);	//发送数据到串口1

}

void usart1_send_char(u8 c)//发送字符串 函数   应用指针 方法
{				
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	    USART_SendData(USART1,c); 
}
