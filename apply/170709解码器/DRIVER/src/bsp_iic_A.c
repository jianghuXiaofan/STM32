#include "bsp_iic_A.h"
#include "bsp_systick.h"

void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(IIC_CLK_Clk|IIC_DAT_Clk,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = IIC_CLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_CLK_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = IIC_DAT_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_DAT_Port,&GPIO_InitStructure);
	
	IIC_CLK_H;
	IIC_DAT_H;
}

void IIC_DAT_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = IIC_DAT_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_DAT_Port,&GPIO_InitStructure);
}

void IIC_DAT_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = IIC_DAT_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_DAT_Port,&GPIO_InitStructure);
}

//产生IIC起始信号
void IIC_Start(void)
{
	IIC_DAT_OUT();
	IIC_CLK_H;
	IIC_DAT_H;
	delay_us(4);
	IIC_DAT_L;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_CLK_L;
}
//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_DAT_OUT();
	IIC_CLK_L;
	IIC_DAT_L;
	delay_us(4);
	IIC_CLK_H;//STOP:when CLK is high DATA change form low to high
	IIC_DAT_H;
	delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	IIC_DAT_IN();
	IIC_CLK_H;delay_us(1);
	IIC_DAT_H;delay_us(1);
	while(READ_IIC_DAT)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_CLK_L;
	return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
	IIC_CLK_L;
	IIC_DAT_OUT();
	IIC_DAT_L;
	delay_us(2);
	IIC_CLK_H;
	delay_us(2);
	IIC_CLK_L;
}
//不产生ACK应答
void IIC_NAck(void)
{
	IIC_CLK_L;
	IIC_DAT_OUT();
	IIC_DAT_H;
	delay_us(2);
	IIC_CLK_H;
	delay_us(2);
	IIC_CLK_L;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答	
void IIC_Send_Byte(uint8_t txb)
{
	uint8_t t;
	IIC_DAT_OUT();
	IIC_CLK_L;
	for(t=0;t<8;t++)
	{
		(((txb&0x80)>>7)>0)?IIC_DAT_H:IIC_DAT_L;
		txb<<=1;
		delay_us(2);
		IIC_CLK_H;
		delay_us(2);
		IIC_CLK_L;
		delay_us(2);
	}
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	IIC_DAT_IN();
	for(i=0;i<8;i++)
	{
		IIC_CLK_L;
		delay_us(2);
		IIC_CLK_H;
		receive<<=1;
		if(READ_IIC_DAT)receive++;
		delay_us(1);
	}
	if(!ack)IIC_NAck();
	else IIC_Ack();
	return receive;
}

