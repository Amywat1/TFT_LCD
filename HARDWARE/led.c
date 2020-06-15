#include "header.h"

void LedInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);    //使能PB，PE端口

    GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_5;                                    //GPIOB.5（LED0）
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;                              //做推挽输出
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                          //初始化GPIOB5
    GPIO_SetBits(GPIOB, GPIO_Pin_5);                                                //输出高

    GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_5;                                    //GPIOE.5（LED1）
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);                                                //输出高
}
