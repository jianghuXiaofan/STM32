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
#include "control.h"

extern  u8 USART2_RX_BUF[USART_REC_LEN];    //接收缓冲,最大USART_REC_LEN个字节.
volatile u16 rx_buf_len = 0;        //串口2 接收FIFO计数值
volatile u16 USART2_RX_OK_FLAG=0;   //接收完成标志
extern volatile int Send_Sensor_Data_Flag;//
//实现目的 读取DHT11的温湿度 JSON格式经过MQTT发送到服务器
//从服务器订阅消息 解析执行响应相应的行为
//基本框架版本

int main(void)
{
    MsgPack pack; //用于存放所有数据类型的信息
    memset(&pack,0,sizeof(pack));
   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
	SysTick_Init();
	led_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
    Dht11_Init();   //DHT11温湿度传感器初始化
    relayInit();    //继电器初始化     
    TIM2_Int_Init(999,7199);    //10Khz的计数频率，计数到500为50ms
    TIM3_Int_Init(4999,7199);   //10Khz的计数频率，计数到5000为500ms//在成功建立连接以后再使能
    delay_ms(200);
    
    int ret_conn = MQTT_Connect();
    while(ret_conn != SUCCESS);//要处理连不上的情况
    int ret_sub = MQTT_Subscribe();
    while(ret_sub != SUCCESS);    
    TIM_Cmd(TIM3, ENABLE);
    
	for(;;)
	{	        
        //Send_pack(&pack);//MQTT打包发送数据 测试用
        getdata();//接收数据处理
        
        //这里可以干其他的事情
        if(Send_Sensor_Data_Flag == 1) {
            Send_Sensor_Data_Flag = 0;
            //pack
            if(SUCCESS == Read_DHT11((DHT11_Data_TypeDef *)&pack))
            {
                Send_pack_data(&pack);
            }
            else
                printf("read DHT11 error\n");
        }
	}
}








