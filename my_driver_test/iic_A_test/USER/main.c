#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_iic_A.h"
#include "bsp_24cxx.h"

/****************************************
    作用：24c02 模拟iic测试
    test: jianghu_xiaofan
****************************************/
int main(void)
{   
    uint8_t buff[20]="";
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    AT24CXX_Init();
    while(AT24CXX_Check())
    {
        printf("AT24c02 error\n");
    }
    printf("eeprom 24c02 test\n");
    for( ;; )
    {
        AT24CXX_Write(3,"saf",11);
        delay_ms(1000);
        delay_ms(1000);
        AT24CXX_Read(0,buff,10);
        printf("read %s\n",buff);
        delay_ms(1000);
        delay_ms(1000);
    }
}
















