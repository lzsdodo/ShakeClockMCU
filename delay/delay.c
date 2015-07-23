//����Ϊ11.0592ʱ����ʱ����
#include <reg52.h>
#include <intrins.h>
#include "DELAY.h"

void delay_10us()						//��ʱ10us���޲���
{
	_nop_(); 
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

void delay_1ms(unsigned int t) 			//��ʱt*1ms
{  
	unsigned int i; 
	for(i=0;i<t;i++) 
	{  
		TMOD=0x01; 						//��ʱ��0��������ģʽ1    
		TF0=0; 							//�������־λ��������
		TH0=0xfc; 
		TL0=0x66;  
		TR0=1; 							//������ʱ��0    
		while(TF0==0){;} 				//��ʱ���������ѭ���ȴ�    
		TR0=0; 							//�رն�ʱ��0    
	} 
}
void delay_10ms(unsigned int t) 		//��ʱt*10ms
{  
	unsigned int i; 
	for(i=0;i<t;i++) 
	{  
		TMOD=0x01; 						//��ʱ��0��������ģʽ1    
		TF0=0; 							//�������־λ��������    
		TH0=0xdc; 
		TL0=0x0;  
		TR0=1; 							//������ʱ��0    
		while(TF0==0){;} 				//��ʱ���������ѭ���ȴ�    
		TR0=0; 							//�رն�ʱ��0    
	} 
}
void delay_50ms(unsigned int t) 	    //��ʱt*50ms
{  
	unsigned int i; 
	for(i=0;i<t;i++) 
	{  
		TMOD=0x01; 						//��ʱ��0��������ģʽ1    
		TF0=0; 							//�������־λ��������    
		TH0=0x4c;
		TL0=0x0;  
		TR0=1; 							//������ʱ��0    
		while(TF0==0){;} 				//��ʱ���������ѭ���ȴ�    
		TR0=0; 							//�رն�ʱ��0    
	} 
}
