#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_malloc.h"
#include "bsp_link.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//÷–∂œ◊È…Ë÷√
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    mem_init();
    
    char * head = NULL;
    head = malloc(32);
    printf("%p\n",head);
    if(NULL == head)
    {
        printf("error malloc\n");
    }
    memset(head,1,32);
    for( ;; )
    {
        delay_ms(1000);
        printf("malloc OK\n");
        delay_ms(1000);
    }
}
















