#include "header.h"

int main(void)
{
//	u8 stepCnt = 0;
//	u8 LcdId_Buff[12];
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	LedInit();
	LCD_Init();
}
