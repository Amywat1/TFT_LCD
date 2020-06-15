#include "header.h"

_lcdParameter lcdParamater;

/* 写寄存器 */
void LcdWriteReg(u16 regval)
{   
	LCD->LCD_REG = regval;		//写入要写的寄存器序号
}

/* 写寄存器
LCD_Reg:寄存器地址
LCD_RegValue:要写入的数据 */
void LcdWriteRegData(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}

/* 读LCD数据 */
u16 LcdReadData(void)
{
	vu16 ram;					//防止被优化
	ram = LCD->LCD_RAM;
	return ram;
}

/* 读寄存器 */
u16 LcdReadReg(u16 LCD_Reg)
{										   
	LcdWriteReg(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LcdReadData();		//返回读到的值
}

/* 写LCD数据 */
void LcdWriteData(u16 data)
{	 
	LCD->LCD_RAM = data;
}

/* 设置LCD的自动扫描方向 */
void LcdScanDir(u8 dir)
{
	u16 regval = 0;
	u16 dirreg = 0;

	if(lcdParamater.id == 0X5510)						//9341/6804/5310/5510/1963,特殊处理
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}

		if(lcdParamater.id==0X5510)
		{
			dirreg=0X3600;
		}
		else
		{
			dirreg=0X36;
		}
 		regval |= 0X08;									//5310/5510/1963不需要BGR   
	   
		LcdWriteRegData(dirreg,regval);

		if(lcdParamater.id==0X5510)
		{
			LcdWriteReg(lcdParamater.setXCmd);LcdWriteData(0); 
			LcdWriteReg(lcdParamater.setXCmd+1);LcdWriteData(0); 
			LcdWriteReg(lcdParamater.setXCmd+2);LcdWriteData((lcdParamater.width-1)>>8); 
			LcdWriteReg(lcdParamater.setXCmd+3);LcdWriteData((lcdParamater.width-1)&0XFF); 
			LcdWriteReg(lcdParamater.setYCmd);LcdWriteData(0); 
			LcdWriteReg(lcdParamater.setYCmd+1);LcdWriteData(0); 
			LcdWriteReg(lcdParamater.setYCmd+2);LcdWriteData((lcdParamater.height-1)>>8); 
			LcdWriteReg(lcdParamater.setYCmd+3);LcdWriteData((lcdParamater.height-1)&0XFF);
		}
		else
		{
			LcdWriteReg(lcdParamater.setXCmd); 
			LcdWriteData(0);LcdWriteData(0);
			LcdWriteData((lcdParamater.width-1)>>8);LcdWriteData((lcdParamater.width-1)&0XFF);
			LcdWriteReg(lcdParamater.setYCmd); 
			LcdWriteData(0);LcdWriteData(0);
			LcdWriteData((lcdParamater.height-1)>>8);LcdWriteData((lcdParamater.height-1)&0XFF);  
		}
  	}
	else 
	{

	}
}

/* 设置LCD显示方向
dir:0,竖屏；1,横屏 */
void LcdDisplayDir(u8 dir)
{
	if(dir == 0)							//竖屏
	{
		lcdParamater.dir	= 0;			//竖屏
		lcdParamater.width	= 240;
		lcdParamater.height	= 320;

		if(lcdParamater.id == 0x5510)
		{
			lcdParamater.wRamCmd = 0X2C00;
	 		lcdParamater.setXCmd = 0X2A00;
			lcdParamater.setYCmd = 0X2B00; 
			lcdParamater.width	 = 480;
			lcdParamater.height	 = 800;
		}
		else
		{
			lcdParamater.wRamCmd = 0X22;
	 		lcdParamater.setXCmd = 0X20;
			lcdParamater.setYCmd = 0X21;
		}
	}
	else 									//横屏
	{	  				
		lcdParamater.dir	= 1;			//横屏
		lcdParamater.width	= 320;
		lcdParamater.height	= 240;
		if(lcdParamater.id == 0x5510)
		{
			lcdParamater.wRamCmd = 0X2C00;
	 		lcdParamater.setXCmd = 0X2A00;
			lcdParamater.setYCmd = 0X2B00; 
			lcdParamater.width	 = 800;
			lcdParamater.height  = 480;
		}
		else
		{
			lcdParamater.wRamCmd = 0X22;
	 		lcdParamater.setXCmd = 0X21;
			lcdParamater.setYCmd = 0X20;  
		}
	} 

	LcdScanDir(DFT_SCAN_DIR);	//默认扫描方向
}

