#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_crc.h"

extern __IO uint32_t CRCValue;


#define  STM32_CRC_DEF   0x04c11db7      //STM32硬件CRC计数等式
u32 RETURN_CRC_DATA(u32 *DATA_BUF, u16 len);

//总结：stm32硬件crc_32，是小字端处理方式，PC和软件的是大字端口
//所以：stm32硬件算出来的值与PC端计算出来的不一致
//STM32的CRC32与目前大多数的PC端软件使用的一些数据顺序及方法不一致
//移植一个软件CRC_8_16的程序，方便modbus使用。

int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    CRC_Config();
//    printf("\r\n 这是一个 CRC(循环冗余校验)实验 \r\n");
//    /* Compute the CRC of "DataBuffer" */
//        for(int i=0; i<BUFFER_SIZE; i++ ) 
//        {
//            CRCValue = CRC_CalcBlockCRC((uint32_t *)DataBuffer, BUFFER_SIZE);
//            printf("\r\n32-bit CRC 校验码为:0X%X\r\n", CRCValue);
//        }
//        printf("\r\nCRC(循环冗余校验)测试成功\r\n");
    uint8_t buff[7]= "we";
    for( ;; )
    {
        delay_ms(1000);
        //printf("OX%X\r\n",CRC_CalcBlockCRC((uint32_t*)buff, 6));
        //printf("OX%X\r\n",RETURN_CRC_DATA((u32 *)buff, 6));
       printf("OX%X\r\n",Get_Crc16(buff,2));//Modbus
        delay_ms(1000);
    }
}






u32 RETURN_CRC_DATA(u32 *DATA_BUF, u16 len) 
{
    u32    xbit = 0 ;                //CRC计算式计算 扫描变量
    u32    data = 0 ;                //当前用于CRC的数据缓存
    u32    CRC_DATA = 0xFFFFFFFF;    // CRC数据寄存器  返回值
    u8     bits = 0 ;                    //位计数变量
    while (len--) 
    {
      xbit = 0x80000000;
      data = *DATA_BUF++;
      for(bits = 0; bits < 32; bits++) 
            {
       if(CRC_DATA & 0x80000000)   //CRC计算式计算
             {
        CRC_DATA <<= 1;
        CRC_DATA ^= STM32_CRC_DEF;
       }
       else
             {
              CRC_DATA <<= 1;
             }
                            
       if (data & xbit)   //CRC计算式计算
             {
              CRC_DATA ^= STM32_CRC_DEF;
             }
       xbit >>= 1;
      }
    }
    return CRC_DATA;
}










