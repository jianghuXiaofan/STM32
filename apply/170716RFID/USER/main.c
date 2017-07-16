#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_oled_spi.h"
#include "bsp_rtc.h"
#include "bsp_ds18b20.h"
#include "bsp_remote.h"
#include "bsp_RFID.h"

struct _TIMS calender={0};//全局结构体
char Open_flag = 0;
int main(void)
{   
    uint16_t temp=0;
    uint16_t count = 0;
    short tem_int=0;
    
    char cStr[30];
    u8 ucArray_ID[4]; //先后存放IC卡的类型和UID(IC卡序列号)
    u8 ucStatusReturn; //返回状态
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    //led_Init(); 
    Usart1_Init(115200);
    OLED_Init();
    RTC_Init();
    
    

    RFID_Init();
	PcdReset();
	M500PcdConfigISOType('A');//设置工作方式
    
    if(DS18B20_Init())
    {
        printf("ds18b20 error\n");
        OLED_ShowChar(56,0,'-',16);
        for(;;);
    }
    delay_ms(400);
    //Remote_Init();
    temp=calender.sec;
    
    
    for( ;; )
    { 
		if ((ucStatusReturn = PcdRequest(PICC_REQALL,ucArray_ID)) != MI_OK )//寻卡
			ucStatusReturn = PcdRequest(PICC_REQALL, ucArray_ID);//若失败再次寻卡

        if (ucStatusReturn == MI_OK)
        {
            if (PcdAnticoll(ucArray_ID) == MI_OK)//防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）
            {
                sprintf(cStr,"%02X%02X%02X%02X",ucArray_ID[0],ucArray_ID[1],ucArray_ID[2],ucArray_ID[3]);
                printf ( "The Card_id: %s\r\n",cStr ); 
                OLED_ShowString(8,4,"The Card_id:",16);
                OLED_ShowString(32,6,(u8*)cStr,16);
            }
        }

        //OLED_ShowString(12,4,"remote_value:",16);        
        //OLED_ShowNum(50,6,Remote_Scan(),3,16);
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
            if((calender.hour==1)||(calender.hour==3)||(calender.hour==6))
            {
                Open_flag = 1;
            }
            else Open_flag=0;
        }
        
        if(count >= 3)//大约1.5秒
        {
            //TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC2,DISABLE); //加上才能关掉
            tem_int = DS18B20_Get_Temp();
            if(tem_int>=0)
            {
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
            //TIM_ClearFlag(TIM5,TIM_FLAG_Update);              //清除标志位。定时器一打开便产生更新事件，若不清除，将会进入中断
            //TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);           
            count = 0;
            
        }
        count++;
        
        //delay_ms(10);
    }
}
















