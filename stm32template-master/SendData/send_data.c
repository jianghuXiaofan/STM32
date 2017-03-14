#include "send_data.h"



//使用前提是:已经建立了连接
//作用:发送数据包到服务器
//说明：所有的传感器的信息存放到pack里面
void Send_pack_data(MsgPack *pack)
{
	char buf[200]; //存放mqtt发送信息
	int buflen = sizeof(buf);
	MQTTString topicString = MQTTString_initializer;
	int len = 0;
    topicString.cstring = "wz";//这里修改topic 如id 等等 
    
/***************************************************************/
    //将温湿度打包发送以JSON格式处理
    cJSON * root;//cJSON格式封装使用
    root = cJSON_CreateObject();//这里面malloc个堆上的空间
    cJSON_AddNumberToObject(root,"temp",pack->temp_int);
    cJSON_AddNumberToObject(root,"humi",pack->humi_int);
    char *out = cJSON_Print(root);
    //printf("%s\r\n",out);
    if(out!=NULL)
    {
        cJSON_Delete(root);
        if(root!=NULL) free(out);//因此这里要记得释放
    }
/***************************************************************/   
    //MQTT 协议发送
    char* payload = out;
    int payloadlen = strlen(payload);
    
    len = MQTTSerialize_publish((unsigned char *)buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
    transport_sendPacketBuffer((unsigned char*)buf, len); //转发
}











void Send_pack(MsgPack *pack)//测试用
{
	//MQTT 相关初始化设置
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	char buf[200]; //存放mqtt发送信息
	int buflen = sizeof(buf);
	MQTTString topicString = MQTTString_initializer;
	int len = 0;
	data.clientID.cstring = "sectong";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "";
	data.password.cstring = "";
	data.MQTTVersion = 4; //版本号 3 = 3.1 4 = 3.1.1
    
    topicString.cstring = "lwz";//这里修改topic 
    
/***************************************************************/
    //将温湿度打包发送以JSON格式处理
    cJSON * root;//cJSON格式封装使用
    root = cJSON_CreateObject();//这里面malloc个堆上的空间
    cJSON_AddNumberToObject(root,"temp",pack->temp_int);
    cJSON_AddNumberToObject(root,"humi",pack->humi_int);
    char *out = cJSON_Print(root);
    printf("%s\r\n",out);
    if(out!=NULL)
    {
        cJSON_Delete(root);
        if(root!=NULL) free(out);//因此这里要记得释放
    }
    
/***************************************************************/   
    //MQTT 协议发送
    char* payload = out;
    int payloadlen = strlen(payload);
    
    len = MQTTSerialize_connect((unsigned char *)buf, buflen, &data);
    len += MQTTSerialize_publish((unsigned char *)(buf + len),
            buflen - len, 0, 0, 0, 0, topicString, 
            (unsigned char *)payload, payloadlen);//publish msg
    
    len += MQTTSerialize_disconnect((unsigned char *)(buf + len), buflen - len);   
    transport_sendPacketBuffer((unsigned char*)buf, len); //转发
}
