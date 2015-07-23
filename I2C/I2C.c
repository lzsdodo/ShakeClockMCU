#include <reg52.h>
#include <intrins.h>
#include "i2c.h"

#define uchar unsigned char



void E2P_delay()
{
	_nop_(); 
 }
void EEPROM_Init()
{
	E2P_SCL=1;
	E2P_delay();
	E2P_SDA = 1;
	E2P_delay();
}
void start()
{
	E2P_SDA	=	1;
	E2P_delay();
	E2P_SCL=1;
	E2P_delay();
	E2P_SDA=0;
	E2P_delay();
}

void stop()
{
	E2P_SDA=0;
	E2P_delay();
	E2P_SCL=1;
	E2P_delay();
	E2P_SDA=1;
	E2P_delay();
}

void respons()
{
	uchar i=0;
	E2P_SCL=1;
	E2P_delay();
	while((E2P_SDA==1)&&(i<255))
				i++;
	E2P_SCL=0;
	E2P_delay();
}

void writebyte(uchar date)
{
	uchar i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		E2P_SCL=0;
		E2P_delay();
		E2P_SDA=CY;
		E2P_delay();
		E2P_SCL=1;
		E2P_delay();
	}
	E2P_SCL=0;
	E2P_delay();
	E2P_SDA=1;
	E2P_delay();
}

uchar readbyte()
{
	uchar i,j,k;
	E2P_SCL=0;
	E2P_delay();
	E2P_SDA=1;
	for(i=0;i<8;i++)
	{
		E2P_SCL=1;
		E2P_delay();
		if(E2P_SDA==1)
		 	j=1;
		else
			j=0;
		k=(k<<1)|j;
		E2P_SCL=0;
		E2P_delay();
	}
	E2P_delay();
	return k;
}

void write_add(uchar address,uchar info)
{
	start();
	writebyte(0xa0);
	respons();
	writebyte(address);
	respons();
	writebyte(info);
	respons();
	stop();
}

uchar read_add(uchar address)
{
	uchar dd;
	start();
	writebyte(0xa0);
	respons();
	writebyte(address);
	respons();
	start();
	writebyte(0xa1);
	respons();
	dd=readbyte();
	stop();
	return dd;
}

