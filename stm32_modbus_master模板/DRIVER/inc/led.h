#ifndef __LED_H
#define __LED_H
#include"stm32f10x.h"
void led_Init(void);


#define ON 0
#define OFF 1

#define  LED0(n)	if(n)  	GPIO_SetBits(GPIOC,GPIO_Pin_13); \
					else \
						GPIO_ResetBits(GPIOC,GPIO_Pin_13)
					
#endif
