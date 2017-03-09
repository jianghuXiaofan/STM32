#ifndef __BSP_TIMER3_H
#define __BSP_TIMER3_H
#include "stm32f10x.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);

#endif

