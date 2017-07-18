#include "bsp_systick.h"
 
static __IO uint32_t TimingDelay;//全局变量 限定作用域

void SysTick_Init(void)
{
	//配置Systick重载值，系统时钟72MHz， 72*(1/720000000) = 1us
	if(SysTick_Config(72))
	{
		while(1);
	}
	/**
	或者写成
	while(Systick_Config(72000) == 1 );
	*/
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;// 关闭滴答定时器
}


void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}


//滴答中断处理
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}


//延时us
void delay_us(__IO uint32_t nTime)
{
	TimingDelay = nTime;//时钟滴答
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;// 使能滴答定时器
	while( TimingDelay != 0 );
}

//延时ms
void delay_ms(__IO uint32_t nTime)
{
	delay_us(nTime * 1000);	//延时1ms
}

/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}


