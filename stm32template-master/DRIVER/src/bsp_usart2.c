#include "bsp_usart2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bsp_timer2.h"
/////////////////////////////////////////////////////////

  	
volatile u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern volatile u16 rx_buf_len;               //串口2 接收FIFO计数值


//串口2的初始化 baudrate 是波特率
void Usart2_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//端口配置
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    //USART2接收中断组和中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	USART_InitStructure.USART_BaudRate = baudrate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//接收中断
	USART_Cmd(USART2,ENABLE);
    USART_GetFlagStatus(USART2, USART_FLAG_TC);     
}

//接收中断函数 返回显示 
//实现功能： 每次接收中断都会清除计数器的值  
//一旦超过设定时间没有收到数据就默认是接收完成了。
void USART2_IRQHandler(void)
{   
  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{   
        TIM_Cmd(TIM2, DISABLE);
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
        USART2_RX_BUF[rx_buf_len] = USART_ReceiveData(USART2);
        rx_buf_len++; //串口2接收FIFO计数值加1
        
        TIM_SetCounter(TIM2, 0);
        TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update,
		ENABLE  //使能
		);
        TIM_Cmd(TIM2, ENABLE); 
  	}
}


void USART2_Putc(unsigned char c)
{
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );
	USART_SendData(USART2, c);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}
//串口2 发送字符串 
void usart2_send_nbyte(unsigned char *str, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		USART2_Putc(*str++);
	}
}



