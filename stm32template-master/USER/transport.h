
int transport_sendPacketBuffer(unsigned char* buf, int buflen);
int transport_getdata(unsigned char* buf, int count);
int transport_getdatanb(unsigned char* buf, int count);
int transport_open(char* host, int port);
int transport_close(void);
int my_recv(unsigned char* buf, int len);

/*************************************************************/
int MQTTPacket_My_read(unsigned char* buf, int buflen);//自己改写的函数
/*************************************************************/
