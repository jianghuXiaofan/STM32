#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H 

#include "stm32f10x.h"
                  
#define DS18B20_CLK      RCC_APB2Periph_GPIOA
#define DS18B20_PIN      GPIO_Pin_0
#define DS18B20_PORT     GPIOA
 
////IO操作函数											   
#define	DS18B20_DQ_Set()      GPIO_SetBits(DS18B20_PORT,DS18B20_PIN)
#define	DS18B20_DQ_Crl()     GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN)        
#define DS18B20_DQ_IN()		GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)

   	
u8 DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    
#endif















