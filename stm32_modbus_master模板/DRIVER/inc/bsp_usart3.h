#ifndef __BSP_USART3_H
#define __BSP_USART3_H

#include "stm32f10x.h"
#include <stdio.h>//����� ��Ҫ�ڱ���������һ��ѡ��Use Microlib��ѡ

#define USART3_REC_LEN  			512  	//�����������ֽ��� 512
//PC9 
#define RS485_TX_EN(n)  if(n) GPIO_SetBits(GPIOA,GPIO_Pin_8); \
                        else \
						GPIO_ResetBits(GPIOA,GPIO_Pin_8)
                        

                        
void Usart3_Init(uint32_t baudrate);
void USART3_IRQHandler(void);
void beginSend3(void);
                        
void usart3_send(int ch);
void USART3_Putc(unsigned char c);
void usart3_send_nbyte(unsigned char *str, int len);
	  	

#endif

