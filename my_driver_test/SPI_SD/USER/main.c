#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_spi.h"
#include "bsp_MMC_SD.h"
#include "stdlib.h"
#include "string.h"
//malloc ��Ҫ��ѡ use microlib 
//�޸� �����ļ� .s���ļ�  ����512 Heap_Size     EQU   0x00002000
int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж�������
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    SPI1_Init();
    while(SD_Initialize())
    {
        printf("SD Error\n");
        delay_ms(500);
    }
    int sd_size=SD_GetSectorCount();//�õ�������
    printf("SD Card Size:%dMB\n",sd_size>>11);
    delay_ms(1000);
    u8 *buf = NULL;
	u16 i;
	buf=malloc(512);				//�����ڴ�
    memset(buf,1,512);
    
    if(NULL != buf)
    {
        if(SD_ReadDisk(buf,0,1)==0)	//��ȡ0����������
        {
            for(i=0;i<512;i++)
                printf("%x ",buf[i]);//��ӡsec��������
        }  
    }
    free(buf);
    for( ;; )
    {
        delay_ms(1000);
        printf("run\n");
        delay_ms(1000);
    }
}
















