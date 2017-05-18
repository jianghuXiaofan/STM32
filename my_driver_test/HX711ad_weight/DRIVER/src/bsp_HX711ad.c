#include "bsp_HX711ad.h"

void HX711ad_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(HX711Sck_CLK | HX711Dat_CLK,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = HX711Sck_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(HX711Sck_Port,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = HX711Dat_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(HX711Dat_Port,&GPIO_InitStructure);
    
}


int Read_Weight(char mode)
{
    unsigned char i;
    uint32_t value = 0;
    
    HX711_SCK_L();
    while(HX711_DAT());
    for(i=0;i<24;i++)
    {    
        HX711_SCK_H();
        value =value<<0x01;
        HX711_SCK_L();
        if(HX711_DAT()) value|=0x01;
    }
    
    HX711_SCK_H();
    //value = value ^ 0x80000000;
    HX711_SCK_L();
    return (value);
}

