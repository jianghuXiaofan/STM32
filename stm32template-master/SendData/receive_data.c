#include "receive_data.h"
#include "timer.h"
#include "bsp_usart2.h"

extern volatile u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern volatile u16 rx_buf_len;
extern volatile u16 USART2_RX_OK_FLAG;//接收完成标志

void getdata(void)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
    unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	char* payload = "mypayload";
	int payloadlen = strlen(payload);
	int len = 0;

	data.clientID.cstring = "sectong";
	data.keepAliveInterval = 60;
	data.cleansession = 1;
	data.username.cstring = "";
	data.password.cstring = "";

	len = MQTTSerialize_connect(buf, buflen, &data);
	rc = transport_sendPacketBuffer(buf, len);//发送数据包 请求连接
    
//	/* wait for connack */   
    MQTTPacket_My_read(buf, rx_buf_len);//请求应答 收到
   
     for(int i=0;i<rx_buf_len;i++)
    {
        printf("%02x ",buf[i]); 
    }
    rx_buf_len = 0;
    //while(1);//收到 20 02 00 00
    
    /* wait for connack */
    topicString.cstring = "pubtopic";//订阅请求
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
	rc = transport_sendPacketBuffer(buf, len);
    
    MQTTPacket_My_read(buf, rx_buf_len);//订阅应答 SUBACK
    for(int i=0;i<rx_buf_len;i++)
        printf("%02x ",buf[i]);
    rx_buf_len = 0;
    //while(1);//收到 90 03 00 01 00
    
    topicString.cstring = "pubtopic";//PUBLISH 发布消息
    len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
    rc = transport_sendPacketBuffer(buf, len);
    while(1){
        MQTTPacket_My_read(buf, rx_buf_len);//订阅应答 SUBACK
//        for(int i=0;i<rx_buf_len;i++)
//            printf("%c ",buf[i]);
        rx_buf_len = 0;
        unsigned char dup;
        int qos;
        unsigned char retained;
        unsigned short msgid;
        int payloadlen_in;
        unsigned char* payload_in;
        int rc;
        MQTTString receivedTopic;

        rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
                &payload_in, &payloadlen_in, buf, buflen);
        printf("message arrived %.*s\n", payloadlen_in, payload_in);
        //while(1);   
    }        
}