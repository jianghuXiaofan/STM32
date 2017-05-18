#include "bsp_timer4.h"
#include "bsp_usart3.h"

volatile u8 modbus_com2_over = 0; //发送完成标志

//u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//u8 receCount2;//接收计数
//u8 sendCount2;//发送计数	 
//u8 sendBuf2[32];//存放要发送的信息

//u8 sendPosi2;//发送信号量
//u8 checkoutError2;//错误情况


void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    
    //TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    //TIM_Cmd(TIM4, ENABLE); 
    TIM4_Disable();
}

void TIM4_Enable(void)
{
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    TIM_SetCounter(TIM4,0x00);
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//
    TIM_Cmd(TIM4,ENABLE);
}

void TIM4_Disable(void)
{
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,DISABLE);
    TIM_Cmd(TIM4,DISABLE);//失能TIMx外设
}


void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
        TIM4_Disable();
        RS485_TX_EN(1);
        //改变标志位
        modbus_com2_over = 2;
    }       
}


