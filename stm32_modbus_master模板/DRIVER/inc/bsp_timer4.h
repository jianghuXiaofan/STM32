#ifndef __BSP_TIMER4_H
#define __BSP_TIMER4_H
#include "stm32f10x.h"

void TIM4_Int_Init(u16 arr,u16 psc);
void TIM4_IRQHandler(void);

void TIM4_Enable(void);//Ê¹ÄÜ
void TIM4_Disable(void);//Ê§ÄÜ
#endif

