#include "receive_data.h"
#include "timer.h"
#include "bsp_usart2.h"


extern volatile u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern volatile u16 rx_buf_len;                      //串口2 接收FIFO计数值
extern volatile u16 USART2_RX_OK_FLAG;               //接收完成标志

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
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "";
	data.password.cstring = "";

	len = MQTTSerialize_connect(buf, buflen, &data);
	rc = transport_sendPacketBuffer(buf, len);//请求连接 connect
    
	/* wait for connack */   
    MQTTPacket_My_read(buf, rx_buf_len);
    
    /* subscribe */  
    topicString.cstring = "pubtopic";//订阅请求 subscribe //千万别操作只读变量 可以改变指向
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
	rc = transport_sendPacketBuffer(buf, len);
    
    MQTTPacket_My_read(buf, rx_buf_len);//订阅应答 SUBACK
    TIM_Cmd(TIM3, ENABLE);//开启定时器3
    
//    topicString.cstring = "pubtopic";//发布消息 PUBLISH 
//    len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
//    rc = transport_sendPacketBuffer(buf, len);
    
    while(1)
    {
        int ch = 0;
        int i = 0;
        ch = MQTTPacket_My_read(buf, rx_buf_len);//订阅应答 SUBACK
        switch(ch)
        {
//  CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
//	PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
//	PINGREQ, PINGRESP, DISCONNECT
            case CONNECT:printf("connect\r\n");break;
            case CONNACK:printf("connack\r\n");break;
            case PUBLISH:i = 1;break;//3
            case SUBACK:printf("suback\r\n");break;
            case DISCONNECT:printf("suback\r\n");break;
            case PINGRESP:printf("ping\r\n");break;
            default : printf("error\r\n");
        }
        
        if(i == 1){
        i=0;
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
        printf("message arrived %.*s\n", payloadlen_in, payload_in);//妙
            //cjson 解除数据包
        }
    }        
}