#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_oled_spi.h"
#include "bsp_rtc.h"
#include "bsp_ds18b20.h"
#include "bsp_remote.h"

struct _TIMS calender={0};//全局结构体
int main(void)
{   
    uint16_t temp=0;
    uint16_t count = 0;
    short tem_int=0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    //led_Init();
    Usart1_Init(115200);
    OLED_Init();
    RTC_Init();
    if(DS18B20_Init())
    {
        printf("ds18b20 error\n");
        OLED_ShowChar(56,0,'-',16);
        for(;;);
    }
    delay_ms(400);
    Remote_Init();
    temp=calender.sec;
    for( ;; )
    {
        OLED_ShowString(12,4,"remote_value:",16);	 
        OLED_ShowNum(50,6,Remote_Scan(),3,16);
        if(temp!=calender.sec)
        {
            temp=calender.sec;
            OLED_ShowNum(0,0,calender.year,4,16);
            OLED_ShowChar(32,0,'-',16);
            OLED_ShowNum(40,0,calender.month,2,16);
            OLED_ShowChar(56,0,'-',16);
            OLED_ShowNum(64,0,calender.day,2,16);
            
            OLED_ShowNum(0,2,calender.hour,2,16);
            OLED_ShowChar(16,2,':',16);
            OLED_ShowNum(24,2,calender.min,2,16);
            OLED_ShowChar(40,2,':',16);
            OLED_ShowNum(48,2,calender.sec,2,16);
        }
        if(count >= 100)
        {
            TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC2,DISABLE); //加上才能关掉
            tem_int = DS18B20_Get_Temp();
            if(tem_int>=0)
            {
                if(tem_int>500) OLED_ShowNum(0,6,tem_int,5,16);//为了测试为什么有时候温度值特别大，remote中断影响了温度传感器的时序
                OLED_ShowNum(64,2,(tem_int/10),3,16);
                OLED_ShowChar(88,2,'.',16);
                OLED_ShowNum(96,2,(tem_int%10),1,16);
                OLED_ShowString(104,2,"C",16);
            }
            else
            {
                OLED_ShowChar(64,2,'-',16);
                OLED_ShowNum(72,2,((tem_int*(-1))/10),2,16);
                OLED_ShowChar(88,2,'.',16);
                OLED_ShowNum(96,2,((tem_int*(-1))%10),1,16);
                OLED_ShowString(104,2,"C",16); 
            }                     
            TIM_ClearFlag(TIM5,TIM_FLAG_Update);              //清除标志位。定时器一打开便产生更新事件，若不清除，将会进入中断
            TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);           
            count = 0;
        }
        count++;
        delay_ms(10);
    }
}
















