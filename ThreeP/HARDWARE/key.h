#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "gpio.h"


//#define KEY1       HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)  //KEY1����  ����Ŀ��ֵ
//#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)  //KEY2����  ��СĿ��ֵ 
//#define KEY3        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2) //KEY3����   �������ϵ��
//#define KEY4       HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)  //KEY4����   ��С����ϵ��

//#define KEY1_PRES 	1
//#define KEY2_PRES		2
//#define KEY3_PRES		3
//#define KEY4_PRES   4

int KEY_Scan(uint8_t mode);
char click(void);
void key_update(void);
void showscreen(void);
#endif
