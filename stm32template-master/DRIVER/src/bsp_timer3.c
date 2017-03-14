#include "bsp_timer3.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "package.h"

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
    
    TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
    //TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM3, DISABLE); 
}

//定时器中断 

void TIM3_IRQHandler(void)
{
    int len = 0;
    int rc = 0;
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        //send ping heart package

        char buf[20]="\0"; //存放mqtt发送信息
        int buflen = sizeof(buf);
        len = MQTTSerialize_pingreq((unsigned char*)buf, buflen);
        rc = transport_sendPacketBuffer((unsigned char*)buf, len);
       
    }
}

