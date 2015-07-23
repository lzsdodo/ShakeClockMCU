//晶振为11.0592时的延时程序
#include <reg52.h>
#include <intrins.h>
#include "DELAY.h"

void delay_10us()						//延时10us，无参数
{
	_nop_(); 
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

void delay_1ms(unsigned int t) 			//延时t*1ms
{  
	unsigned int i; 
	for(i=0;i<t;i++) 
	{  
		TMOD=0x01; 						//定时器0，工作在模式1    
		TF0=0; 							//清溢出标志位，软清零
		TH0=0xfc; 
		TL0=0x66;  
		TR0=1; 							//启动定时器0    
		while(TF0==0){;} 				//定时器无溢出，循环等待    
		TR0=0; 							//关闭定时器0    
	} 
}
void delay_10ms(unsigned int t) 		//延时t*10ms
{  
	unsigned int i; 
	for(i=0;i<t;i++) 
	{  
		TMOD=0x01; 						//定时器0，工作在模式1    
		TF0=0; 							//清溢出标志位，软清零    
		TH0=0xdc; 
		TL0=0x0;  
		TR0=1; 							//启动定时器0    
		while(TF0==0){;} 				//定时器无溢出，循环等待    
		TR0=0; 							//关闭定时器0    
	} 
}
void delay_50ms(unsigned int t) 	    //延时t*50ms
{  
	unsigned int i; 
	for(i=0;i<t;i++) 
	{  
		TMOD=0x01; 						//定时器0，工作在模式1    
		TF0=0; 							//清溢出标志位，软清零    
		TH0=0x4c;
		TL0=0x0;  
		TR0=1; 							//启动定时器0    
		while(TF0==0){;} 				//定时器无溢出，循环等待    
		TR0=0; 							//关闭定时器0    
	} 
}
