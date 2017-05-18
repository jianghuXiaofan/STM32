#ifndef __HX711AD_H
#define __HX711AD_H
#include "stm32f10x.h"

#define HX711Sck_CLK    RCC_APB2Periph_GPIOA
#define HX711Sck_Pin    GPIO_Pin_2
#define HX711Sck_Port   GPIOA


#define HX711Dat_CLK    RCC_APB2Periph_GPIOA
#define HX711Dat_Pin    GPIO_Pin_3
#define HX711Dat_Port   GPIOA

#define HX711_SCK_H()   GPIO_SetBits(HX711Sck_Port,HX711Sck_Pin)
#define HX711_SCK_L()   GPIO_ResetBits(HX711Sck_Port,HX711Sck_Pin)

#define HX711_DAT()     GPIO_ReadInputDataBit(HX711Dat_Port,HX711Dat_Pin)


void HX711ad_Init(void);
int Read_Weight(char mode);
#endif

