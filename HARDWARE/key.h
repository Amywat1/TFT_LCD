#ifndef _KEY_H_
#define _KEY_H_

#define KEY_0           (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))     //读按键0
#define KEY_1           (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3))     //读按键1
#define KEY_2           (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2))     //读按键2
#define KEY_3           (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))     //读按键3

#define KEY_0_PRES      1
#define KEY_1_PRES      2
#define KEY_2_PRES      3
#define KEY_3_PRES      4

void KeyInit(void);
u8 KeyScan(u8 mode);

#endif
