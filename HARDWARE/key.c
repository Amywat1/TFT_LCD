#include "header.h"

void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;         //GPIOE.2~4
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IPU;                                //做上拉输入
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                                         //初始化GPIOE.2~4

    GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_0;                                   //GPIOA.0
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IPD;                                //做输入下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

u8 KeyScan(u8 mode)
{
    static u8 keyReleaseFlag    = 1;                                                //按键释放标志

    if(mode)
    {
        keyReleaseFlag  = 1;
    }

    if(keyReleaseFlag && (KEY_0 == 0 || KEY_1 == 0 || KEY_2 == 0 || KEY_3 == 1))
    {
        keyReleaseFlag  = 0;

        delay_ms(10);

        if(KEY_0 == 0)
        {
            return KEY_0_PRES;
        }
        else if(KEY_1 == 0)
        {
            return KEY_1_PRES;
        }
        else if(KEY_2 == 0)
        {
            return KEY_2_PRES;
        }
        else if(KEY_3 == 1)
        {
            return KEY_3_PRES;
        }
        else
        {
            keyReleaseFlag = 1;

            return 0;
        }
    }
    else if(KEY_0 == 1 && KEY_1 == 1 && KEY_2 == 1 && KEY_3 == 0)
    {
        keyReleaseFlag = 1;
    }
	return 0;
}
