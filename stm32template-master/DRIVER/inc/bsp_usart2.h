#ifndef __BSP_USART2_H
#define __BSP_USART2_H

#include "stm32f10x.h"
#include <stdio.h>//����� ��Ҫ�ڱ���������һ��ѡ��Use Microlib��ѡ

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void Usart2_Init(uint32_t baudrate);
void USART2_IRQHandler(void);

void usart2_send(int ch);
void USART2_Putc(unsigned char c);
void usart2_send_nbyte(unsigned char *str, int len);
	  	

#endif

