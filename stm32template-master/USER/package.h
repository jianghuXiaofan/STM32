#ifndef __PACKAGE_H
#define __PACKAGE_H
#include "stm32f10x.h"

//这个结构体包含所有的信息 包括设备号 id heart 
typedef struct 
{
    uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
    
    
}MsgPack;


#endif

