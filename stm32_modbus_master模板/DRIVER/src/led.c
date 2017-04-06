#include "led.h"
/****************************

ledµ∆≤‚ ‘

*****************************/
void led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//enable GPIOA periph_clock
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

