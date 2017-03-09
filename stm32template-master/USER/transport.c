#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "transport.h"
#include "bsp_usart2.h"
#include "timer.h"


#include "cJSON.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "package.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern volatile u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern volatile u16 rx_buf_len; //串口2 接收FIFO计数值
extern volatile u16 USART2_RX_OK_FLAG;//接收完成标志


/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/

int transport_sendPacketBuffer(unsigned char* buf, int buflen)
{
	int rc = 0;
	usart2_send_nbyte(buf, buflen);
	//printf("Debug %s\r\n",buf);
	return rc;
}

int transport_getdata(unsigned char* buf, int count)
{
	int rc =4;
    my_recv(buf,count);    
    //printf("rc%d\r\n",rc);
	return rc;
}

/****************************************************************************************/
//请求连接connect --> 请求应答 connack 收到 20 02 00 00
//订阅请求subscribe --> 订阅应答 SUBACK 收到 90 03 00 01 00
//发布消息PUBLISH --> 订阅应答 SUBACK 收到 接收到的数据
/****************************************************************************************/
int MQTTPacket_My_read(unsigned char* buf, int buflen)
{
    int rc = 0;//自己的接收函数 
    while(USART2_RX_OK_FLAG !=1);   //阻塞等待接收完成标志
    USART2_RX_OK_FLAG = 0;          //清完成标志位
    for(int i=0;i<rx_buf_len;i++)
    {
        buf[i] = USART2_RX_BUF[i];  
    }
//  for(int i=0;i<rx_buf_len;i++) { //打印测试 收到的是否是正确的
//      printf("%02x ",buf[i]);
//    }
    rx_buf_len = 0; //串口2 接收FIFO重新计数。
    //memset(USART2_RX_BUF,0,sizeof(USART_REC_LEN));
    rc = buf[0];    //返回第一个接收字符(报头)
    //printf("%d\r\n",rc);
    
    switch(rc)
    {
        case 0x10:  rc = CONNECT; break;
        case 0x20:  rc = CONNACK; break;
        case 0x30:  rc = PUBLISH; break;
        case 0x40:  rc = PUBACK; break;
        case 0x50:  rc = PUBREC; break;
        case 0x60:  rc = PUBREL; break;
        case 0x70:  rc = PUBCOMP; break;
        case 0x80:  rc = SUBSCRIBE; break;
        case 0x90:  rc = SUBACK; break;
        case 0xa0:  rc = UNSUBSCRIBE; break;
        case 0xb0:  rc = UNSUBACK; break;
        case 0xc0:  rc = PINGREQ; break;
        case 0xd0:  rc = PINGRESP;break;
        case 0xe0:  rc = DISCONNECT; break;
        default: rc = 0;
    }
    //printf("%d\r\n",rc);
    return rc;
}
/*****************************************************************************************/

//阻塞等待接收完成标志位 清标志位 禁止接收中断 
//读取数据 清理上一次缓存 使能接收中断
int my_recv(unsigned char* buf, int len)
{
    int i = 0;
//    if(SEND_flag==1){
//        SEND_flag = 0;
//        while(USART2_RX_OK_FLAG !=1);//阻塞等待接收完成标志
//        USART2_RX_OK_FLAG = 0;
//    }
    if(len <= 0)
        return -1;
    if(len > USART_REC_LEN)
        len = USART_REC_LEN - 2;
    for(i=0;i<len;i++)
    {
        buf[i] = USART2_RX_BUF[i];  
    }
//  memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
    printf("len %d\n",len);
    return len;
}



int transport_getdatanb(unsigned char* buf, int count)
{
	int rc=0;
	return rc;
}


/**
return >=0 for a socket descriptor, <0 for an error code
@todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
removing indirections
*/
int transport_open(char* addr, int port)
{
	int rc = 0;
	return rc;
}

int transport_close(void)
{
	int rc=0;
	return rc;
}
