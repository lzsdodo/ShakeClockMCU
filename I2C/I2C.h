#ifndef __I2C_H__
#define __I2C_H__
#include "reg52.h"
sbit E2P_SDA=P1^0;
sbit E2P_SCL=P1^1;
void E2P_delay(void);                      //��ʱ
void start(void);					//�����ź��Ӻ���
void stop(void); 				//ֹͣ�ź��Ӻ���
void respons(void);					//Ӧ���ź��Ӻ���
void writebyte(unsigned char  date);	//����һ�ֽ������ݳ���_WRBYT
unsigned char readbyte(void);	//����һ�ֽ������ݳ���_RDBYT
extern void write_add(unsigned char address ,unsigned char info);   //ָ��λ�� д
extern unsigned char read_add(unsigned char address);		 //ָ��λ�� ��
extern void EEPROM_Init(void);						 //��ʼ��
#endif