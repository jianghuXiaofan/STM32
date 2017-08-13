#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "intrinsics.h"
#include "stm8s_uart1.h"
#include <stdio.h>
#include "stm8s_it.h"
#include "stm8s_exti.h"

u8 RxBuffer1[64];
volatile u8 UART1_RX_NUM=0;
/***********************
函数功能：us延时
输入参数：无
输出参数：无
备    注：粗略延时
***********************/
void delay_us(void)
{ 
    asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
    asm("nop");
    asm("nop");
    asm("nop"); 
}

/***********************
函数功能：ms延时
输入参数：无
输出参数：无
备    注：粗略延时
***********************/
/*void delay_ms(unsigned int time)
{
    unsigned int i;
    while(time--)  
    for(i=900;i>0;i--)
    delay_us(); 
}*/
void led_Init(void);
void key_scan(void);
void uart1_Init(void)
{
  UART1_DeInit();
  UART1_Init((u32)115200,UART1_WORDLENGTH_8D,UART1_STOPBITS_1, \
    UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
  UART1_Cmd(ENABLE);
}

void UART1_SendByte(u8 data)
{
  UART1_SendData8((unsigned char)data);
  while(UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);
}

void UART1_SendString(u8* Data,u16 len)
{
  u16 i=0;
  for(;i<len;i++)
  {
    UART1_SendByte(*Data+i);
  }
}

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch,FILE *f)
{
  UART1_SendData8((unsigned char)ch);
  //while(!(UART1->SR&UART1_FLAG_TXE));
   while((UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));
  return(ch);
}

void key_exit_Init(void)
{
  GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_IN_PU_IT);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD,EXTI_SENSITIVITY_FALL_ONLY);
}
u32 TimingDelay; 
void timer1_Init(void)
{
  TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,1000,0);
  TIM1_ARRPreloadConfig(ENABLE);
  TIM1_ITConfig(TIM1_IT_UPDATE,ENABLE);
  TIM1_Cmd(ENABLE);
}

void delay_ms( u32 nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void pwm_Init(void)
{
  TIM2_TimeBaseInit(TIM2_PRESCALER_16,499);
  TIM2_OC3Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,500,TIM2_OCPOLARITY_HIGH);
  TIM2_OC2PreloadConfig(ENABLE);
  TIM2_Cmd(ENABLE);
}
void SetTIM2_PWM_DutyCycle( uint16_t TIM2_Pulse)
{
    TIM2->CCR3H = (uint8_t)(TIM2_Pulse >> 8);
    TIM2->CCR3L = (uint8_t)(TIM2_Pulse);
}

int main(void)
{
  /*Infinite loop*/
  int len=0;
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  led_Init();
  //key_scan();
  uart1_Init();
  key_exit_Init();
  timer1_Init();
  pwm_Init();
  __enable_interrupt();
  for(;;)
  {  
      /*if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_3)==0)
      {
        delay_ms(10);
        if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_3)==0)
        {
          printf("ddddd%d\n",22);
          while(GPIO_ReadInputPin(GPIOD,GPIO_PIN_3)==0);
        }
      }
      if(UART1_RX_NUM&0x80)
      {
        len=UART1_RX_NUM&0x3f;
        printf("recv%s-%d\n",RxBuffer1,len);
        GPIO_WriteReverse(GPIOA, GPIO_PIN_3);
        UART1_RX_NUM=0;
      }*/
    u16 Duty_Val;
    for(Duty_Val=0;Duty_Val<499;Duty_Val++)
    {
      SetTIM2_PWM_DutyCycle(Duty_Val);
      delay_ms(5);
    }
      for(;Duty_Val>0;Duty_Val--)
    {
      SetTIM2_PWM_DutyCycle(Duty_Val);
      delay_ms(5);
    }
  }
}

void led_Init(void)
{
  GPIO_Init(GPIOA,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);
      /*delay_ms(500);
    GPIO_WriteLow(GPIOA, GPIO_PIN_3);
    delay_ms(500);
    GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
    */
}
void key_scan(void)
{
  GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);
      /*if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_3)==0)
    {
        delay_ms(10);
        if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_3)==0)
        {
            GPIO_WriteReverse(GPIOA,GPIO_PIN_3);
            while(!GPIO_ReadInputPin(GPIOD,GPIO_PIN_3));
        }
    }*/
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif