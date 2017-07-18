#include "bsp_595_led.h"
#include "bsp_systick.h"



void led_595_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DIO_595_CLK|RCLK_595_CLK|SCLK_595_CLK, ENABLE);	 //ʹ�ܶ˿�ʱ��
    
	GPIO_InitStructure.GPIO_Pin = DIO_595_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    //�ٶ�2MHz
 	GPIO_Init(DIO_595_PORT, &GPIO_InitStructure);	  //��ʼ��

	
    GPIO_InitStructure.GPIO_Pin = RCLK_595_PIN;	 
 	GPIO_Init(RCLK_595_PORT, &GPIO_InitStructure);	  

    
    GPIO_InitStructure.GPIO_Pin = SCLK_595_PIN;	 
 	GPIO_Init(SCLK_595_PORT, &GPIO_InitStructure);	  
}


void led_595_data(uint8_t send_addr,uint8_t send_data)
{
    unsigned char seg[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xFF};//��������
    unsigned char no[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//λ��
    uint8_t i;
    for(i=0;i<8;i++)
    {
        
        if(no[send_addr]&0x80)
            DIO_595_VAL(1);
        else
            DIO_595_VAL(0);
        no[send_addr] <<=1;
        SCLK_595_VAL(0);
        SCLK_595_VAL(1);
    }
    
    for(i=0;i<8;i++)
    {
        
        if(seg[send_data]&0x80)
            DIO_595_VAL(1);
        else
            DIO_595_VAL(0);
        seg[send_data]<<=1;
        SCLK_595_VAL(0);
        SCLK_595_VAL(1);
                
    }  
    RCLK_595_VAL(0);
    RCLK_595_VAL(1);  
}

//��ʾ���ֵ�����ܸ�λΪ0 ����ʾ
void led_display_num(uint32_t num)
{
    char i = 0;
    for(i=0;i<8;i++)
    {   
        led_595_data(7-i,num%10);
        num = num /10;
        if(num == 0) break;
    } 
}

