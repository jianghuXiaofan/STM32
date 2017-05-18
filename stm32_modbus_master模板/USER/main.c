#include "stm32f10x.h"
#include "led.h"
#include "timer.h"
#include "bsp_usart.h"

#include "bsp_usart3.h"
#include "bsp_modbus.h"
#include "bsp_timer4.h"
#include "string.h"
int main(void)
{   
    uint32_t slaveraddr = 0x02;
    uint16_t data_save[20];
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    Usart3_Init(9600);//modbus 9600
    TIM4_Int_Init(60,7199); //6ms 
    
    for( ;; )
    {
        memset(data_save,0,sizeof(data_save));
        //读取从机寄存器值03指令
        modbus_master_read(slaveraddr,0,1,data_save);
        LED0(ON);
        delay_ms(1000);
        delay_ms(1000);
        LED0(OFF);
        delay_ms(1000);
    }
}
















