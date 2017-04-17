#ifndef __BSP_SPI1_H
#define __BSP_SPI1_H
#include "stm32f10x.h"

#define CC_CSN_Clk		RCC_APB2Periph_GPIOC
#define CC_CSN_Pin		GPIO_Pin_4
#define CC_CSN_Port  	GPIOC

#define CC_IRQ_Clk 		RCC_APB2Periph_GPIOA
#define CC_IRQ_Pin		GPIO_Pin_4
#define CC_IRQ_Port		GPIOA

#define CC_GDO2_Clk 	RCC_APB2Periph_GPIOC
#define CC_GDO2_Pin		GPIO_Pin_5
#define CC_GDO2_Port	GPIOC


#define CC_CSN_H()		GPIO_SetBits(CC_CSN_Port,CC_CSN_Pin)
#define CC_CSN_L()		GPIO_ResetBits(CC_CSN_Port,CC_CSN_Pin)

#define CC_IRQ_READ()	GPIO_ReadInputDataBit(CC_IRQ_Port,CC_IRQ_Pin)



void SPI1_Init(void);
void SPI1_SetSpeed(uint8_t SpeedSet);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);

#endif



