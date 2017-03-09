#include "bsp_usart2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bsp_timer2.h"
/////////////////////////////////////////////////////////

  	
volatile u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
extern volatile u16 rx_buf_len;               //����2 ����FIFO����ֵ


//����2�ĳ�ʼ�� baudrate �ǲ�����
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�˿�����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    //USART2�����ж�����ж����ȼ�
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
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�����ж�
	USART_Cmd(USART2,ENABLE);
    USART_GetFlagStatus(USART2, USART_FLAG_TC);     
}

//�����жϺ��� ������ʾ 
//ʵ�ֹ��ܣ� ÿ�ν����ж϶��������������ֵ  
//һ�������趨ʱ��û���յ����ݾ�Ĭ���ǽ�������ˡ�
void USART2_IRQHandler(void)
{   
  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{   
        TIM_Cmd(TIM2, DISABLE);
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
        USART2_RX_BUF[rx_buf_len] = USART_ReceiveData(USART2);
        rx_buf_len++; //����2����FIFO����ֵ��1
        
        TIM_SetCounter(TIM2, 0);
        TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update,
		ENABLE  //ʹ��
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
//����2 �����ַ��� 
void usart2_send_nbyte(unsigned char *str, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		USART2_Putc(*str++);
	}
}



