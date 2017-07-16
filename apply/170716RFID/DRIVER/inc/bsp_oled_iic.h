#ifndef __BSP_OLED_H
#define __BSP_OLED_H			  	 

#include "stm32f10x.h"
#include "stdlib.h"	
#include "bsp_iic_A.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);//移植接口  
void OLED_Display_On(void);//显示开
void OLED_Display_Off(void);//显示关	   							   		    
void OLED_Init(void);//初始化
void OLED_Clear(void);//清屏
void OLED_Set_Pos(unsigned char x, unsigned char y);//设置坐标
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);//填充

void OLED_DrawPoint(u8 x,u8 y,u8 t);//画点(x,y)
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);//显示单个字符
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//显示n个数字
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);//显示字符串	 
void OLED_ShowCHinese(u8 x,u8 y,u8 no);//显示汉字，字模软件


//下面两个函数，不建议使用
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);


#endif
