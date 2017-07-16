#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f10x.h"

//extern __IO uint32_t TimingDelay; 

void SysTick_Init(void);
void delay_us(__IO uint32_t nTime);
void delay_ms(__IO uint32_t nTime);
//软件延时，特殊场合使用
void Delay_1ms(unsigned int Del_1ms);
void Delay_50ms(unsigned int Del_50ms);
#endif
