#include "bsp_led.h"

/****************************

led灯运行和错误指示灯 

*****************************/

void led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LEDRUN_CLK | LEDERR_CLK,ENABLE);//enable GPIO periph_clock
	
	GPIO_InitStructure.GPIO_Pin = LEDRUN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LEDRUN_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = LEDERR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LEDERR_PORT,&GPIO_InitStructure);

}

