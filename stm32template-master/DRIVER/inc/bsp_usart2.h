#ifndef __BSP_USART2_H
#define __BSP_USART2_H

#include "stm32f10x.h"
#include <stdio.h>//这个库 还要在编译器设置一个选项Use Microlib勾选

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

void Usart2_Init(uint32_t baudrate);
void USART2_IRQHandler(void);

void usart2_send(int ch);
void USART2_Putc(unsigned char c);
void usart2_send_nbyte(unsigned char *str, int len);
	  	

#endif

