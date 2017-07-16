#include "bsp_rtc.h"
#include <string.h>
//#include "time.h"


const u8 *COMPILED_DATE=__DATE__;//获得编译日期
const u8 *COMPILED_TIME=__TIME__;//获得编译时间

const u8 Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
//自动设置时间为编译器时间
static void Auto_Time_Set(void)
{
    u8 temp[3];
    u8 i;
    u8 mon,date;
    u16 year;
    u8 sec,min,hour;
    for(i=0;i<3;i++)temp[i]=COMPILED_DATE[i];
    for(i=0;i<12;i++)if(!strncmp((void*)Month_Tab[i],(void*)temp,3))break;//str_cmpx((u8*)Month_Tab[i],temp,3)
    mon=i+1;//得到月份
    if(COMPILED_DATE[4]==' ')date=COMPILED_DATE[5]-'0';
    else date=10*(COMPILED_DATE[4]-'0')+COMPILED_DATE[5]-'0';
    year=1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')+10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';
    hour=10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';
    min=10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';
    sec=10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';
    RTC_Set(year,mon,date,hour,min,sec)	;
}
static void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void delay_ms(uint32_t tim)
{
    uint32_t i,j;
    for(i=tim;i>0;i--)
        for(j=1000;j>0;j--)
            ;
}

uint8_t RTC_Init(void)
{
    uint8_t time_out = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    if(BKP_ReadBackupRegister(BKP_DR1) != 0x5050)
    {
        BKP_DeInit();
        RCC_LSEConfig(RCC_LSE_ON);
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            time_out++;
            delay_ms(10);
            if(time_out>=200)
                return 1;//晶振没启动
        }
        
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC,ENABLE);
        RTC_WaitForLastTask();
        RTC_EnterConfigMode();
        RTC_SetPrescaler(32767);
        RTC_WaitForLastTask();
        //RTC_set();set times
        Auto_Time_Set();
        //RTC_Set(2017,7,9,21,40,20);
        RTC_ExitConfigMode();
        BKP_WriteBackupRegister(BKP_DR1,0x5050);
    }
    else
    {
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC,ENABLE);
        RTC_WaitForLastTask();
    }
    RTC_NVIC_Config();
    RTC_get();//get times
    return 0;
}

void RTC_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        RTC_get();
    }
    if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
    RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);//?
    RTC_WaitForLastTask();
}
//非整百年 除4无余 ，整百年 除400无余
static uint8_t Is_Leap_Year(uint16_t year)
{
    if(((year%4==0)&&(year%100!=0))||(year%400==0))
        return 1;
    return 0;
}
//月份数据表
uint8_t table_week[12] = {0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

uint8_t RTC_get(void)
{
    static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		calender.year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calender.year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		calender.month=temp1+1;	//得到月份
		calender.day=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	calender.hour=temp/3600;     	//小时
	calender.min=(temp%3600)/60; 	//分钟	
	calender.sec=(temp%3600)%60; 	//秒钟
	//calender.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期
    return 0;    
}

//1970.1.1为基准
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	RTC_get();
	return 0;	    
}


/* //rtc_demo
struct _TIMS calender={0};//全局结构体
int main(void)
{   
    uint16_t temp=0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    //led_Init();
    Usart1_Init(115200);
    OLED_Init();
    RTC_Init();
    for( ;; )
    {
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
        delay_ms(10);
    }
}
*/



















