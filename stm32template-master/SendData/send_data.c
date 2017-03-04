#include "send_data.h"
//���������ͨ�� MQTT �������ݰ���

void Send_pack(MsgPack *pack)
{
	//MQTT ��س�ʼ������
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	//int rc = 0;
	char buf[200]; //���mqtt������Ϣ
	int buflen = sizeof(buf);
	MQTTString topicString = MQTTString_initializer;
	int len = 0;
	data.clientID.cstring = "sectong";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "";
	data.password.cstring = "";
	data.MQTTVersion = 4; //�汾�� 3 = 3.1 4 = 3.1.1
    
    topicString.cstring = "lalala";//�����޸�topic 
    
/***************************************************************/
    //����ʪ�ȴ��������JSON��ʽ����
    cJSON * root;//cJSON��ʽ��װʹ��
    root = cJSON_CreateObject();//������malloc�����ϵĿռ�
    cJSON_AddNumberToObject(root,"temp",pack->temp_int);
    cJSON_AddNumberToObject(root,"humi",pack->humi_int);
    char *out = cJSON_Print(root);
    //printf("%s\r\n",out);
    cJSON_Delete(root);	
    free(out);//�������Ҫ�ǵ��ͷ�
    
/***************************************************************/   
    //MQTT Э�鷢��
    char* payload = out;
    int payloadlen = strlen(payload);
    
    len = MQTTSerialize_connect((unsigned char *)buf, buflen, &data);
    len += MQTTSerialize_publish((unsigned char *)(buf + len),
            buflen - len, 0, 0, 0, 0, topicString, 
            (unsigned char *)payload, payloadlen);//publish msg
    
    len += MQTTSerialize_disconnect((unsigned char *)(buf + len), buflen - len);   
    transport_sendPacketBuffer((unsigned char*)buf, len); //ת��
}
