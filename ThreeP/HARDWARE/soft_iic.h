#ifndef _SOFTIIC_H
#define _SOFTIIC_H
#include "sys.h"
#include "delay.h"

//IO方向设置
#define SDA_IN()  {GPIOD->MODER&=~(3<<(10*2));GPIOD->MODER|=0<<10*2;}	//PB11输入模式
#define SDA_OUT() {GPIOD->MODER&=~(3<<(10*2));GPIOD->MODER|=1<<10*2;} //PB11输出模式
//IO操作
#define IIC_SCL   PDout(11) //SCL
#define IIC_SDA   PDout(10) //SDA
#define READ_SDA  PDin(10)  //输入SDA

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
#endif
