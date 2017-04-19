/******************** (C) COPYRIGHT 2013 ***************************
 * 文件名  ：crc.c
 * 描述    ：crc（循环冗余校验）应用函数库         
 * 实验平台： STM32开发板
 * 硬件连接：--------------------------
 *          |                          |
 *          | 利用的是CPU内部的CRC硬件 |
 *          |                          |
 *           --------------------------
 * 库版本  ：ST3.5.0

**********************************************************************************/
#include "bsp_crc.h"

__IO uint32_t CRCValue = 147;		 // 用于存放产生的CRC校验值

/*
 * 函数名：CRC_Config
 * 描述  ：使能CRC时钟
 * 输入  ：无
 * 输出  ：无
 * 调用  : 外部调用
 */
void CRC_Config(void)
{
	/* Enable CRC clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}



//获得CRC16值 
//puchMsg:要校验的数组 
//usDataLen:数组长度 
u16 Get_Crc16(u8 *puchMsg,u16 usDataLen) 
{ 
    u8 uchCRCHi=0xFF; 	//高CRC 字节初始化 
    u8 uchCRCLo=0xFF; 	//低CRC 字节初始化  
    u32 uIndex; 	//CRC 循环中的索引 
    while(usDataLen--) 	//传输消息缓冲区 
    { 
        uIndex=uchCRCHi^*puchMsg++; //计算CRC  
        uchCRCHi=uchCRCLo^auchCRCHi[uIndex]; 
        uchCRCLo=auchCRCLo[uIndex]; 
    } 
    return (uchCRCHi<<8|uchCRCLo); 
} 
//CRC8校验 
//ptr:要校验的数组 
//len:数组长度 
//返回值:CRC8码 
u8 Get_Crc8(u8 *ptr,u16 len) 
{ 
    u8 crc; 
    u8 i; 
    crc=0; 
    while(len--) 
    { 
        crc^=*ptr++; 
        for(i=0;i<8;i++) 
        { 
            if(crc&0x01)crc=(crc>>1)^0x8C; 
            else crc >>= 1; 
        } 
    } 
    return crc; 
}
/******************* (C) COPYRIGHT 2013 *****END OF FILE************/
