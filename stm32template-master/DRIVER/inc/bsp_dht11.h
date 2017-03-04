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

#define DHT11_DATA_IN()		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)

void Dht11_Init(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t Read_Byte(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef * DHT11_Data);
#endif

