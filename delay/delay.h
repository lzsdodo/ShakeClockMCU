//��ʱ����
#ifndef _DELAY_H_
#define _DELAY_H_
#include <reg52.h>
#include <intrins.h> 
extern void delay_10us();					//��ʱ10us���޲���
extern void delay_1ms(unsigned int t);		//��ʱt*1ms
extern void delay_10ms(unsigned int t);		//��ʱt*10ms
extern void delay_50ms(unsigned int t);		//��ʱt*50ms
#endif