#ifndef __I2C_H__
#define __I2C_H__
#include "reg52.h"
sbit E2P_SDA=P1^0;
sbit E2P_SCL=P1^1;
void E2P_delay(void);                      //延时
void start(void);					//启动信号子函数
void stop(void); 				//停止信号子函数
void respons(void);					//应答信号子函数
void writebyte(unsigned char  date);	//发送一字节子数据程序_WRBYT
unsigned char readbyte(void);	//接收一字节子数据程序_RDBYT
extern void write_add(unsigned char address ,unsigned char info);   //指定位置 写
extern unsigned char read_add(unsigned char address);		 //指定位置 读
extern void EEPROM_Init(void);						 //初始化
#endif