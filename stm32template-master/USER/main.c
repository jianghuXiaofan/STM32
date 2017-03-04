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

//实现目的 读取DHT11的温湿度 JSON格式经过MQTT发送

int main(void)
{
	//DHT11_Data_TypeDef DHT11_Data = {0,0,0,0,0};
    MsgPack pack; //用于存放所有数据类型的信息
    memset(&pack,0,sizeof(pack));
    
	SysTick_Init();
	led_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
	NVIC_Configuration();//中断组设置
    Dht11_Init();//DHT11温湿度传感器初始化
    relayInit();//继电器初始化
      
	for(;;)
	{		
        if(1 == Read_DHT11((DHT11_Data_TypeDef*)(&pack))) //if(1 == Read_DHT11(&DHT11_Data))
		{      
          	printf("湿度为%d.%d 温度为 %d.%d\n",
					pack.humi_int,
					pack.humi_deci,
					pack.temp_int,
					pack.temp_deci);
		}
		else 
            printf("读取错误\n");
        
        Send_pack(&pack);//MQTT打包发送数据
        
		LED0(ON);
        Relay(ON);//继电器打开
		delay_ms(1000);
		LED0(OFF);
        //Relay(OFF);
		delay_ms(1000);
	}
}












