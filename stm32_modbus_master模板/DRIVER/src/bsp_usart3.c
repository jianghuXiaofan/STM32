#include "bsp_usart3.h"
#include "bsp_timer4.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////

  	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u8 receCount2;//接收计数
u8 sendCount2;//发送计数	 
u8 sendBuf2[32];//存放要发送的信息

u8 sendPosi2;//发送信号量
u8 checkoutError2;//错误情况


////接收到的数据长度
//u8 RS485_RX_CNT=0; 

//串口3的初始化 baudrate 是波特率
void Usart3_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//端口配置
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    

	USART_InitStructure.USART_BaudRate = baudrate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
    //USART_InitStructure.USART_Parity = USART_Parity_Odd;//奇校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);
    
     //USART3接收中断组和中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//接收中断
    USART_ClearFlag(USART3, USART_FLAG_TC);		  	//清零发送置位标准!!
    USART_ITConfig(USART3,USART_IT_TC,ENABLE);//开启发送中断？？
	USART_Cmd(USART3,ENABLE);
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOA时钟 485控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PA8端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
    RS485_TX_EN(1);//默认接收为0
}

void beginSend3(void)
{
    RS485_TX_EN(1);
    sendPosi2 = 0; //发送信号量
    if(sendCount2 > 1) sendCount2--;
    USART_SendData(USART3,sendBuf2[0]);//0 是一个地址
}


    
//接收中断函数 485
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
        //USART_ClearITPendingBit(USART3,USART_IT_RXNE);//不能加
        USART3_RX_BUF[receCount2] = USART_ReceiveData(USART3);
        printf( "%02x ", USART3_RX_BUF[receCount2] );    //将接受到的数据直接返回打印
	    receCount2++;          //接收地址偏移寄存器加1
        TIM4_Enable();//启动超时        
	}
    else if(USART_GetITStatus(USART3,USART_IT_TC) != RESET)//发送中断
    {
        USART_ClearFlag(USART3,USART_FLAG_TC);
        if(sendPosi2 < sendCount2)
	    {
	        sendPosi2++;//发送信号量
	        USART_SendData(USART3,sendBuf2[sendPosi2]);//发送的是modbus master的命令
	    }
	    else
	    {
	        RS485_TX_EN(0);    //发送完后将485置于接收状态
	        receCount2 = 0;   //清接收地址偏移寄存器
	        checkoutError2 = 0; //错误情况
	    } 
    }        
}










//	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//设置为接收模式	
//}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
//void RS485_Receive_Data(u8 *buf,u8 *len)
//{
//	u8 rxlen=RS485_RX_CNT;
//	u8 i=0;
//	*len=0;				//默认为0
//	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
//	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=RS485_RX_BUF[i];	
//		}		
//		*len=RS485_RX_CNT;	//记录本次数据长度
//		RS485_RX_CNT=0;		//清零
//	}
//}


void USART3_Putc(unsigned char c)
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
	USART_SendData(USART3, c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

//串口3 发送字符串 
void usart3_send_nbyte(unsigned char *str, int len)
{
	int i;
    RS485_TX_EN(1);
	for(i = 0; i < len; i++)
	{
		USART3_Putc(*str++);
	}
    RS485_TX_EN(0);
}

