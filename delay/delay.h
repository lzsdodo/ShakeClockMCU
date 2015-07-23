//延时函数
#ifndef _DELAY_H_
#define _DELAY_H_
#include <reg52.h>
#include <intrins.h> 
extern void delay_10us();					//延时10us，无参数
extern void delay_1ms(unsigned int t);		//延时t*1ms
extern void delay_10ms(unsigned int t);		//延时t*10ms
extern void delay_50ms(unsigned int t);		//延时t*50ms
#endif