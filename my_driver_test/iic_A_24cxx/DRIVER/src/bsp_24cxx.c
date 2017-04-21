#include "bsp_24cxx.h"
#include "bsp_iic_A.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)							
{
	uint8_t temp = 0;
	IIC_Start();
	if(EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0xA0);//发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);
        IIC_Wait_Ack();
	}
	else
	{
		IIC_Send_Byte(0xA0+((ReadAddr/256)<<1));
	}
	
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr%256);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0xA1);//进入接收模式
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
	IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);//发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);
	}
	else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr%256);
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);//这么大
}		
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{
	int i;
	for(i=0;i<Len;i++)
	{
		AT24CXX_WriteOneByte(WriteAddr+i,(DataToWrite>>(8*i))&0xff);
	}
}
//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
uint32_t AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{
	uint8_t i;
	uint32_t temp=0;
	for(i=0;i<Len;i++)
	{
		temp<<=8;
		temp += AT24CXX_ReadOneByte(ReadAddr+Len-i-1);
	}
	return temp;
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数					
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr++,*pBuffer);
		pBuffer++;
	}
}
//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.24c02
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
uint8_t AT24CXX_Check(void)  //检查器件
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX  
	if(temp==0X55)return 0;//0101_0101		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;
}
