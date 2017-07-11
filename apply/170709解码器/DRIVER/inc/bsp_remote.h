#ifndef __BSP_REMOTE_H
#define __BSP_REMOTE_H 
#include "stm32f10x.h"   

#define REMOTE_CLK      RCC_APB2Periph_GPIOA
#define REMOTE_PIN      GPIO_Pin_1
#define REMOTE_PORT     GPIOA
 

#define RDATA 	GPIO_ReadInputDataBit(REMOTE_PORT,REMOTE_PIN)	 	//�������������

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;			//�������µĴ���

void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	    
#endif















