#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_oled.h"


int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//÷–∂œ◊È…Ë÷√
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    OLED_Init();
    for( ;; )
    {
        OLED_ShowString(0,0,"jianghuxiaofan",8);
        OLED_ShowString(0,1,"jianghuxiaofan",8);  
        OLED_ShowNum(0,2,1024,4,16);
        OLED_ShowNum(0,4,1024,4,16);

        delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
    }
}
















