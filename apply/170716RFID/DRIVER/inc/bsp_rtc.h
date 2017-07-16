#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "stm32f10x.h"

struct _TIMS
{
    uint16_t year;
    uint8_t month;
    uint8_t week;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
};

extern struct _TIMS calender;//全局结构体
uint8_t RTC_Init(void);
uint8_t RTC_get(void);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);

#endif
