#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_HX711ad.h"

int main(void)
{   
    int i = 0;
    u32 weight = 0;
    u32 weight_g = 0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    led_Init();
    HX711ad_Init();
    Usart1_Init(115200);
    for( ;; )
    {
        delay_ms(1000);
        for(i = 0;i<10;i++)
        {
            weight += Read_Weight(1);
            delay_ms(10);
        }
        weight /= 10;
        /*
        *   (Akg/3kg)*4.3mv = 1.433A mv
        *   128*1.433A = 183.466A mv
        *   183.466*(2^24)/4.3V/1000 = 715827.8826 所以校正取715.8
        */
        weight_g = (unsigned long)((float)weight/715.883);
        //毛重302克要减去
        printf("value = %d\t weight = %d\r\n",weight,weight_g);
        
        delay_ms(1000);
    }
}
















