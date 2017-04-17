#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_spi1.h"
#include "bsp_cc1101.h"
#include "string.h"


#define SEND_GAP        1000    // ÿ���1s����һ������
#define RECV_TIMEOUT    800     // ���ճ�ʱ

#define ACK_LENGTH      10      // Ӧ���źų���        
#define SEND_LENGTH     10      // ��������ÿ���ĳ���

uint8_t   Cnt1ms = 0;             // 1ms����������ÿ1ms��һ 
uint8_t   SendFlag = 0;           // =1�������������ݣ�=0������

uint16_t  SendTime = 1;           // �������ݷ��ͼ��ʱ��
uint16_t  RecvWaitTime = 0;       // ���յȴ�ʱ��                
uint16_t  SendCnt = 0;            // �������͵����ݰ���  

uint8_t AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
uint8_t RF_SendPacket(uint8_t *Sendbuffer, uint8_t length);
uint16_t RF_RecvHandler(uint8_t * Receivebuffer);

int main(void)
{   
    uint8_t buff[20] = "dfsdfs";
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж�������
    SysTick_Init();    
    led_Init();
    Usart1_Init(115200);
    
    CC1101Init();
    CC1101SetTRMode(TX_MODE);
    
    for( ;; )
    {
        delay_ms(1000);
        if (RF_SendPacket(buff, SEND_LENGTH))
        {
            printf("sendok\r\n"); 
            //CC1101SetTRMode(RX_MODE);
            //RF_RecvHandler();
        }
        delay_ms(1000);
    }
}



uint8_t RF_SendPacket(uint8_t * Sendbuffer, uint8_t length)
{
    uint8_t ack_flag = 0;         // =1,���յ�Ӧ���źţ�=0������
    uint8_t error = 0, i=0, ack_len=0, ack_buffer[65]={ 0 }, TxBuffer[100];

    CC1101SendPacket(Sendbuffer, length, ADDRESS_CHECK);    // ��������   
    CC1101SetTRMode(RX_MODE);           // �������ģʽ���ȴ�Ӧ��
    RecvWaitTime = RECV_TIMEOUT;        // �ȴ�Ӧ��ʱ����Ϊ800ms

    ack_flag = 1;

    while (CC_IRQ_READ() != 0)
    {
        if (0 == RecvWaitTime)      
        { 
            ack_flag = 0; 
            break; 
        }
    }

    if (0 != ack_flag)                          // ����Ƿ��յ����ݰ�
    {
        while (CC_IRQ_READ() == 0);
        ack_len = CC1101RecPacket(ack_buffer);  // ��ȡ�յ�������

        // �ж������Ƿ�����Ӧ���ź�Ӧ��Ϊ10-19
        for (i=0, error=0; i<10; i++ )
        {
            if (ack_buffer[i] != (i+10))    
            { 
                error=1; 
                break; 
            }
        }

        if ((ack_len==10) && (error==0))    
        { 
            return (1); 
        } // ��������  
    }
    return (0);  
}


/*===========================================================================
* ���� ��RF_RecvHandler() => �������ݽ��մ���                               * 
============================================================================*/
uint16_t RF_RecvHandler(uint8_t * Receivebuffer)
{
    uint8_t error=0, i=0, length=0, recv_buffer[65]={ 0 };
    
    CC1101SetTRMode(RX_MODE);           // ����RFоƬ����ģʽ����������
    
    if (0 == CC_IRQ_READ())             // �������ģ���Ƿ���������ж� 
    {
        while (CC_IRQ_READ() == 0);

        // �������㣬��ֹ���ж�
        memset(recv_buffer,0,sizeof(recv_buffer));
            
        // ��ȡ���յ������ݳ��Ⱥ���������
        length = CC1101RecPacket(recv_buffer);
       
        // �ж������Ƿ����󣬽��յ����ź�Ӧ��Ϊ0-9
        for (i=0, error=0; i<10; i++)
        {
            if (recv_buffer[i] != i)    
            { 
                error=1; 
                break; 
            } // ���ݳ���
        }

        if ((length==10) && (error==0))                     // ������ȷ
        {
            printf("receive %s",recv_buffer);
            CC1101SetTRMode(TX_MODE);   // ����Ӧ���ź�
            CC1101SendPacket(AckBuffer, ACK_LENGTH, ADDRESS_CHECK);    
            //RecvCnt++;   
        }
        
//        CC1101SetTRMode(RX_MODE);           // ����RFоƬ����ģʽ����������
    }    
}














