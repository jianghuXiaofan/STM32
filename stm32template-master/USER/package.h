#ifndef __PACKAGE_H
#define __PACKAGE_H
#include "stm32f10x.h"

//����ṹ��������е���Ϣ �����豸�� id heart 
typedef struct 
{
    uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
    
    
}MsgPack;


#endif

