#include "receive_data.h"
#include "timer.h"
#include "bsp_usart2.h"
#include "cJSON.h"
#include "bsp_relay.h"
#include "led.h"

extern u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern volatile u16 rx_buf_len;                      //串口2 接收FIFO计数值
extern volatile u16 USART2_RX_OK_FLAG;               //接收完成标志
u8 Temp_BUF[USART_REC_LEN];//临时缓存
extern int read_buf_len;
void Reset_receive_buf(void);

void getdata(void)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	char* payload = "mypayload";
	int payloadlen = strlen(payload);
	int len = 0;

	data.clientID.cstring = "sectong";
	data.keepAliveInterval = 30;
	data.cleansession = 1;
	data.username.cstring = "";
	data.password.cstring = "";

	len = MQTTSerialize_connect(buf, buflen, &data);
	transport_sendPacketBuffer(buf, len);//请求连接 connect
    
	/* wait for connack */   
    Reset_receive_buf();//等待接收完成
    int ret = MQTTPacket_read(buf, buflen, transport_getdata);
    printf("connect ack: %d\r\n",ret);
    if ( ret == CONNACK)
	{
		unsigned char sessionPresent, connack_rc;
        
		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			printf("Unable to connect, return code %d\n", connack_rc);
			//goto exit;
		}
	}
	else
		printf("error!\n");
    
    /* subscribe */  
    topicString.cstring = "pubtopic";//订阅请求 subscribe 
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
			//goto exit;
		}
	}
	else
		printf("error!\n");//goto exit;
    
    TIM_Cmd(TIM3, ENABLE);//开启定时器3   
//    topicString.cstring = "pubtopic";//发布消息 PUBLISH 
//    len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
//    rc = transport_sendPacketBuffer(buf, len);
    
    while(1)
    {
        Reset_receive_buf();
        TIM_Cmd(TIM3, DISABLE);
        TIM_SetCounter(TIM3, 0);
        
        if (MQTTPacket_read(buf, buflen, transport_getdata) == PUBLISH)
        {
            unsigned char dup;
            int qos;
            unsigned char retained;
            unsigned short msgid;
            int payloadlen_in;
            unsigned char* payload_in;
            MQTTString receivedTopic;

            MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
                    &payload_in, &payloadlen_in, buf, buflen);
            //printf("message arrived %.*s\n", payloadlen_in, payload_in);//妙

            //cjson 解除数据
//            char msg[200] = "";
//            memset(msg,0,sizeof(200));
//            memcpy(msg,payload_in,sizeof(msg));
//            cJSON *pSub = NULL;
//            cJSON * pJson = cJSON_Parse(msg);
            // get cmd from json
//            pSub = cJSON_GetObjectItem( pJson, "Relay" );
//            if(pSub==NULL){
//                continue;
//            }
        //上面不稳定，下面的代码相对好一些
            cJSON * root = cJSON_Parse((void*)payload_in);
            int relay = cJSON_GetObjectItem(root, "relay")->valueint;
            printf( "Relay : %d\n", relay );
            cJSON_Delete(root);
            //free(root);
            //printf( "Relay : %d\n", pSub->valueint );  
//            if(pSub->valueint==1)
//            {
//                Relay(ON);//继电器开
//            }
//            else 
//                Relay(OFF);

//            if(pSub!=NULL)
//            {
//                cJSON_Delete(pSub);
//                free(pSub);
//            }
        }
        TIM_Cmd(TIM3, ENABLE); 
    }        
}


void Reset_receive_buf(void)
{
    rx_buf_len = 0; //清除接收缓存区起始位
    while(USART2_RX_OK_FLAG !=1);//阻塞等待接收完成标志
    USART2_RX_OK_FLAG = 0;//清完成标志位
    read_buf_len = 0; //清除读取缓冲区起始位
}



//int ch = 0;
//switch(ch)
//        {
////  CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
////	PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
////	PINGREQ, PINGRESP, DISCONNECT
//            case CONNECT:printf("connect\r\n");break;
//            case CONNACK:printf("connack\r\n");break;
//            case PUBLISH:i = 1;break;//3
//            case SUBACK:printf("suback\r\n");break;
//            case DISCONNECT:printf("suback\r\n");break;
//            case PINGRESP:printf("ping\r\n");break;
//            default : printf("error\r\n");
//        }

