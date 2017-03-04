#include "bsp_usart.h"


/////////////////////////////////////////////////////////
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 rx_buf_len = 0;
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���



//����1�ĳ�ʼ�� baudrate �ǲ�����
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�˿�����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudrate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����ж�
	USART_Cmd(USART1,ENABLE);
}


//�����ж�������ȼ�
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
    //USART1�����ж� ����ж����ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    //USART2�����ж� ����ж����ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART2_Putc(unsigned char c)
{
	USART_SendData(USART2, c);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void usart2_send_nbyte(unsigned char *str, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		USART2_Putc(*str++);
	}
}

/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
		return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}


//�����жϺ��� ������ʾ
void USART1_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		ch = USART_ReceiveData(USART1);
	  	printf( "%c", ch );    //�����ܵ�������ֱ�ӷ��ش�ӡ
	} 
}

//����2�ĳ�ʼ�� baudrate �ǲ�����
void Usart2_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
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
	
	USART_InitStructure.USART_BaudRate = baudrate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�����ж�
	USART_Cmd(USART2,ENABLE);
}


//�����жϺ��� ������ʾ
void USART2_IRQHandler(void)
{
    uint8_t str1 = 0;
  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{   
     USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
     USART2_RX_BUF[rx_buf_len] = USART_ReceiveData(USART2);
     ++rx_buf_len;   
     //USART_SendData(USART2, str1);
     while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
     
     //rx_buf_len = (rx_buf_len+1)%USART_REC_LEN;
  	}
    
}




//u16 str1;
//  	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//  	{
//		 USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//		 //str1=USART_ReceiveData(USART3);
//         USART3_RX_BUF[rx_buf_len] = USART_ReceiveData(USART3);
//		 ++rx_buf_len;
//		 //���ؽ�������
//		 //USART_SendData(USART3, str1);
//		 //while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
//		 USART_ITConfig( USART3,USART_IT_RXNE, ENABLE);
//  	}


//	uint8_t ch;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{ 	
//		ch = USART_ReceiveData(USART2);
//	  	printf( "%c", ch );    //�����ܵ�������ֱ�ӷ��ش�ӡ
//	} 













//����2,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
//void usart2_printf(char* fmt,...)  
//{  
//        u16 i,j; 
//        i=strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
//        for(j=0;j<i;j++)							//ѭ����������
//        {
//            while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
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

	  
  

	





