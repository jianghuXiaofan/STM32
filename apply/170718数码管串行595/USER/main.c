#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_rtc.h"
#include "bsp_595_led.h"

struct _TIMS calender={0};//全局结构体

int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    Usart1_Init(115200);
    RTC_Init();
    led_595_Init();
    
    for( ;; )
    { 
        //led_display_num(4534);
       led_595_data(7,calender.sec%10);
       led_595_data(6,calender.sec/10);
       led_595_data(5,calender.min%10);
       led_595_data(4,calender.min/10);
       led_595_data(3,calender.hour%10);
       led_595_data(2,calender.hour/10);
       led_595_data(1,calender.day%10);
       led_595_data(0,calender.day/10);
    }
}
















