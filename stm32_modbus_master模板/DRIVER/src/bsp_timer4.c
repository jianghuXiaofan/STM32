#include "bsp_timer4.h"

volatile u8 modbus_com2_over = 0; //发送完成标志

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
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    //TIM_Cmd(TIM4, ENABLE); 
}

void TIM4_Enable(void)
{
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    TIM_SetCounter(TIM4,0x00);
    TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
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
        //改变标志位
        modbus_com2_over = 2;
    }       
}


