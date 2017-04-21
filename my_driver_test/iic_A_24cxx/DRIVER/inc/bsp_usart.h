#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>//����� ��Ҫ�ڱ���������һ��ѡ��Use Microlib��ѡ


#define USART_REC_LEN  			512  	//�����������ֽ��� 512
	  	
void Usart1_Init(uint32_t baudrate);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void Get_ChipID(char *str);

#endif

