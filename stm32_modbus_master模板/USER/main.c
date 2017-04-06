#include "stm32f10x.h"
#include "led.h"
#include "timer.h"
#include "bsp_timer4.h"
#include "bsp_usart.h"
#include "bsp_usart3.h"
#include "bsp_modbus.h"

int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    Usart3_Init(9600);//modbus 9600
    TIM4_Int_Init(60,7199); //6ms 
    for( ;; )
    {
        LED0(ON);
        modbus_rtu_dy();
        
        delay_ms(1000);
        LED0(OFF);
        delay_ms(1000);
    }
    //读取第一个从机的 保存到结构体 
        //读取第二个从机的，保存到结构体
        //读取第三个从机的，三个寄存器的值保存到结构体
//    rtu_read_hldreg(slaveraddr,sendBuf2,0,1);
//    rtu_data_anlys(data_save,USART3_RX_BUF,0,12);
   
}
















