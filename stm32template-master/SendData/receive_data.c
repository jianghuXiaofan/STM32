#include "receive_data.h"
#include "timer.h"
#include "bsp_usart2.h"
#include "cJSON.h"
#include "bsp_relay.h"
#include "led.h"

extern u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
extern volatile u16 rx_buf_len;                      //����2 ����FIFO����ֵ
extern volatile u16 USART2_RX_OK_FLAG;               //������ɱ�־
extern volatile int read_buf_len;


//�������ã��������ӣ�����id����Ϣ������payload��
//�ȴ��������� ������Ӧ�Ĵ���
void getdata(void)
{ 
    int pack_type = 0;    
    unsigned char buf[200];
	int buflen = sizeof(buf);
	
	char* payload = "mypayload";
	int payloadlen = strlen(payload);
   
    if(USART2_RX_OK_FLAG ==1)//�����ȴ�������ɱ�־
    {   
        rx_buf_len = 0; //������ջ�������ʼλ
        USART2_RX_OK_FLAG = 0;//����ɱ�־λ
        read_buf_len = 0; //�����ȡ��������ʼλ
        pack_type = MQTTPacket_read(buf, buflen, transport_getdata);
        //printf("%d\n",pack_type);
        switch(pack_type)//���������ݲ�ͬ����Ϣ��������
        {
            case CONNECT:printf("connect\r\n");break;
            case CONNACK:printf("connack\r\n");break;
            case PUBLISH:suback_msg(buf,buflen);break;
            case SUBACK:printf("suback\r\n");break;
            case DISCONNECT:printf("suback\r\n");break;
            case PINGRESP:printf("ping\r\n");break;
            default : printf("error\r\n");             
        }
    }  
}



//���ĺ��յ���Ϣ�Ľ�������
void suback_msg(unsigned char* buf,int buflen)
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
    //printf("message arrived %.*s\n", payloadlen_in, payload_in);//��
    
    cJSON * root = cJSON_Parse((void*)payload_in);//�������ݴ����֣���ܻ�û���
    int relay = cJSON_GetObjectItem(root, "relay")->valueint;
    printf( "Relay : %d\n", relay );
    if(root != NULL) 
    {
        cJSON_Delete(root);
        //free(root);  
    }
    if(relay==1)
    {
        Relay(ON);//�̵�����
    }
    else 
        Relay(OFF); 
}


void Reset_receive_buf(void)
{
    rx_buf_len = 0; //������ջ�������ʼλ
    while(USART2_RX_OK_FLAG !=1);//�����ȴ�������ɱ�־
    USART2_RX_OK_FLAG = 0;//����ɱ�־λ
    read_buf_len = 0; //�����ȡ��������ʼλ
}







//    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
//    unsigned char buf[200];
//	int buflen = sizeof(buf);
//	int msgid = 1;
//	MQTTString topicString = MQTTString_initializer;
//	int req_qos = 0;
//	char* payload = "mypayload";
//	int payloadlen = strlen(payload);
//	int len = 0;

//	data.clientID.cstring = "sectong";
//	data.keepAliveInterval = 30;
//	data.cleansession = 1;
//	data.username.cstring = "";
//	data.password.cstring = "";

//	len = MQTTSerialize_connect(buf, buflen, &data);
//	transport_sendPacketBuffer(buf, len);//�������� connect
//    
//	/* wait for connack */   
//    Reset_receive_buf();//�ȴ��������
//    int ret = MQTTPacket_read(buf, buflen, transport_getdata);
//    printf("connect ack: %d\r\n",ret);
//    if ( ret == CONNACK)
//	{
//		unsigned char sessionPresent, connack_rc;
//        
//		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
//		{
//			printf("Unable to connect, return code %d\n", connack_rc);
//			//goto exit;
//		}
//	}
//	else
//		printf("error!\n");
    
//    /* subscribe */  
//    topicString.cstring = "pubtopic";//�������� subscribe 
//	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
//	transport_sendPacketBuffer(buf, len);
//    
//    Reset_receive_buf();//�ȴ��������
//    if (MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK) 	/* wait for suback */ 
//	{
//		unsigned short submsgid;
//		int subcount;
//		int granted_qos;

//		MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
//		if (granted_qos != 0)
//		{
//			printf("granted qos != 0, %d\n", granted_qos);
//			//goto exit;
//		}
//	}
//	else
//		printf("error!\n");//goto exit;
    
//    TIM_Cmd(TIM3, ENABLE);//������ʱ��3   
//    topicString.cstring = "pubtopic";//������Ϣ PUBLISH 
//    len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
//    rc = transport_sendPacketBuffer(buf, len);

