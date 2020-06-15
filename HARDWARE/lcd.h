#ifndef _LCD_H
#define _LCD_H
#include "stm32f10x.h"

/* LCD重要参数集 */
typedef struct  
{										    
    u16 width;			//LCD 宽度
    u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wRamCmd;		//开始写gram指令
	u16 setXCmd;		//设置x坐标指令
	u16 setYCmd;		//设置y坐标指令 
}_lcdParameter;
	    
/* LCD地址结构体 */
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
}_lcdTypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((_lcdTypeDef *) LCD_BASE)

#define	LCD_LED PBout(0)		//LCD背光  PB0

//扫描方向定义
#define L2R_U2D  0				//从左到右,从上到下
#define L2R_D2U  1				//从左到右,从下到上
#define R2L_U2D  2				//从右到左,从上到下
#define R2L_D2U  3				//从右到左,从下到上

#define U2D_L2R  4				//从上到下,从左到右
#define U2D_R2L  5				//从上到下,从右到左
#define D2U_L2R  6				//从下到上,从左到右
#define D2U_R2L  7				//从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D	//默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

void LcdInit(void);

#endif
