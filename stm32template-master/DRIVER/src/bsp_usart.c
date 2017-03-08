#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



//串口1的初始化 baudrate 是波特率
void Usart1_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//端口配置
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudrate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
    NVIC_InitTypeDef NVIC_InitStructure;
    //USART1接收中断 组和中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//接收中断
	USART_Cmd(USART1,ENABLE);
    USART_GetFlagStatus(USART1, USART_FLAG_TC);
}




/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
		USART_SendData(USART1, (uint8_t) ch);
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
		return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}


//接收中断函数 返回显示
void USART1_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		ch = USART_ReceiveData(USART1);
	  	printf( "%c", ch );    //将接受到的数据直接返回打印
	} 
}







////接收中断函数 返回显示
//void USART2_IRQHandler(void)
//{
//    uint8_t str1 = 0;
//  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  	{   
//         USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
//         str1 = USART_ReceiveData(USART2);
//         while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  
//     //rx_buf_len = (rx_buf_len+1)%USART_REC_LEN;
//  	}
//    
//    USART2_RX_BUF[rx_buf_len] = str1;
//    rx_buf_len++;
//    if(rx_buf_len == USART_REC_LEN)
//    {
//        rx_buf_len = 0;
//        memset(USART2_RX_BUF,0,USART_REC_LEN);
//    }
//}


//u16 str1;
//  	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//  	{
//		 USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//		 //str1=USART_ReceiveData(USART3);
//         USART3_RX_BUF[rx_buf_len] = USART_ReceiveData(USART3);
//		 ++rx_buf_len;
//		 //返回接收数据
//		 //USART_SendData(USART3, str1);
//		 //while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
//		 USART_ITConfig( USART3,USART_IT_RXNE, ENABLE);
//  	}


//	uint8_t ch;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{ 	
//		ch = USART_ReceiveData(USART2);
//	  	printf( "%c", ch );    //将接受到的数据直接返回打印
//	} 


//串口2,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
//void usart2_printf(char* fmt,...)  
//{  
//        u16 i,j; 
//        i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
//        for(j=0;j<i;j++)							//循环发送数据
//        {
//            while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
//            USART_SendData(USART2,USART2_TX_BUF[j]); 
//        } 
//}


//void USART2_IRQHandler(void)
//{
//	u16 str1;   
//    //uint8_t i = 0;
//  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  	{
//     USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//     USART_SendData(USART2, str1);
//     while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
//     USART_ITConfig( USART2,USART_IT_RXNE, ENABLE);
//  	}
//}

	  
  

	





