#ifndef __BSP_OLED_H
#define __BSP_OLED_H			  	 

#include "stm32f10x.h"

//#include "stdlib.h"	


#define SIZE 16
//#define XLevelL		0x00
//#define XLevelH		0x10
#define Max_Column	128
//#define Max_Row		64
//#define	Brightness	0xFF 
//#define X_WIDTH 	128
//#define Y_WIDTH 	64	   

//端口宏定义
#define OLED_SCLK_CLK      RCC_APB2Periph_GPIOC
#define OLED_SCLK_PIN      GPIO_Pin_10
#define OLED_SCLK_PORT     GPIOC

#define OLED_SDIN_CLK      RCC_APB2Periph_GPIOB
#define OLED_SDIN_PIN      GPIO_Pin_15
#define OLED_SDIN_PORT     GPIOB

#define OLED_RST_CLK      RCC_APB2Periph_GPIOB
#define OLED_RST_PIN      GPIO_Pin_13
#define OLED_RST_PORT     GPIOB

#define OLED_DC_CLK      RCC_APB2Periph_GPIOB
#define OLED_DC_PIN      GPIO_Pin_11
#define OLED_DC_PORT     GPIOB
                    
#define OLED_SCLK_Clr() GPIO_ResetBits(OLED_SCLK_PORT,OLED_SCLK_PIN)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(OLED_SCLK_PORT,OLED_SCLK_PIN)

#define OLED_SDIN_Clr() GPIO_ResetBits(OLED_SDIN_PORT,OLED_SDIN_PIN)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(OLED_SDIN_PORT,OLED_SDIN_PIN)

#define OLED_RST_Clr() GPIO_ResetBits(OLED_RST_PORT,OLED_RST_PIN)//RES
#define OLED_RST_Set() GPIO_SetBits(OLED_RST_PORT,OLED_RST_PIN)

#define OLED_DC_Clr() GPIO_ResetBits(OLED_DC_PORT,OLED_DC_PIN)//DC
#define OLED_DC_Set() GPIO_SetBits(OLED_DC_PORT,OLED_DC_PIN)
 		     
//#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_12)//CS 默认拉低了
//#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_12)


 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif
