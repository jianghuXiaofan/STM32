/******************** (C) COPYRIGHT 2013 ***************************
 * �ļ���  ��crc.c
 * ����    ��crc��ѭ������У�飩Ӧ�ú�����         
 * ʵ��ƽ̨�� STM32������
 * Ӳ�����ӣ�--------------------------
 *          |                          |
 *          | ���õ���CPU�ڲ���CRCӲ�� |
 *          |                          |
 *           --------------------------
 * ��汾  ��ST3.5.0

**********************************************************************************/
#include "bsp_crc.h"

__IO uint32_t CRCValue = 147;		 // ���ڴ�Ų�����CRCУ��ֵ

/*
 * ��������CRC_Config
 * ����  ��ʹ��CRCʱ��
 * ����  ����
 * ���  ����
 * ����  : �ⲿ����
 */
void CRC_Config(void)
{
	/* Enable CRC clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}



//���CRC16ֵ 
//puchMsg:ҪУ������� 
//usDataLen:���鳤�� 
u16 Get_Crc16(u8 *puchMsg,u16 usDataLen) 
{ 
    u8 uchCRCHi=0xFF; 	//��CRC �ֽڳ�ʼ�� 
    u8 uchCRCLo=0xFF; 	//��CRC �ֽڳ�ʼ��  
    u32 uIndex; 	//CRC ѭ���е����� 
    while(usDataLen--) 	//������Ϣ������ 
    { 
        uIndex=uchCRCHi^*puchMsg++; //����CRC  
        uchCRCHi=uchCRCLo^auchCRCHi[uIndex]; 
        uchCRCLo=auchCRCLo[uIndex]; 
    } 
    return (uchCRCHi<<8|uchCRCLo); 
} 
//CRC8У�� 
//ptr:ҪУ������� 
//len:���鳤�� 
//����ֵ:CRC8�� 
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
