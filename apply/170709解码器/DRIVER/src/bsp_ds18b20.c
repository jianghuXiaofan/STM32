#include "bsp_ds18b20.h"
#include "bsp_systick.h"	



//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);	 //ʹ��PORTA��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;				//PORTA0 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);

 	DS18B20_DQ_Set();    //���1

	DS18B20_Rst();

	return DS18B20_Check();
}  

static void DS18B20_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DS18B20_PORT,&GPIO_InitStructure);
}

static void DS18B20_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DS18B20_PORT,&GPIO_InitStructure);
}

//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_Crl();  //����DQ
    delay_us(750);    //����750us
    DS18B20_DQ_Set(); //DQ=1 
	delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN()&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN()&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
	DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_Crl();  
	delay_us(2);
    DS18B20_DQ_Set();  
	DS18B20_IO_IN();//SET PA0 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN())data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)     
{             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_Crl(); // Write 1
            delay_us(2);                            
            DS18B20_DQ_Set(); 
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_Crl(); // Write 0
            delay_us(60);             
            DS18B20_DQ_Set(); 
            delay_us(2);                          
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 

//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(float)tem*0.625;//ת��     
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
}

/* ds18b20_demo
int main(void)
{   
    uint16_t temp=0;
    uint16_t count = 0;
    short tem_int=0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж�������
    SysTick_Init();    
    Usart1_Init(115200);
    OLED_Init();
    RTC_Init();
    if(DS18B20_Init())
    {
        printf("ds18b20 error\n");
        OLED_ShowChar(56,0,'-',16);
        for(;;);
    }
    for( ;; )
    {
        if(count >= 50)
        {
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
            count = 0;
        }
        count++;
        delay_ms(10);
    }
}

*/



