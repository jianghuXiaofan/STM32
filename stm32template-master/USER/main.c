#include "stm32f10x.h"
#include "timer.h"
#include "led.h"
#include "bsp_usart.h"
#include "bsp_usart2.h"
#include "bsp_dht11.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "package.h"
#include "send_data.h"
#include "bsp_relay.h"
#include "receive_data.h"
#include "bsp_timer2.h"
#include "bsp_timer3.h"
extern volatile u8 USART2_RX_BUF[USART_REC_LEN];    //���ջ���,���USART_REC_LEN���ֽ�.
volatile u16 rx_buf_len = 0;        //����2 ����FIFO����ֵ
volatile u16 USART2_RX_OK_FLAG=0;   //������ɱ�־

//ʵ��Ŀ�� ��ȡDHT11����ʪ�� JSON��ʽ����MQTT���͵�������
//�ӷ�����������Ϣ ����ִ����Ӧ��Ӧ����Ϊ
//ĿǰСbug ΪʲôҪ�ֶ���λһ�£�Ҫ��
int main(void)
{
	//DHT11_Data_TypeDef DHT11_Data = {0,0,0,0,0};
    MsgPack pack; //���ڴ�������������͵���Ϣ
    memset(&pack,0,sizeof(pack));
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж�������
	SysTick_Init();
    
	led_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
    Dht11_Init();//DHT11��ʪ�ȴ�������ʼ��
    relayInit();//�̵�����ʼ��     
    TIM2_Int_Init(999,7199);//10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms
    TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
    delay_ms(500);
	for(;;)
	{	        
//        if(1 == Read_DHT11((DHT11_Data_TypeDef*)(&pack))) //if(1 == Read_DHT11(&DHT11_Data))
//		{      
//		}
//		else 
//            printf("��ȡ����\n");
        
        //Send_pack(&pack);//MQTT�����������
        getdata();//�������ݴ���
		LED0(ON);
        Relay(ON);//�̵�����
		delay_ms(500);
		LED0(OFF);
        //Relay(OFF);
		delay_ms(500);
	}
}



//          	printf("ʪ��Ϊ%d.%d �¶�Ϊ %d.%d\n",
//					pack.humi_int,
//					pack.humi_deci,
//					pack.temp_int,
//					pack.temp_deci);










