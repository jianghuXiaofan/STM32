#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_spi1.h"
#include "bsp_cc1101.h"
#include "string.h"


#define SEND_GAP        1000    // 每间隔1s发送一次数据
#define RECV_TIMEOUT    800     // 接收超时

#define ACK_LENGTH      10      // 应答信号长度        
#define SEND_LENGTH     10      // 发送数据每包的长度

uint8_t   Cnt1ms = 0;             // 1ms计数变量，每1ms加一 
uint8_t   SendFlag = 0;           // =1，发送无线数据，=0不处理

uint16_t  SendTime = 1;           // 计数数据发送间隔时间
uint16_t  RecvWaitTime = 0;       // 接收等待时间                
uint16_t  SendCnt = 0;            // 计数发送的数据包数  

uint8_t AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
uint8_t RF_SendPacket(uint8_t *Sendbuffer, uint8_t length);
uint16_t RF_RecvHandler(uint8_t * Receivebuffer);

int main(void)
{   
    uint8_t buff[20] = "dfsdfs";
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断组设置
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
    uint8_t ack_flag = 0;         // =1,接收到应答信号，=0不处理
    uint8_t error = 0, i=0, ack_len=0, ack_buffer[65]={ 0 }, TxBuffer[100];

    CC1101SendPacket(Sendbuffer, length, ADDRESS_CHECK);    // 发送数据   
    CC1101SetTRMode(RX_MODE);           // 进入接收模式，等待应答
    RecvWaitTime = RECV_TIMEOUT;        // 等待应答超时限制为800ms

    ack_flag = 1;

    while (CC_IRQ_READ() != 0)
    {
        if (0 == RecvWaitTime)      
        { 
            ack_flag = 0; 
            break; 
        }
    }

    if (0 != ack_flag)                          // 检测是否收到数据包
    {
        while (CC_IRQ_READ() == 0);
        ack_len = CC1101RecPacket(ack_buffer);  // 读取收到的数据

        // 判断数据是否有误，应答信号应该为10-19
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
        } // 数据无误  
    }
    return (0);  
}


/*===========================================================================
* 函数 ：RF_RecvHandler() => 无线数据接收处理                               * 
============================================================================*/
uint16_t RF_RecvHandler(uint8_t * Receivebuffer)
{
    uint8_t error=0, i=0, length=0, recv_buffer[65]={ 0 };
    
    CC1101SetTRMode(RX_MODE);           // 设置RF芯片接收模式，接收数据
    
    if (0 == CC_IRQ_READ())             // 检测无线模块是否产生接收中断 
    {
        while (CC_IRQ_READ() == 0);

        // 数据请零，防止误判断
        memset(recv_buffer,0,sizeof(recv_buffer));
            
        // 读取接收到的数据长度和数据内容
        length = CC1101RecPacket(recv_buffer);
       
        // 判断数据是否有误，接收到的信号应该为0-9
        for (i=0, error=0; i<10; i++)
        {
            if (recv_buffer[i] != i)    
            { 
                error=1; 
                break; 
            } // 数据出错
        }

        if ((length==10) && (error==0))                     // 数据正确
        {
            printf("receive %s",recv_buffer);
            CC1101SetTRMode(TX_MODE);   // 发送应答信号
            CC1101SendPacket(AckBuffer, ACK_LENGTH, ADDRESS_CHECK);    
            //RecvCnt++;   
        }
        
//        CC1101SetTRMode(RX_MODE);           // 设置RF芯片接收模式，接收数据
    }    
}














