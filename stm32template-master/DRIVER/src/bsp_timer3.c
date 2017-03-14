#include "bsp_timer3.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "package.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "send_data.h"
#include "timer.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
    
    //TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM3, DISABLE); 
}

//��ʱ���ж� 
int TIM3_count = 0;
volatile int Send_Sensor_Data_Flag = 0;
void TIM3_IRQHandler(void)
{
    int len = 0;
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

        if(TIM3_count>5)//����Ϣ
        {
            TIM3_count = 0;
            Send_Sensor_Data_Flag = 1;
        }
        else if(TIM3_count == 2)
        {   //send ping heart package
            char buf[20]="\0"; //���mqtt������Ϣ
            int buflen = sizeof(buf);
            len = MQTTSerialize_pingreq((unsigned char*)buf, buflen);
            transport_sendPacketBuffer((unsigned char*)buf, len);
        }
        TIM3_count++; 
    }
}

