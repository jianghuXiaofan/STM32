#ifndef __LED_H
#define __LED_H
#include"stm32f10x.h"
void led_Init(void);


#define ON 0
#define OFF 1

#define  LED0(n)	if(n)  	GPIO_SetBits(GPIOA,GPIO_Pin_8); \
					else \
						GPIO_ResetBits(GPIOA,GPIO_Pin_8)
					
#endif
