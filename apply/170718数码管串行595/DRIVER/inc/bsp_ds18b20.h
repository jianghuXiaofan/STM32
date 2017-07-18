#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H 

#include "stm32f10x.h"
                  
#define DS18B20_CLK      RCC_APB2Periph_GPIOA
#define DS18B20_PIN      GPIO_Pin_0
#define DS18B20_PORT     GPIOA
 
////IO��������											   
#define	DS18B20_DQ_Set()      GPIO_SetBits(DS18B20_PORT,DS18B20_PIN)
#define	DS18B20_DQ_Crl()     GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN)        
#define DS18B20_DQ_IN()		GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)

   	
u8 DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    
#endif















