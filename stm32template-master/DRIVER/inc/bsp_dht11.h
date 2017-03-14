#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "stm32f10x.h"
typedef struct 
{
	uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
	
}DHT11_Data_TypeDef;

#define HIGH  1
#define LOW   0

#define DHT11_CLK     RCC_APB2Periph_GPIOB//�޸�����
#define DHT11_PIN     GPIO_Pin_10                  
#define DHT11_PORT		GPIOB


//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(DHT11_PORT,DHT11_PIN);\
					else		\
					GPIO_ResetBits(DHT11_PORT,DHT11_PIN)
 //��ȡ���ŵĵ�ƽ
#define DHT11_DATA_IN()		GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

                    
void Dht11_Init(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t Read_Byte(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef * DHT11_Data);
                    
#endif

