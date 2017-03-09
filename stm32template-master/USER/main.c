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
extern volatile u8 USART2_RX_BUF[USART_REC_LEN];    //接收缓冲,最大USART_REC_LEN个字节.
volatile u16 rx_buf_len = 0;        //串口2 接收FIFO计数值
volatile u16 USART2_RX_OK_FLAG=0;   //接收完成标志

//实现目的 读取DHT11的温湿度 JSON格式经过MQTT发送到服务器
//从服务器订阅消息 解析执行响应相应的行为
//目前小bug 为什么要手动复位一下，要改
int main(void)
{
	//DHT11_Data_TypeDef DHT11_Data = {0,0,0,0,0};
    MsgPack pack; //用于存放所有数据类型的信息
    memset(&pack,0,sizeof(pack));
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断组设置
	SysTick_Init();
    
	led_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
    Dht11_Init();//DHT11温湿度传感器初始化
    relayInit();//继电器初始化     
    TIM2_Int_Init(999,7199);//10Khz的计数频率，计数到500为50ms
    TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms
    delay_ms(500);
	for(;;)
	{	        
//        if(1 == Read_DHT11((DHT11_Data_TypeDef*)(&pack))) //if(1 == Read_DHT11(&DHT11_Data))
//		{      
//		}
//		else 
//            printf("读取错误\n");
        
        //Send_pack(&pack);//MQTT打包发送数据
        getdata();//接收数据处理
		LED0(ON);
        Relay(ON);//继电器打开
		delay_ms(500);
		LED0(OFF);
        //Relay(OFF);
		delay_ms(500);
	}
}



//          	printf("湿度为%d.%d 温度为 %d.%d\n",
//					pack.humi_int,
//					pack.humi_deci,
//					pack.temp_int,
//					pack.temp_deci);










