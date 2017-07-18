#ifndef __BSP_595_LED_H
#define __BSP_595_LED_H

#include "stm32f10x.h"


#define DIO_595_CLK      RCC_APB2Periph_GPIOC
#define DIO_595_PIN      GPIO_Pin_0
#define DIO_595_PORT     GPIOC

#define RCLK_595_CLK      RCC_APB2Periph_GPIOC
#define RCLK_595_PIN      GPIO_Pin_3
#define RCLK_595_PORT     GPIOC

#define SCLK_595_CLK      RCC_APB2Periph_GPIOC
#define SCLK_595_PIN      GPIO_Pin_2
#define SCLK_595_PORT     GPIOC

#define DIO_595_VAL(n)   if(n)   GPIO_SetBits(DIO_595_PORT,DIO_595_PIN); \
                    else \
                        GPIO_ResetBits(DIO_595_PORT,DIO_595_PIN)
                    
#define RCLK_595_VAL(n)   if(n)   GPIO_SetBits(RCLK_595_PORT,RCLK_595_PIN); \
                    else \
                        GPIO_ResetBits(RCLK_595_PORT,RCLK_595_PIN)
                    
#define SCLK_595_VAL(n)   if(n)   GPIO_SetBits(SCLK_595_PORT,SCLK_595_PIN); \
                    else \
                        GPIO_ResetBits(SCLK_595_PORT,SCLK_595_PIN)                    
                    
void led_595_Init(void);
void led_595_data(uint8_t send_addr,uint8_t send_data);
void led_display_num(uint32_t num);

                    
#endif
                    
                    