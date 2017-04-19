#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_crc.h"

extern __IO uint32_t CRCValue;


#define  STM32_CRC_DEF   0x04c11db7      //STM32Ӳ��CRC������ʽ
u32 RETURN_CRC_DATA(u32 *DATA_BUF, u16 len);

//�ܽ᣺stm32Ӳ��crc_32����С�ֶ˴���ʽ��PC��������Ǵ��ֶ˿�
//���ԣ�stm32Ӳ���������ֵ��PC�˼�������Ĳ�һ��
//STM32��CRC32��Ŀǰ�������PC�����ʹ�õ�һЩ����˳�򼰷�����һ��
//��ֲһ�����CRC_8_16�ĳ��򣬷���modbusʹ�á�

int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж�������
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    CRC_Config();
//    printf("\r\n ����һ�� CRC(ѭ������У��)ʵ�� \r\n");
//    /* Compute the CRC of "DataBuffer" */
//        for(int i=0; i<BUFFER_SIZE; i++ ) 
//        {
//            CRCValue = CRC_CalcBlockCRC((uint32_t *)DataBuffer, BUFFER_SIZE);
//            printf("\r\n32-bit CRC У����Ϊ:0X%X\r\n", CRCValue);
//        }
//        printf("\r\nCRC(ѭ������У��)���Գɹ�\r\n");
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
    u32    xbit = 0 ;                //CRC����ʽ���� ɨ�����
    u32    data = 0 ;                //��ǰ����CRC�����ݻ���
    u32    CRC_DATA = 0xFFFFFFFF;    // CRC���ݼĴ���  ����ֵ
    u8     bits = 0 ;                    //λ��������
    while (len--) 
    {
      xbit = 0x80000000;
      data = *DATA_BUF++;
      for(bits = 0; bits < 32; bits++) 
            {
       if(CRC_DATA & 0x80000000)   //CRC����ʽ����
             {
        CRC_DATA <<= 1;
        CRC_DATA ^= STM32_CRC_DEF;
       }
       else
             {
              CRC_DATA <<= 1;
             }
                            
       if (data & xbit)   //CRC����ʽ����
             {
              CRC_DATA ^= STM32_CRC_DEF;
             }
       xbit >>= 1;
      }
    }
    return CRC_DATA;
}










