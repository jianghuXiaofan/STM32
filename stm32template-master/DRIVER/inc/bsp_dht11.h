#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "stm32f10x.h"
typedef struct 
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
	
}DHT11_Data_TypeDef;

#define HIGH  1
#define LOW   0

#define DHT11_CLK     RCC_APB2Periph_GPIOB//修改这里
#define DHT11_PIN     GPIO_Pin_10                  
#define DHT11_PORT		GPIOB


//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(DHT11_PORT,DHT11_PIN);\
					else		\
					GPIO_ResetBits(DHT11_PORT,DHT11_PIN)
 //读取引脚的电平
#define DHT11_DATA_IN()		GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

                    
void Dht11_Init(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t Read_Byte(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef * DHT11_Data);
                    
#endif

