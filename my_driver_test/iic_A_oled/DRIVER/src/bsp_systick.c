#include "bsp_systick.h"
 
static __IO uint32_t TimingDelay;//ȫ�ֱ��� �޶�������

void SysTick_Init(void)
{
	//����Systick����ֵ��ϵͳʱ��72MHz�� 72*(1/720000000) = 1us
	if(SysTick_Config(72))
	{
		while(1);
	}
	/**
	����д��
	while(Systick_Config(72000) == 1 );
	*/
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;// �رյδ�ʱ��
}


void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}


//�δ��жϴ���
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}


//��ʱus
void delay_us(__IO uint32_t nTime)
{
	TimingDelay = nTime;//ʱ�ӵδ�
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;// ʹ�ܵδ�ʱ��
	while( TimingDelay != 0 );
}

//��ʱms
void delay_ms(__IO uint32_t nTime)
{
	delay_us(nTime * 1000);	//��ʱ1ms
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