/* 开始写GRAM */
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG = lcdParamater.wRamCmd;	  
}

/* 设置光标位置
Xpos:横坐标
Ypos:纵坐标 */
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LcdWriteReg(lcdParamater.setXCmd);LcdWriteData(Xpos>>8);
	LcdWriteReg(lcdParamater.setXCmd+1);LcdWriteData(Xpos&0XFF);
	LcdWriteReg(lcdParamater.setYCmd);LcdWriteData(Ypos>>8);
	LcdWriteReg(lcdParamater.setYCmd+1);LcdWriteData(Ypos&0XFF);
}

/* 清屏函数
color:要清屏的填充色 */
void LCD_Clear(u16 color)
{
	u32 index = 0;      
	u32 totalpoint = lcdParamater.width;
	totalpoint *= lcdParamater.height; 			//得到总点数
	
	LCD_SetCursor(0x00,0x0000);					//设置光标位置 
	LCD_WriteRAM_Prepare();						//开始写入GRAM

	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM = color;	
	}
}

void LcdInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef readWriteTiming;
	FSMC_NORSRAMTimingInitTypeDef writeTiming;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);                       //使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | \
	RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG,ENABLE);                    //使能GPIOB、D、E、G时钟

	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PORTD复用推挽输出
	GPIO_InitStructure.GPIO_Pin     = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | \
									   GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;						//复用推挽
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	//PORTE复用推挽输出
	GPIO_InitStructure.GPIO_Pin     = (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
									   GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;						//复用推挽
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//PORTG复用推挽输出
	GPIO_InitStructure.GPIO_Pin     = (GPIO_Pin_0 | GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;						//复用推挽
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	readWriteTiming.FSMC_AddressSetupTime	= 0x01;							//地址建立时间（ADDSET）为2个HCLK 1/72M
	readWriteTiming.FSMC_AddressHoldTime	= 0;							//地址保持时间（ADDHLD）模式A未用到
	readWriteTiming.FSMC_DataSetupTime		= 0x0f;							//数据保存时间为16个HCLK
	readWriteTiming.FSMC_BusTurnAroundDuration	= 0;
	readWriteTiming.FSMC_CLKDivision		= 0;
	readWriteTiming.FSMC_DataLatency		= 0;
	readWriteTiming.FSMC_AccessMode			= FSMC_AccessMode_A;			//模式A

	writeTiming.FSMC_AddressSetupTime		= 0x00;							//地址建立时间（ADDSET）为1个HCLK 1/72M
	writeTiming.FSMC_AddressHoldTime		= 0;							//地址保持时间（ADDHLD）模式A未用到
	writeTiming.FSMC_DataSetupTime			= 0x03;							//数据保存时间为4个HCLK
	writeTiming.FSMC_BusTurnAroundDuration	= 0;
	writeTiming.FSMC_CLKDivision			= 0;
	writeTiming.FSMC_DataLatency			= 0;
	writeTiming.FSMC_AccessMode				= FSMC_AccessMode_A;			//模式A

	FSMC_NORSRAMInitStructure.FSMC_Bank		= FSMC_Bank1_NORSRAM4;			//选择区4
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux	= FSMC_DataAddressMux_Disable;	//不服用地址
	FSMC_NORSRAMInitStructure.FSMC_MemoryType		= FSMC_MemoryType_SRAM;			// FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth	= FSMC_MemoryDataWidth_16b;		//存储器数据宽度为16bit   
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode	= FSMC_BurstAccessMode_Disable;	// FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity	= FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait		=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode	= FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive	= FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation	= FSMC_WriteOperation_Enable;	//  存储器写使能
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal	= FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode	= FSMC_ExtendedMode_Enable; 		// 读写使用不同的时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst	= FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct= &readWriteTiming; 		//读写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct	= &writeTiming;  			//写时序

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);									//初始化FSMC配置
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4,ENABLE);									//使能NORSRAM的bank1,4区块

	delay_ms(50);
	lcdParamater.id = LcdReadReg(0x0000);					//读ID
	if(lcdParamater.id<0XFF||lcdParamater.id==0XFFFF||lcdParamater.id==0X9300)//读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300
	{
		LcdWriteReg(0XDA00);	
		lcdParamater.id=LcdReadData();		//读回0X00	 
		LcdWriteReg(0XDB00);	
		lcdParamater.id=LcdReadData();		//读回0X80
		lcdParamater.id<<=8;	
		LcdWriteReg(0XDC00);	
		lcdParamater.id|=LcdReadData();		//读回0X00		
		if(lcdParamater.id==0x8000)
		{
			lcdParamater.id=0x5510;//NT35510读回的ID是8000H,为方便区分,我们强制设置为5510
		}
	}

	printf(" LCD ID:%x\r\n",lcdParamater.id);				//打印LCD ID 

	if(lcdParamater.id == 0x5510)
	{
		LcdWriteRegData(0xF000,0x55);
		LcdWriteRegData(0xF001,0xAA);
		LcdWriteRegData(0xF002,0x52);
		LcdWriteRegData(0xF003,0x08);
		LcdWriteRegData(0xF004,0x01);
		//AVDD Set AVDD 5.2V
		LcdWriteRegData(0xB000,0x0D);
		LcdWriteRegData(0xB001,0x0D);
		LcdWriteRegData(0xB002,0x0D);
		//AVDD ratio
		LcdWriteRegData(0xB600,0x34);
		LcdWriteRegData(0xB601,0x34);
		LcdWriteRegData(0xB602,0x34);
		//AVEE -5.2V
		LcdWriteRegData(0xB100,0x0D);
		LcdWriteRegData(0xB101,0x0D);
		LcdWriteRegData(0xB102,0x0D);
		//AVEE ratio
		LcdWriteRegData(0xB700,0x34);
		LcdWriteRegData(0xB701,0x34);
		LcdWriteRegData(0xB702,0x34);
		//VCL -2.5V
		LcdWriteRegData(0xB200,0x00);
		LcdWriteRegData(0xB201,0x00);
		LcdWriteRegData(0xB202,0x00);
		//VCL ratio
		LcdWriteRegData(0xB800,0x24);
		LcdWriteRegData(0xB801,0x24);
		LcdWriteRegData(0xB802,0x24);
		//VGH 15V (Free pump)
		LcdWriteRegData(0xBF00,0x01);
		LcdWriteRegData(0xB300,0x0F);
		LcdWriteRegData(0xB301,0x0F);
		LcdWriteRegData(0xB302,0x0F);
		//VGH ratio
		LcdWriteRegData(0xB900,0x34);
		LcdWriteRegData(0xB901,0x34);
		LcdWriteRegData(0xB902,0x34);
		//VGL_REG -10V
		LcdWriteRegData(0xB500,0x08);
		LcdWriteRegData(0xB501,0x08);
		LcdWriteRegData(0xB502,0x08);
		LcdWriteRegData(0xC200,0x03);
		//VGLX ratio
		LcdWriteRegData(0xBA00,0x24);
		LcdWriteRegData(0xBA01,0x24);
		LcdWriteRegData(0xBA02,0x24);
		//VGMP/VGSP 4.5V/0V
		LcdWriteRegData(0xBC00,0x00);
		LcdWriteRegData(0xBC01,0x78);
		LcdWriteRegData(0xBC02,0x00);
		//VGMN/VGSN -4.5V/0V
		LcdWriteRegData(0xBD00,0x00);
		LcdWriteRegData(0xBD01,0x78);
		LcdWriteRegData(0xBD02,0x00);
		//VCOM
		LcdWriteRegData(0xBE00,0x00);
		LcdWriteRegData(0xBE01,0x64);
		//Gamma Setting
		LcdWriteRegData(0xD100,0x00);
		LcdWriteRegData(0xD101,0x33);
		LcdWriteRegData(0xD102,0x00);
		LcdWriteRegData(0xD103,0x34);
		LcdWriteRegData(0xD104,0x00);
		LcdWriteRegData(0xD105,0x3A);
		LcdWriteRegData(0xD106,0x00);
		LcdWriteRegData(0xD107,0x4A);
		LcdWriteRegData(0xD108,0x00);
		LcdWriteRegData(0xD109,0x5C);
		LcdWriteRegData(0xD10A,0x00);
		LcdWriteRegData(0xD10B,0x81);
		LcdWriteRegData(0xD10C,0x00);
		LcdWriteRegData(0xD10D,0xA6);
		LcdWriteRegData(0xD10E,0x00);
		LcdWriteRegData(0xD10F,0xE5);
		LcdWriteRegData(0xD110,0x01);
		LcdWriteRegData(0xD111,0x13);
		LcdWriteRegData(0xD112,0x01);
		LcdWriteRegData(0xD113,0x54);
		LcdWriteRegData(0xD114,0x01);
		LcdWriteRegData(0xD115,0x82);
		LcdWriteRegData(0xD116,0x01);
		LcdWriteRegData(0xD117,0xCA);
		LcdWriteRegData(0xD118,0x02);
		LcdWriteRegData(0xD119,0x00);
		LcdWriteRegData(0xD11A,0x02);
		LcdWriteRegData(0xD11B,0x01);
		LcdWriteRegData(0xD11C,0x02);
		LcdWriteRegData(0xD11D,0x34);
		LcdWriteRegData(0xD11E,0x02);
		LcdWriteRegData(0xD11F,0x67);
		LcdWriteRegData(0xD120,0x02);
		LcdWriteRegData(0xD121,0x84);
		LcdWriteRegData(0xD122,0x02);
		LcdWriteRegData(0xD123,0xA4);
		LcdWriteRegData(0xD124,0x02);
		LcdWriteRegData(0xD125,0xB7);
		LcdWriteRegData(0xD126,0x02);
		LcdWriteRegData(0xD127,0xCF);
		LcdWriteRegData(0xD128,0x02);
		LcdWriteRegData(0xD129,0xDE);
		LcdWriteRegData(0xD12A,0x02);
		LcdWriteRegData(0xD12B,0xF2);
		LcdWriteRegData(0xD12C,0x02);
		LcdWriteRegData(0xD12D,0xFE);
		LcdWriteRegData(0xD12E,0x03);
		LcdWriteRegData(0xD12F,0x10);
		LcdWriteRegData(0xD130,0x03);
		LcdWriteRegData(0xD131,0x33);
		LcdWriteRegData(0xD132,0x03);
		LcdWriteRegData(0xD133,0x6D);
		LcdWriteRegData(0xD200,0x00);
		LcdWriteRegData(0xD201,0x33);
		LcdWriteRegData(0xD202,0x00);
		LcdWriteRegData(0xD203,0x34);
		LcdWriteRegData(0xD204,0x00);
		LcdWriteRegData(0xD205,0x3A);
		LcdWriteRegData(0xD206,0x00);
		LcdWriteRegData(0xD207,0x4A);
		LcdWriteRegData(0xD208,0x00);
		LcdWriteRegData(0xD209,0x5C);
		LcdWriteRegData(0xD20A,0x00);

		LcdWriteRegData(0xD20B,0x81);
		LcdWriteRegData(0xD20C,0x00);
		LcdWriteRegData(0xD20D,0xA6);
		LcdWriteRegData(0xD20E,0x00);
		LcdWriteRegData(0xD20F,0xE5);
		LcdWriteRegData(0xD210,0x01);
		LcdWriteRegData(0xD211,0x13);
		LcdWriteRegData(0xD212,0x01);
		LcdWriteRegData(0xD213,0x54);
		LcdWriteRegData(0xD214,0x01);
		LcdWriteRegData(0xD215,0x82);
		LcdWriteRegData(0xD216,0x01);
		LcdWriteRegData(0xD217,0xCA);
		LcdWriteRegData(0xD218,0x02);
		LcdWriteRegData(0xD219,0x00);
		LcdWriteRegData(0xD21A,0x02);
		LcdWriteRegData(0xD21B,0x01);
		LcdWriteRegData(0xD21C,0x02);
		LcdWriteRegData(0xD21D,0x34);
		LcdWriteRegData(0xD21E,0x02);
		LcdWriteRegData(0xD21F,0x67);
		LcdWriteRegData(0xD220,0x02);
		LcdWriteRegData(0xD221,0x84);
		LcdWriteRegData(0xD222,0x02);
		LcdWriteRegData(0xD223,0xA4);
		LcdWriteRegData(0xD224,0x02);
		LcdWriteRegData(0xD225,0xB7);
		LcdWriteRegData(0xD226,0x02);
		LcdWriteRegData(0xD227,0xCF);
		LcdWriteRegData(0xD228,0x02);
		LcdWriteRegData(0xD229,0xDE);
		LcdWriteRegData(0xD22A,0x02);
		LcdWriteRegData(0xD22B,0xF2);
		LcdWriteRegData(0xD22C,0x02);
		LcdWriteRegData(0xD22D,0xFE);
		LcdWriteRegData(0xD22E,0x03);
		LcdWriteRegData(0xD22F,0x10);
		LcdWriteRegData(0xD230,0x03);
		LcdWriteRegData(0xD231,0x33);
		LcdWriteRegData(0xD232,0x03);
		LcdWriteRegData(0xD233,0x6D);
		LcdWriteRegData(0xD300,0x00);
		LcdWriteRegData(0xD301,0x33);
		LcdWriteRegData(0xD302,0x00);
		LcdWriteRegData(0xD303,0x34);
		LcdWriteRegData(0xD304,0x00);
		LcdWriteRegData(0xD305,0x3A);
		LcdWriteRegData(0xD306,0x00);
		LcdWriteRegData(0xD307,0x4A);
		LcdWriteRegData(0xD308,0x00);
		LcdWriteRegData(0xD309,0x5C);
		LcdWriteRegData(0xD30A,0x00);

		LcdWriteRegData(0xD30B,0x81);
		LcdWriteRegData(0xD30C,0x00);
		LcdWriteRegData(0xD30D,0xA6);
		LcdWriteRegData(0xD30E,0x00);
		LcdWriteRegData(0xD30F,0xE5);
		LcdWriteRegData(0xD310,0x01);
		LcdWriteRegData(0xD311,0x13);
		LcdWriteRegData(0xD312,0x01);
		LcdWriteRegData(0xD313,0x54);
		LcdWriteRegData(0xD314,0x01);
		LcdWriteRegData(0xD315,0x82);
		LcdWriteRegData(0xD316,0x01);
		LcdWriteRegData(0xD317,0xCA);
		LcdWriteRegData(0xD318,0x02);
		LcdWriteRegData(0xD319,0x00);
		LcdWriteRegData(0xD31A,0x02);
		LcdWriteRegData(0xD31B,0x01);
		LcdWriteRegData(0xD31C,0x02);
		LcdWriteRegData(0xD31D,0x34);
		LcdWriteRegData(0xD31E,0x02);
		LcdWriteRegData(0xD31F,0x67);
		LcdWriteRegData(0xD320,0x02);
		LcdWriteRegData(0xD321,0x84);
		LcdWriteRegData(0xD322,0x02);
		LcdWriteRegData(0xD323,0xA4);
		LcdWriteRegData(0xD324,0x02);
		LcdWriteRegData(0xD325,0xB7);
		LcdWriteRegData(0xD326,0x02);
		LcdWriteRegData(0xD327,0xCF);
		LcdWriteRegData(0xD328,0x02);
		LcdWriteRegData(0xD329,0xDE);
		LcdWriteRegData(0xD32A,0x02);
		LcdWriteRegData(0xD32B,0xF2);
		LcdWriteRegData(0xD32C,0x02);
		LcdWriteRegData(0xD32D,0xFE);
		LcdWriteRegData(0xD32E,0x03);
		LcdWriteRegData(0xD32F,0x10);
		LcdWriteRegData(0xD330,0x03);
		LcdWriteRegData(0xD331,0x33);
		LcdWriteRegData(0xD332,0x03);
		LcdWriteRegData(0xD333,0x6D);
		LcdWriteRegData(0xD400,0x00);
		LcdWriteRegData(0xD401,0x33);
		LcdWriteRegData(0xD402,0x00);
		LcdWriteRegData(0xD403,0x34);
		LcdWriteRegData(0xD404,0x00);
		LcdWriteRegData(0xD405,0x3A);
		LcdWriteRegData(0xD406,0x00);
		LcdWriteRegData(0xD407,0x4A);
		LcdWriteRegData(0xD408,0x00);
		LcdWriteRegData(0xD409,0x5C);
		LcdWriteRegData(0xD40A,0x00);
		LcdWriteRegData(0xD40B,0x81);

		LcdWriteRegData(0xD40C,0x00);
		LcdWriteRegData(0xD40D,0xA6);
		LcdWriteRegData(0xD40E,0x00);
		LcdWriteRegData(0xD40F,0xE5);
		LcdWriteRegData(0xD410,0x01);
		LcdWriteRegData(0xD411,0x13);
		LcdWriteRegData(0xD412,0x01);
		LcdWriteRegData(0xD413,0x54);
		LcdWriteRegData(0xD414,0x01);
		LcdWriteRegData(0xD415,0x82);
		LcdWriteRegData(0xD416,0x01);
		LcdWriteRegData(0xD417,0xCA);
		LcdWriteRegData(0xD418,0x02);
		LcdWriteRegData(0xD419,0x00);
		LcdWriteRegData(0xD41A,0x02);
		LcdWriteRegData(0xD41B,0x01);
		LcdWriteRegData(0xD41C,0x02);
		LcdWriteRegData(0xD41D,0x34);
		LcdWriteRegData(0xD41E,0x02);
		LcdWriteRegData(0xD41F,0x67);
		LcdWriteRegData(0xD420,0x02);
		LcdWriteRegData(0xD421,0x84);
		LcdWriteRegData(0xD422,0x02);
		LcdWriteRegData(0xD423,0xA4);
		LcdWriteRegData(0xD424,0x02);
		LcdWriteRegData(0xD425,0xB7);
		LcdWriteRegData(0xD426,0x02);
		LcdWriteRegData(0xD427,0xCF);
		LcdWriteRegData(0xD428,0x02);
		LcdWriteRegData(0xD429,0xDE);
		LcdWriteRegData(0xD42A,0x02);
		LcdWriteRegData(0xD42B,0xF2);
		LcdWriteRegData(0xD42C,0x02);
		LcdWriteRegData(0xD42D,0xFE);
		LcdWriteRegData(0xD42E,0x03);
		LcdWriteRegData(0xD42F,0x10);
		LcdWriteRegData(0xD430,0x03);
		LcdWriteRegData(0xD431,0x33);
		LcdWriteRegData(0xD432,0x03);
		LcdWriteRegData(0xD433,0x6D);
		LcdWriteRegData(0xD500,0x00);
		LcdWriteRegData(0xD501,0x33);
		LcdWriteRegData(0xD502,0x00);
		LcdWriteRegData(0xD503,0x34);
		LcdWriteRegData(0xD504,0x00);
		LcdWriteRegData(0xD505,0x3A);
		LcdWriteRegData(0xD506,0x00);
		LcdWriteRegData(0xD507,0x4A);
		LcdWriteRegData(0xD508,0x00);
		LcdWriteRegData(0xD509,0x5C);
		LcdWriteRegData(0xD50A,0x00);
		LcdWriteRegData(0xD50B,0x81);

		LcdWriteRegData(0xD50C,0x00);
		LcdWriteRegData(0xD50D,0xA6);
		LcdWriteRegData(0xD50E,0x00);
		LcdWriteRegData(0xD50F,0xE5);
		LcdWriteRegData(0xD510,0x01);
		LcdWriteRegData(0xD511,0x13);
		LcdWriteRegData(0xD512,0x01);
		LcdWriteRegData(0xD513,0x54);
		LcdWriteRegData(0xD514,0x01);
		LcdWriteRegData(0xD515,0x82);
		LcdWriteRegData(0xD516,0x01);
		LcdWriteRegData(0xD517,0xCA);
		LcdWriteRegData(0xD518,0x02);
		LcdWriteRegData(0xD519,0x00);
		LcdWriteRegData(0xD51A,0x02);
		LcdWriteRegData(0xD51B,0x01);
		LcdWriteRegData(0xD51C,0x02);
		LcdWriteRegData(0xD51D,0x34);
		LcdWriteRegData(0xD51E,0x02);
		LcdWriteRegData(0xD51F,0x67);
		LcdWriteRegData(0xD520,0x02);
		LcdWriteRegData(0xD521,0x84);
		LcdWriteRegData(0xD522,0x02);
		LcdWriteRegData(0xD523,0xA4);
		LcdWriteRegData(0xD524,0x02);
		LcdWriteRegData(0xD525,0xB7);
		LcdWriteRegData(0xD526,0x02);
		LcdWriteRegData(0xD527,0xCF);
		LcdWriteRegData(0xD528,0x02);
		LcdWriteRegData(0xD529,0xDE);
		LcdWriteRegData(0xD52A,0x02);
		LcdWriteRegData(0xD52B,0xF2);
		LcdWriteRegData(0xD52C,0x02);
		LcdWriteRegData(0xD52D,0xFE);
		LcdWriteRegData(0xD52E,0x03);
		LcdWriteRegData(0xD52F,0x10);
		LcdWriteRegData(0xD530,0x03);
		LcdWriteRegData(0xD531,0x33);
		LcdWriteRegData(0xD532,0x03);
		LcdWriteRegData(0xD533,0x6D);
		LcdWriteRegData(0xD600,0x00);
		LcdWriteRegData(0xD601,0x33);
		LcdWriteRegData(0xD602,0x00);
		LcdWriteRegData(0xD603,0x34);
		LcdWriteRegData(0xD604,0x00);
		LcdWriteRegData(0xD605,0x3A);
		LcdWriteRegData(0xD606,0x00);
		LcdWriteRegData(0xD607,0x4A);
		LcdWriteRegData(0xD608,0x00);
		LcdWriteRegData(0xD609,0x5C);
		LcdWriteRegData(0xD60A,0x00);
		LcdWriteRegData(0xD60B,0x81);

		LcdWriteRegData(0xD60C,0x00);
		LcdWriteRegData(0xD60D,0xA6);
		LcdWriteRegData(0xD60E,0x00);
		LcdWriteRegData(0xD60F,0xE5);
		LcdWriteRegData(0xD610,0x01);
		LcdWriteRegData(0xD611,0x13);
		LcdWriteRegData(0xD612,0x01);
		LcdWriteRegData(0xD613,0x54);
		LcdWriteRegData(0xD614,0x01);
		LcdWriteRegData(0xD615,0x82);
		LcdWriteRegData(0xD616,0x01);
		LcdWriteRegData(0xD617,0xCA);
		LcdWriteRegData(0xD618,0x02);
		LcdWriteRegData(0xD619,0x00);
		LcdWriteRegData(0xD61A,0x02);
		LcdWriteRegData(0xD61B,0x01);
		LcdWriteRegData(0xD61C,0x02);
		LcdWriteRegData(0xD61D,0x34);
		LcdWriteRegData(0xD61E,0x02);
		LcdWriteRegData(0xD61F,0x67);
		LcdWriteRegData(0xD620,0x02);
		LcdWriteRegData(0xD621,0x84);
		LcdWriteRegData(0xD622,0x02);
		LcdWriteRegData(0xD623,0xA4);
		LcdWriteRegData(0xD624,0x02);
		LcdWriteRegData(0xD625,0xB7);
		LcdWriteRegData(0xD626,0x02);
		LcdWriteRegData(0xD627,0xCF);
		LcdWriteRegData(0xD628,0x02);
		LcdWriteRegData(0xD629,0xDE);
		LcdWriteRegData(0xD62A,0x02);
		LcdWriteRegData(0xD62B,0xF2);
		LcdWriteRegData(0xD62C,0x02);
		LcdWriteRegData(0xD62D,0xFE);
		LcdWriteRegData(0xD62E,0x03);
		LcdWriteRegData(0xD62F,0x10);
		LcdWriteRegData(0xD630,0x03);
		LcdWriteRegData(0xD631,0x33);
		LcdWriteRegData(0xD632,0x03);
		LcdWriteRegData(0xD633,0x6D);
		//LV2 Page 0 enable
		LcdWriteRegData(0xF000,0x55);
		LcdWriteRegData(0xF001,0xAA);
		LcdWriteRegData(0xF002,0x52);
		LcdWriteRegData(0xF003,0x08);
		LcdWriteRegData(0xF004,0x00);
		//Display control
		LcdWriteRegData(0xB100, 0xCC);
		LcdWriteRegData(0xB101, 0x00);
		//Source hold time
		LcdWriteRegData(0xB600,0x05);
		//Gate EQ control
		LcdWriteRegData(0xB700,0x70);
		LcdWriteRegData(0xB701,0x70);
		//Source EQ control (Mode 2)
		LcdWriteRegData(0xB800,0x01);
		LcdWriteRegData(0xB801,0x03);
		LcdWriteRegData(0xB802,0x03);
		LcdWriteRegData(0xB803,0x03);
		//Inversion mode (2-dot)
		LcdWriteRegData(0xBC00,0x02);
		LcdWriteRegData(0xBC01,0x00);
		LcdWriteRegData(0xBC02,0x00);
		//Timing control 4H w/ 4-delay
		LcdWriteRegData(0xC900,0xD0);
		LcdWriteRegData(0xC901,0x02);
		LcdWriteRegData(0xC902,0x50);
		LcdWriteRegData(0xC903,0x50);
		LcdWriteRegData(0xC904,0x50);
		LcdWriteRegData(0x3500,0x00);
		LcdWriteRegData(0x3A00,0x55);  //16-bit/pixel
		LcdWriteReg(0x1100);
		delay_us(120);
		LcdWriteReg(0x2900);
	}

	LcdDisplayDir(0);		//默认为竖屏
	LCD_LED	= 1;			//点亮背光
	LCD_Clear(WHITE);
}
