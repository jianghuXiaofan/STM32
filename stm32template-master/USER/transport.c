
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "transport.h"
#include "bsp_usart.h"
extern u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

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
	int rc =0;
	//printf("received %d bytes count %d\n", rc, (int)count);
    int i = 0;
    for(i =0;i<USART_REC_LEN;i++)
    {
        buf[i] = USART2_RX_BUF[i];
    }
    //printf("%s\n",buf);
    //printf("%s\n",USART2_RX_BUF);
	return rc;
}

int transport_getdatanb(void *sck, unsigned char* buf, int count)
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
