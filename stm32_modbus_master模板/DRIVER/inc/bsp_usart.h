#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>//这个库 还要在编译器设置一个选项Use Microlib勾选


void Usart1_Init(uint32_t baudrate);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void Get_ChipID(char *str);
unsigned int ELFHash(char* str, unsigned int len);

#endif

