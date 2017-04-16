#ifndef __BSP_IIC_A_H
#define __BSP_IIC_A_H
#include "stm32f10x.h"

#define IIC_CLK_Clk     RCC_APB2Periph_GPIOC
#define IIC_CLK_Pin     GPIO_Pin_12
#define IIC_CLK_Port    GPIOC

#define IIC_DAT_Clk     RCC_APB2Periph_GPIOC
#define IIC_DAT_Pin     GPIO_Pin_11
#define IIC_DAT_Port    GPIOC


#define IIC_CLK_H	GPIO_SetBits(IIC_CLK_Port,IIC_CLK_Pin)
#define IIC_CLK_L	GPIO_ResetBits(IIC_CLK_Port,IIC_CLK_Pin)
#define IIC_DAT_H	GPIO_SetBits(IIC_DAT_Port,IIC_DAT_Pin)
#define IIC_DAT_L	GPIO_ResetBits(IIC_DAT_Port,IIC_DAT_Pin)
#define READ_IIC_DAT	GPIO_ReadInputDataBit(IIC_DAT_Port,IIC_DAT_Pin)


void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);//²»µÈ´ýÓ¦´ð

void IIC_Send_Byte(uint8_t txb);
uint8_t IIC_Read_Byte(uint8_t ack);

//void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
//uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);

void IIC_DAT_OUT(void);
void IIC_DAT_IN(void);



#endif
