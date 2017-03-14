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

extern u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern volatile u16 rx_buf_len; //串口2 接收FIFO计数值
extern volatile u16 USART2_RX_OK_FLAG;//接收完成标志
uint16_t num = 0;
volatile int read_buf_len = 0; //读入缓冲区起始位
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
	//int rc = 0;
    //rc = my_recv(buf,count); 
	//return rc;

    memcpy(buf, (void*)&USART2_RX_BUF[read_buf_len], count);//(void*)强转why?
    read_buf_len += count;
    return count;
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
/*********************************************************************************/
//一下代码有问题，不要参考了！

//阻塞等待接收完成标志位 清标志位 禁止接收中断 
//读取数据 清理上一次缓存 使能接收中断
//int my_recv(unsigned char* buf, int len)
//{
//    int i = 0;
//    if (len <= 0)
//        return 0;
//    if (len > USART_REC_LEN)
//        len=USART_REC_LEN - 2;
//    
//    for(i = 0;i<len;i++)
//    {
//        *buf = Temp_BUF[0];//第二缓存区   
//        *buf++;
//        memmove(Temp_BUF,Temp_BUF+1,(USART_REC_LEN-1)*sizeof(*Temp_BUF));//整个数组左移
//        read_buf_len++;
//    }      
//    return len;
//}

//void wait_rx_full(void)//等待接收数据完成
//{
//    while(USART2_RX_OK_FLAG !=1);   //阻塞等待接收完成标志
//    //memcpy(Temp_BUF,USART2_RX_BUF,sizeof(Temp_BUF));
//    USART2_RX_OK_FLAG = 0;          //清完成标志位
//    rx_buf_len = 0;//串口2  接收FIFO重新计数。
//    read_buf_len = 0;
//}
/****************************************************************************************/
//请求连接connect --> 请求应答 connack 收到 20 02 00 00
//订阅请求subscribe --> 订阅应答 SUBACK 收到 90 03 00 01 00
//发布消息PUBLISH --> 订阅应答 SUBACK 收到 接收到的数据
/*****************************************************************************************/
    /*df*/
//    int read_buf_len = 0; //游标
//    int transport_getdata(unsigned char* buf, int count)
//    {
    //	  int rc = 0;
    //    memcpy(buf, &USART2_RX_BUF+read_buf_len, count);
    //    read_buf_len += count;
    //    return count;
//    }
