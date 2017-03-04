#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H

#include "stm32f10x.h"


#define  Relay(n)	if(n)  	GPIO_ResetBits(GPIOA,GPIO_Pin_4); \
					else \
						GPIO_SetBits(GPIOA,GPIO_Pin_4)
                    
void relayInit(void);

#endif

