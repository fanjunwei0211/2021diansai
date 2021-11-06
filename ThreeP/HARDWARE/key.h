#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "gpio.h"


//#define KEY1       HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)  //KEY1按键  增大目标值
//#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)  //KEY2按键  减小目标值 
//#define KEY3        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2) //KEY3按键   增大比例系数
//#define KEY4       HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)  //KEY4按键   减小比例系数

//#define KEY1_PRES 	1
//#define KEY2_PRES		2
//#define KEY3_PRES		3
//#define KEY4_PRES   4

int KEY_Scan(uint8_t mode);
char click(void);
void key_update(void);
void showscreen(void);
#endif
