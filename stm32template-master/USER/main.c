#include "stm32f10x.h"
#include "timer.h"
#include "led.h"
#include "bsp_usart.h"
#include "bsp_dht11.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "package.h"
#include "send_data.h"
#include "bsp_relay.h"

//ʵ��Ŀ�� ��ȡDHT11����ʪ�� JSON��ʽ����MQTT����

int main(void)
{
	//DHT11_Data_TypeDef DHT11_Data = {0,0,0,0,0};
    MsgPack pack; //���ڴ�������������͵���Ϣ
    memset(&pack,0,sizeof(pack));
    
	SysTick_Init();
	led_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
	NVIC_Configuration();//�ж�������
    Dht11_Init();//DHT11��ʪ�ȴ�������ʼ��
    relayInit();//�̵�����ʼ��
      
	for(;;)
	{		
        if(1 == Read_DHT11((DHT11_Data_TypeDef*)(&pack))) //if(1 == Read_DHT11(&DHT11_Data))
		{      
          	printf("ʪ��Ϊ%d.%d �¶�Ϊ %d.%d\n",
					pack.humi_int,
					pack.humi_deci,
					pack.temp_int,
					pack.temp_deci);
		}
		else 
            printf("��ȡ����\n");
        
        Send_pack(&pack);//MQTT�����������
        
		LED0(ON);
        Relay(ON);//�̵�����
		delay_ms(1000);
		LED0(OFF);
        //Relay(OFF);
		delay_ms(1000);
	}
}












