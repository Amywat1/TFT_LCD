#include "header.h"

void BeepInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_8;               //GPIOB.8
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //做推挽输出
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);                      //初始化GPIOB.8
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);                          //初始化输出0
}
