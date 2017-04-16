#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>//这个库 还要在编译器设置一个选项Use Microlib勾选


#define USART_REC_LEN  			512  	//定义最大接收字节数 512
	  	
void Usart1_Init(uint32_t baudrate);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void Get_ChipID(char *str);

#endif

