#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_spi_W25Qxx.h"
#include "stdlib.h"
#include "string.h"
//malloc 需要勾选 use microlib 
//修改 启动文件 .s的文件  大于512 Heap_Size     EQU   0x00002000
int main(void)
{   
    char buff[21] = "";
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    SPI_Flash_Init();
    while(SPI_Flash_ReadID()!=W25Q16)
    {
        printf("read deriver is error\r\n");
        delay_ms(1000);
    }
    for( ;; )
    {
        delay_ms(1000);
        SPI_Flash_Write((u8*)"dsadasda",0,20);
        printf("run\n");
        delay_ms(1000);
        SPI_Flash_Read((u8*)buff,0,20);
        printf("Read buff = %s\n",buff);
        delay_ms(1000);
    }
}
















