#include "bsp_dht11.h"
#include "timer.h"
#include "bsp_usart.h"

void Dht11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(DHT11_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(DHT11_PORT,DHT11_PIN);
}

static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DHT11_PORT,&GPIO_InitStructure);
}

static void DHT11_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT,&GPIO_InitStructure);
}

static uint8_t Read_Byte(void)
{
	uint8_t i, temp = 0;
	for(i=0;i<8;i++)
	{
		while(DHT11_DATA_IN() == Bit_RESET);
		delay_us(40);// 大于26到28 小于70us最好
		if(DHT11_DATA_IN()==Bit_SET)
		{
			while(DHT11_DATA_IN()==Bit_SET);
			temp |= (uint8_t)(0x01<<(7-i));
		}else
			temp &= (uint8_t)~(0x01<<(7-i)); 
	}
	return temp;
}

uint8_t Read_DHT11(DHT11_Data_TypeDef * DHT11_Data)
{
	DHT11_Mode_Out_PP();
	GPIO_ResetBits(DHT11_PORT,DHT11_PIN);
	delay_ms(20);//不小于18ms
	GPIO_SetBits(DHT11_PORT,DHT11_PIN);
	delay_us(30);
	
	DHT11_Mode_IPU();
	if(Bit_RESET  == DHT11_DATA_IN())
	{
		while(DHT11_DATA_IN() == Bit_RESET);
		while(DHT11_DATA_IN() == Bit_SET);
		delay_us(30);
		DHT11_Data->humi_int = Read_Byte();
		DHT11_Data->humi_deci = Read_Byte();
		DHT11_Data->temp_int = Read_Byte();
		DHT11_Data->temp_deci = Read_Byte();
		DHT11_Data->check_sum = Read_Byte();
		
		GPIO_ResetBits(DHT11_PORT,DHT11_PIN);
		DHT11_Mode_Out_PP();
		GPIO_SetBits(DHT11_PORT,DHT11_PIN);
		
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
		{
			return SUCCESS;
		}else 
			return ERROR;//失败
	}
	else{
		return ERROR;//失败
	}
}
