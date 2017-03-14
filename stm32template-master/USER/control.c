#include "control.h"
#include "bsp_usart2.h"
#include <stdio.h>
#include <stdlib.h>
#include "receive_data.h"
#include "transport.h"

extern u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern volatile u16 rx_buf_len;                      //串口2 接收FIFO计数值
extern volatile u16 USART2_RX_OK_FLAG;               //接收完成标志
extern volatile int read_buf_len;

int MQTT_Connect(void)//unsigned char* buf, int buflen, MQTTPacket_connectData* data
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    unsigned char buf[200];
	int buflen = sizeof(buf);
	int len = 0;
	data.clientID.cstring = "sectong";
	data.keepAliveInterval = 30;
	data.cleansession = 1;
	data.username.cstring = "";
	data.password.cstring = "";
    //int len = 0;
    len = MQTTSerialize_connect(buf, buflen, &data);
	transport_sendPacketBuffer(buf, len);//请求连接 connect
    
	/* wait for connack */   
    Reset_receive_buf();//等待接收完成
    int ret = MQTTPacket_read(buf, buflen, transport_getdata);
    printf("read ret %d\n",ret);
    if ( ret == CONNACK)
	{
		unsigned char sessionPresent, connack_rc;
        
		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			printf("Unable to connect, return code %d\n", connack_rc);
			return ERROR;
		}
        return SUCCESS;
	}
	else
		return ERROR;
}


int MQTT_Subscribe(void)//unsigned char* buf,int buflen,MQTTString topicString,int msgid,int req_qos
{
    unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	int len = 0;
    /* subscribe */
    topicString.cstring = "pubtopic";
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
	transport_sendPacketBuffer(buf, len);
    
    Reset_receive_buf();//等待接收完成
    if (MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK) 	/* wait for suback */ 
	{
		unsigned short submsgid;
		int subcount;
		int granted_qos;

		MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if (granted_qos != 0)
		{
			printf("granted qos != 0, %d\n", granted_qos);
			return ERROR;
		}
        return SUCCESS;
	}
	else
		return ERROR;
}


