#include "bsp_usart3.h"
#include "bsp_timer4.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////

  	
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u8 receCount2;//���ռ���
u8 sendCount2;//���ͼ���	 
u8 sendBuf2[32];//���Ҫ���͵���Ϣ

u8 sendPosi2;//�����ź���
u8 checkoutError2;//�������


////���յ������ݳ���
//u8 RS485_RX_CNT=0; 

//����3�ĳ�ʼ�� baudrate �ǲ�����
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�˿�����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    

	USART_InitStructure.USART_BaudRate = baudrate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
    //USART_InitStructure.USART_Parity = USART_Parity_Odd;//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);
    
     //USART3�����ж�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�����ж�
    USART_ClearFlag(USART3, USART_FLAG_TC);		  	//���㷢����λ��׼!!
    USART_ITConfig(USART3,USART_IT_TC,ENABLE);//���������жϣ���
	USART_Cmd(USART3,ENABLE);
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ�� 485��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PA8�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
    RS485_TX_EN(1);//Ĭ�Ͻ���Ϊ0
}

void beginSend3(void)
{
    RS485_TX_EN(1);
    sendPosi2 = 0; //�����ź���
    if(sendCount2 > 1) sendCount2--;
    USART_SendData(USART3,sendBuf2[0]);//0 ��һ����ַ
}


    
//�����жϺ��� 485
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
        //USART_ClearITPendingBit(USART3,USART_IT_RXNE);//���ܼ�
        USART3_RX_BUF[receCount2] = USART_ReceiveData(USART3);
        printf( "%02x ", USART3_RX_BUF[receCount2] );    //�����ܵ�������ֱ�ӷ��ش�ӡ
	    receCount2++;          //���յ�ַƫ�ƼĴ�����1
        TIM4_Enable();//������ʱ        
	}
    else if(USART_GetITStatus(USART3,USART_IT_TC) != RESET)//�����ж�
    {
        USART_ClearFlag(USART3,USART_FLAG_TC);
        if(sendPosi2 < sendCount2)
	    {
	        sendPosi2++;//�����ź���
	        USART_SendData(USART3,sendBuf2[sendPosi2]);//���͵���modbus master������
	    }
	    else
	    {
	        RS485_TX_EN(0);    //�������485���ڽ���״̬
	        receCount2 = 0;   //����յ�ַƫ�ƼĴ���
	        checkoutError2 = 0; //�������
	    } 
    }        
}










//	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//����Ϊ����ģʽ	
//}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
//void RS485_Receive_Data(u8 *buf,u8 *len)
//{
//	u8 rxlen=RS485_RX_CNT;
//	u8 i=0;
//	*len=0;				//Ĭ��Ϊ0
//	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
//	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=RS485_RX_BUF[i];	
//		}		
//		*len=RS485_RX_CNT;	//��¼�������ݳ���
//		RS485_RX_CNT=0;		//����
//	}
//}


void USART3_Putc(unsigned char c)
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET );
	USART_SendData(USART3, c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

//����3 �����ַ��� 
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

