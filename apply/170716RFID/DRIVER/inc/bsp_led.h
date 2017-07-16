#ifndef __BSP_LED_H
#define __BSP_LED_H

#include  "stm32f10x.h"

#define ON 0
#define OFF 1

#define LEDERR_CLK      RCC_APB2Periph_GPIOB
#define LEDERR_PIN      GPIO_Pin_14
#define LEDERR_PORT     GPIOB

#define LEDRUN_CLK      RCC_APB2Periph_GPIOC
#define LEDRUN_PIN      GPIO_Pin_13
#define LEDRUN_PORT     GPIOC

#define LEDERR(n)   if(n)   GPIO_SetBits(LEDERR_PORT,LEDERR_PIN); \
                    else \
                        GPIO_ResetBits(LEDERR_PORT,LEDERR_PIN)
                    
#define  LEDRUN(n)	if(n)  	GPIO_SetBits(LEDRUN_PORT,LEDRUN_PIN); \
					else \
						GPIO_ResetBits(LEDRUN_PORT,LEDRUN_PIN)

                    
#define digitalToggle(p,i)		{p->ODR ^=i;}			//Êä³ö·´×ª×´Ì¬

#define LEDRUN_TOGGLE		digitalToggle(LEDRUN_PORT,LEDRUN_PIN)
#define LEDERR_TOGGLE       digitalToggle(LEDERR_PORT,LEDERR_PIN)


void led_Init(void);

#endif
