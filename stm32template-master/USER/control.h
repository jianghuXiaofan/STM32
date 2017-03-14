#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f10x.h"

#include "MQTTPacket.h"

int MQTT_Connect(void);//unsigned char* buf, int buflen, MQTTPacket_connectData* data
int MQTT_Subscribe(void);//unsigned char* buf,int buflen,MQTTString topicString,int msgid,int req_qos
#endif

