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

extern  u8 USART2_RX_BUF[USART_REC_LEN];    //���ջ���,���USART_REC_LEN���ֽ�.
volatile u16 rx_buf_len = 0;        //����2 ����FIFO����ֵ
volatile u16 USART2_RX_OK_FLAG=0;   //������ɱ�־
extern volatile int Send_Sensor_Data_Flag;//
//ʵ��Ŀ�� ��ȡDHT11����ʪ�� JSON��ʽ����MQTT���͵�������
//�ӷ�����������Ϣ ����ִ����Ӧ��Ӧ����Ϊ
//������ܰ汾

int main(void)
{
    MsgPack pack; //���ڴ�������������͵���Ϣ
    memset(&pack,0,sizeof(pack));
   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж�������
	SysTick_Init();
	led_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
    Dht11_Init();   //DHT11��ʪ�ȴ�������ʼ��
    relayInit();    //�̵�����ʼ��     
    TIM2_Int_Init(999,7199);    //10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms
    TIM3_Int_Init(4999,7199);   //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms//�ڳɹ����������Ժ���ʹ��
    delay_ms(200);
    
    int ret_conn = MQTT_Connect();
    while(ret_conn != SUCCESS);//Ҫ���������ϵ����
    int ret_sub = MQTT_Subscribe();
    while(ret_sub != SUCCESS);    
    TIM_Cmd(TIM3, ENABLE);
    
	for(;;)
	{	        
        //Send_pack(&pack);//MQTT����������� ������
        getdata();//�������ݴ���
        
        //������Ը�����������
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








