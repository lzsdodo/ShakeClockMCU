#include <reg51.h>
#include <intrins.h>
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library 
#include <sys\sys.h>
#include <lcd\lcd.h>
#include <font\font.h>

#define uchar unsigned char
#define uint unsigned int 
typedef unsigned char  BYTE;
typedef unsigned short WORD;

//DS1302��ص�ַ����
#define Ds1302_sec_addr			0x80	//0x81		//�����ݵ�ַ	bit1=1ʱ���͹��ģ�ʱ������ֹͣ������bit1=0ʱ������������
#define Ds1302_min_addr			0x82	//0x83		//�����ݵ�ַ
#define Ds1302_hr_addr			0x84	//0x85		//ʱ���ݵ�ַ	bit7=1ʱ��12Сʱģʽ��bit7=0ʱ��24Сʱģʽ��
#define Ds1302_date_addr		0x86	//0x87		//�����ݵ�ַ
#define Ds1302_month_addr		0x88	//0x89		//�����ݵ�ַ
#define Ds1302_day_addr			0x8a	//0x8b		//�������ݵ�ַ
#define Ds1302_year_addr		0x8c	//0x8d		//�����ݵ�ַ
#define Ds1302_control_addr		0x8e	//0x8f		//�������ݵ�ַ		0x80ʱ�޷�д��DS1302��0x00ʱ��д�룻(bit7=1/0,����ȫΪ0��)
#define Ds1302_charger_addr		0x90	//0x91		//������ģʽ 	 
#define Ds1302_clkburst_addr	0xbe	//0xbf		//ʱ��ͻ��ģʽ�Ĵ���
#define Ds1302_ramburst_addr	0xfe	//0xff		//RAMͻ��ģʽ�Ĵ���

//ADXL345��ص�ַ����
#define SlaveAddress   0xA6   //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A

#define Status_num 100


sbit Ds1302_SCK = P3^0;	//ʱ��	
sbit Ds1302_SDA = P3^1;	//����	
sbit Ds1302_RST = P3^2; //DS1302��λ

/*
//������DS1302�ܽŶ���
sbit Ds1302_SCK = P3^6;	//ʱ��	
sbit Ds1302_SDA = P3^4;	//����	
sbit Ds1302_RST = P3^5; //DS1302��λ
*/
//ADXL345���ٶ�оƬ�ܽŶ���
sbit   SCL=P1^1;      //IICʱ�����Ŷ���
sbit   SDA=P1^0;      //IIC�������Ŷ���

//�����־
bit ReadRTC_Flag;									//��DS1302��־
bit reset;											//��ʾ��ʼ����־
bit mod;											//ģʽ��־
bit setmod;											//����ģʽ 	0��ʱ��������ʾ��1��ʱ���������ģʽ
bit settime;										//�л�����ʱ��״̬��־
bit setbell;										//�л���������״̬��־
bit ring;											//�л�����ģʽ״̬��־
bit rering;											//�����ٴοɴ�����־

//DS1302������鶨��
static unsigned char Ds1302_time_BCD[8]={40,34,9,18,9,4,13,0};				//���ʱ��������13-08-19 09:33:00		 BCD��
static unsigned char Ds1302_time[8];										//���ʱ��������  ds1302�ڲ�
static unsigned char Ds1302_display_time[8];								//ʱ����ʾ��������
//ADXL345������鶨��
static unsigned char BUF[8];                         						//�������ݻ����� 
static int  dis_data;                       								//���� 
static int x_value,y_value,z_value;
static uchar w_status,n_status,e_status,s_status;

//�������鶨��
static unsigned char Ds1302_Bell_time_BCD[3]={0,30,6};						//����ʱ�䣺��-ʱ						 BCD��
static unsigned char Ds1302_Bell_time[3];									//����ʱ��,16����
static unsigned char Ds1302_Bell_display_time[2];							//����ʱ����ʾ��������
//ҡһҡ��ض���
static unsigned int  shake_num_value,shake_num,shake_display_num;			//ҡҡ�����Լ�����ʾ����
static unsigned char ring_time_BCD,ring_display_time;						//����ʱ���Լ�����ʾ����
static unsigned char ring_stop_time[2],ring_stop_time_BCD[2];
//��־λ���
static unsigned char setring;												//�������ò���
static unsigned char modnum,modnum_cmp;


/******************************************************************/
/*                    ��������                                    */
/******************************************************************/
//DS1302��������
void Ds1302_Write_Byte(unsigned char temp);
void Ds1302_Write( unsigned char address,unsigned char dat );
unsigned char Ds1302_Read ( unsigned char address );
void Ds1302_Read_RTC(void);
void Ds1302_Set_RTC(void);
//ADXL345��������
void delay(unsigned int k);
void Init_ADXL345(void);             //��ʼ��ADXL345
void WriteDataLCM(uchar dataW);
void WriteCommandLCM(uchar CMD,uchar Attribc);
//void DisplayOneChar(uchar X,uchar Y,uchar DData);
void conversion(uint temp_data);
void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   //����д������
uchar Single_Read_ADXL345(uchar REG_Address);                   //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_ADXL345();                                  //�����Ķ�ȡ�ڲ��Ĵ�������
 
void Delay5us(void);
void Delay5ms(void);
void ADXL345_Start(void);
void ADXL345_Stop(void);
void ADXL345_SendACK(bit ack);
bit  ADXL345_RecvACK(void);
void ADXL345_SendByte(BYTE dat);
BYTE ADXL345_RecvByte(void);
void ADXL345_ReadPage(void);
void ADXL345_WritePage(void);

void ADXL345_AXYZ(void);

//������ʾ��������
void judge_mode(void);
void w_mod0(void);
void n_mod1(void);
void e_mod2(void);
void s_mod3(void);
//void a_mod4(void);
//����ת����������
void bell_changeinBCD(void);
void ring_changeinBCD(void);
//��ʼ����������
void Int0_Init(void);
void main_Init(void);

/******************************************************************/
/*                    �жϺ���                                    */
/******************************************************************/
void INT0_SVC(void) interrupt 0                           
{
	setmod=~setmod;
	delayms(50);
//	if(Key==0x07)	
//	{		
//		settime=~settime;
//		if(settime) {reset=0;setbell=0;setring=0;k=0;}
//		if(!settime) Flash(8);
//	}		//��ʱ���趨�� ;����0-1ת����setmod=1ʱ������ģʽΪ0��k=0��
//	if(Key==0x06)	
//	{
//		setbell=~setbell;		
//		if(setbell) {reset=0;settime=0;setring=0;k=0;}
//		if(!setbell) Flash(8);
//	}	//�������趨�� ;����0-1ת����setbell=1ʱ��ʱ��ģʽΪ0��k=0;����������ʾ����cpr_bellΪ0��
//	if(Key==0x05)	{if((settime)||(setbell)||(setring!=0)) num=1;}
//	if(Key==0x04)	{if((settime)||(setbell)||(setring!=0)) num=2;}
//	if(Key==0x03)	{if((settime)||(setbell)||(setring!=0)) num=3;}
//	if(Key==0x02)	{if((settime)||(setbell)||(setring!=0)) num=4;}
//	if(Key==0x01)	
//	{		
//		setring++;				
//		if(setring!=0)	{reset=0;settime=0;setbell=0;k=0;}		
//		if(setring>2) 
//		{
//			setring=0;		
//			Flash(8);
//		}
//		if(setring==0)	Flash(8);
//	}				
//	if(Key==0x00)	mod=~mod;reset=0;   
}

void main()
{ 	               
	Int0_Init();
	main_Init();
	Ds1302_Set_RTC();
	bell_changeinBCD();
	ring_changeinBCD();	
	Lcd_Init();   //tft��ʼ��
	Init_ADXL345();                  //��ʼ��ADXL345
	
	while(1)
	{	
		ADXL345_AXYZ(); 
		judge_mode();
		while(ReadRTC_Flag)
		{
		    ReadRTC_Flag=0;
			Ds1302_Read_RTC();
			if(reset==0) LCD_Clear(BLACK);
			if(modnum==0) {w_mod0();}
			else if(modnum==1) {n_mod1();}
			else if(modnum==2) {e_mod2();}
			else if(modnum==3) {s_mod3();}		
		} 
		ReadRTC_Flag=1;	
    }

}

//��ʼ������**********************************************************************************************************
//������ʼ��
void main_Init(void)
{
	unsigned char i;
	//��Ļ������ʼ��
	LCD_Clear(BLACK); 			//����
	BACK_COLOR=BLACK;			//����ɫ����
	POINT_COLOR=WHITE; 			//����ɫ: ��
	//ģʽ�Լ���ز�����ʼ��
	w_status=0;
	n_status=0;
	e_status=0;
	s_status=0;
	modnum=0;
	modnum_cmp=modnum;
	setmod=0;
	
	mod=0;						  //ģʽ��ʼ�� mod����ʾʱ����/�����հ���7��;
	settime=0;
	setbell=0;
	setring=0;	
	ring=0;
	rering=0;
	reset=0;
	//ҡһҡ�����ʼ��ֵ�趨(���ø�Ϊ��ȡEEPROM����)
	shake_num_value=10;
	shake_num=shake_num_value;
	shake_display_num=shake_num;
	ring_time_BCD=10;
	ring_display_time=ring_time_BCD;
	
	for(i=0;i<8;i++)
	Ds1302_display_time[i]=Ds1302_time[i];	
}
//�жϳ�ʼ��
void Int0_Init(void)
{
	EA = 0;
    IT0 = 1; //�����ش����ж�
    EX0 = 1; //�����ⲿ�ж�
    EA = 1;
}
//��ʼ������**********************************************************************************************************

//��ʾ������**********************************************************************************************************
//��ʾģʽ0��ʱ:��:��
void w_mod0(void)
{
	//���ֲ���
	if(reset==0)
	{
		showhanzi(80,40,0);
		showhanzi(80,88,1);
	}
	//���ֲ���
	if((reset==0)||(Ds1302_time[2]!=Ds1302_display_time[2]))
	{
		Ds1302_display_time[2]=Ds1302_time[2];
		w_LCD_ShowNum( 32, 16, (Ds1302_display_time[2]/16));											//	   
		w_LCD_ShowNum( 32, 32, (Ds1302_display_time[2]&0x0f));								  			//
	}	
	if(reset==0) w_LCD_ShowNum( 32, 48, 10);
	if((reset==0)||(Ds1302_time[1]!=Ds1302_display_time[1]))
	{
		Ds1302_display_time[1]=Ds1302_time[1];					    	
		w_LCD_ShowNum( 32, 64, (Ds1302_display_time[1]/16));								  			//
		w_LCD_ShowNum( 32, 80, (Ds1302_display_time[1]&0x0f));								  			//
	}
	if(reset==0) w_LCD_ShowNum( 32, 96, 10);
	if((reset==0)||(Ds1302_time[0]!=Ds1302_display_time[0]))
	{
		Ds1302_display_time[0]=Ds1302_time[0];
		w_LCD_ShowNum( 32, 112, (Ds1302_display_time[0]/16));									   			//
		w_LCD_ShowNum( 32, 128, (Ds1302_display_time[0]&0x0f));									   			//
	}
	if(reset==0)
	{
		showxuexiao(0,80,0);
		showxuexiao(0,96,1);
		showxuexiao(0,112,0);
		showxuexiao(0,128,2);
		showxuexiao(0,144,3);
	}
	if(reset==0)	reset=1;	
}
//��ʾģʽ2����-��-��
void e_mod2(void)
{
	//���ֲ���
	if(reset==0)
	{
		showhanzi(16,88,2);
		showhanzi(16,40,3);
	}
	//���ֲ���
	if((reset==0)||(Ds1302_time[6]!=Ds1302_display_time[6]))
	{
		Ds1302_display_time[6]=Ds1302_time[6];
		e_LCD_ShowNum( 64, 128, (Ds1302_display_time[6]/16));								 //
		e_LCD_ShowNum( 64, 112, (Ds1302_display_time[6]&0x0f));								 //
	}
	if(reset==0) e_LCD_ShowNum( 64, 96, 10);	
	if((reset==0)||(Ds1302_time[4]!=Ds1302_display_time[4]))
	{
		Ds1302_display_time[4]=Ds1302_time[4];				    	
		e_LCD_ShowNum( 64, 80, (Ds1302_display_time[4]/16));								 //
		e_LCD_ShowNum( 64, 64, (Ds1302_display_time[4]&0x0f));								 //
	}
	if(reset==0) e_LCD_ShowNum( 64, 48, 10);
	if((reset==0)||(Ds1302_time[3]!=Ds1302_display_time[3]))
	{
		Ds1302_display_time[3]=Ds1302_time[3];
		e_LCD_ShowNum( 64, 32, (Ds1302_display_time[3]/16));								 //
		e_LCD_ShowNum( 64, 16, (Ds1302_display_time[3]&0x0f));								 //
	}
	if(reset==0)
	{
		showxuexiao(112,64,4);
		showxuexiao(112,48,5);
		showxuexiao(112,32,4);
		showxuexiao(112,16,6);
		showxuexiao(112,0,7);
	}
	if(reset==0)	reset=1;		
}

//��ʾģʽ1����ʾ����ʱ��: ʱ:��
void n_mod1(void)		  
{
	//���ֲ���
	if(reset==0)
	{
		showhanzi(72,112,4);
		showhanzi(24,112,5);
	}
	//���ֲ���
	if((reset==0)||(Ds1302_Bell_time[2]!=Ds1302_Bell_display_time[1]))
	{
		Ds1302_Bell_display_time[1]=Ds1302_Bell_time[2];
		n_LCD_ShowNum( 88, 48, (Ds1302_Bell_display_time[1]/16));									//
		n_LCD_ShowNum( 72, 48, (Ds1302_Bell_display_time[1]&0x0f));									//
	}
	if(reset==0) n_LCD_ShowNum( 56, 48, 10);
	if((reset==0)||(Ds1302_Bell_time[1]!=Ds1302_Bell_display_time[0]))
	{
		Ds1302_Bell_display_time[0]=Ds1302_Bell_time[1];
		n_LCD_ShowNum( 40, 48, (Ds1302_Bell_display_time[0]/16));									//
		n_LCD_ShowNum( 24, 48, (Ds1302_Bell_display_time[0]&0x0f));									//
	}
	if(reset==0)
	{
		showxuexiao(64,0,12);
		showxuexiao(48,0,13);
		showxuexiao(32,0,12);
		showxuexiao(16,0,14);
		showxuexiao(0,0,15);
	}
	if(reset==0) reset=1;
}
//������ʱ��ʾ:��������ʱ���Լ�ҡһҡ�Ĵ���
void s_mod3(void)
{
	//���ֲ���
	if(reset==0)
	{
		showhanzi(16,16,6);
		showhanzi(48,16,7);
		showhanzi(80,16,6);	
	}
	//���ֲ���
	if((reset==0)||(ring_time_BCD!=ring_display_time))
	{
		ring_display_time=ring_time_BCD;
		s_LCD_ShowNum( 24, 56, (ring_display_time/10) );									//
		s_LCD_ShowNum( 40, 56, (ring_display_time%10) );									//
	}
	if(reset==0)
	{
		s_LCD_ShowNum( 56, 56, 10 );
		s_LCD_ShowNum( 72, 56, 11 );
		s_LCD_ShowNum( 88, 56, 12 );	
	}
	if((reset==0)||(shake_num_value!=shake_display_num))
	{
		shake_display_num=shake_num_value;
		s_LCD_ShowNum( 24, 96, (shake_display_num/100));									//
		s_LCD_ShowNum( 40, 96, ((shake_display_num/10)%10));									//
		s_LCD_ShowNum( 56, 96, (shake_display_num%10));									//
	}
	if(reset==0)
	{
		showhanzi(72,96,8);
		showxuexiao(48,144,8);
		showxuexiao(64,144,9);
		showxuexiao(80,144,8);
		showxuexiao(96,144,10);
		showxuexiao(112,144,11);
	}
	if(reset==0)	reset=1;
}
/*���ٶ���ʾ
void a_mod4(void)
{
	if(x_value<0)	w_LCD_ShowNum(96,16,11);
	else w_LCD_ShowNum(96,16,12);											   
	w_LCD_ShowNum( 96, 32, (x_value/100));								  		
	w_LCD_ShowNum( 96, 48, ((x_value%100)/10));
	w_LCD_ShowNum( 96, 64, ((x_value%10)));
					    	
	if(y_value<0)	w_LCD_ShowNum(64,16,11);
	else w_LCD_ShowNum(64,16,12);											   
	w_LCD_ShowNum( 64, 32, (y_value/100));								  			
	w_LCD_ShowNum( 64, 48, ((y_value%100)/10));
	w_LCD_ShowNum( 64, 64, ((y_value%10)));								  			

	if(z_value<0)	w_LCD_ShowNum(32,16,11);
	else w_LCD_ShowNum(32,16,12);										
	w_LCD_ShowNum( 32, 32, (z_value/100));								  		
	w_LCD_ShowNum( 32, 48, ((z_value%100)/10));
	w_LCD_ShowNum( 32, 64, ((z_value%10)));									   		

	if(reset==0)
	{
		showxuexiao(0,80,0);
		showxuexiao(0,96,1);
		showxuexiao(0,112,0);
		showxuexiao(0,128,2);
		showxuexiao(0,144,3);
	}
	if(reset==0)	reset=1;		
}*/
//��ʾ������************************************************************************************************************

//����ת������**********************************************************************************************************
//����ʱ��BCDת��
void bell_changeinBCD(void)
{
	unsigned char i,tmp;
	for(i=0;i<3;i++)		//BCD����
	{       
		tmp=Ds1302_Bell_time_BCD[i]/10;
		Ds1302_Bell_time[i]=Ds1302_Bell_time_BCD[i]%10;
		Ds1302_Bell_time[i]=Ds1302_Bell_time[i]+tmp*16;
	} 
}
//����ֹͣʱ��BCDת��
void ring_changeinBCD(void)									
{
	unsigned char i,tmp;
	ring_stop_time_BCD[1]=Ds1302_Bell_time_BCD[2];
	ring_stop_time_BCD[0]=Ds1302_Bell_time_BCD[1]+ring_time_BCD;
	if(ring_stop_time_BCD[0]>59)	{ring_stop_time_BCD[0]-=60;ring_stop_time_BCD[1]++;}
	if(ring_stop_time_BCD[1]>23)	ring_stop_time_BCD[1]=0;
	for(i=0;i<8;i++)_nop_();
	for(i=0;i<2;i++)		//BCD����
	{       
		tmp=ring_stop_time_BCD[i]/10;
		ring_stop_time[i]=ring_stop_time_BCD[i]%10;				
		ring_stop_time[i]=ring_stop_time[i]+tmp*16;
	}
	for(i=0;i<8;i++)_nop_(); 
}
//����ת������**********************************************************************************************************

//Ds1302������**********************************************************************************************************
//дһ���ֽ�
void Ds1302_Write_Byte(unsigned char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	//ѭ��8�� д������
	{ 
		Ds1302_SCK=0;
		Ds1302_SDA=temp&0x01;     //ÿ�δ�����ֽ� 
		temp>>=1;  		//����һλ
		Ds1302_SCK=1;
	}
}  
//д��DS1302
void Ds1302_Write( unsigned char address,unsigned char dat )
{
	Ds1302_RST=0;
	_nop_();
 	Ds1302_SCK=0;
	_nop_();
 	Ds1302_RST=1;	
   	_nop_();                    //����
 	Ds1302_Write_Byte(address);	//���͵�ַ
 	Ds1302_Write_Byte(dat);		//��������
 	Ds1302_RST=0;  		            //�ָ�
}
//����DS1302����
unsigned char Ds1302_Read ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	Ds1302_RST=0;
	_nop_();
	_nop_();
 	Ds1302_SCK=0;
	_nop_();
	_nop_();
 	Ds1302_RST=1;
	_nop_();
	_nop_();
 	Ds1302_Write_Byte(address);
 	for (i=0;i<8;i++) 		//ѭ��8�� ��ȡ����
 	{		
 		if(Ds1302_SDA)
 		temp|=0x80;			//ÿ�δ�����ֽ�
		Ds1302_SCK=0;
		temp>>=1;			//����һλ
		_nop_();
		_nop_();
		_nop_();
		Ds1302_SCK=1;
	} 
 	Ds1302_RST=0;
	_nop_();	          	//����ΪDS1302��λ���ȶ�ʱ��
 	_nop_();
	Ds1302_RST=0;
	Ds1302_SCK=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	Ds1302_SCK=1;
	_nop_();
	_nop_();
	Ds1302_SDA=0;
	_nop_();
	_nop_();
	Ds1302_SDA=1;
	_nop_();
	_nop_();
	return (temp);			//����
}
//read RTC ��ʱ������
void Ds1302_Read_RTC(void)
{
	unsigned char i,tmp;
	Ds1302_time[0]=Ds1302_Read(Ds1302_sec_addr|0x01);
	Ds1302_time[1]=Ds1302_Read(Ds1302_min_addr|0x01);
	Ds1302_time[2]=Ds1302_Read(Ds1302_hr_addr|0x01);
	Ds1302_time[3]=Ds1302_Read(Ds1302_date_addr|0x01);
	Ds1302_time[4]=Ds1302_Read(Ds1302_month_addr|0x01);
	Ds1302_time[5]=Ds1302_Read(Ds1302_day_addr|0x01);
	Ds1302_time[6]=Ds1302_Read(Ds1302_year_addr|0x01);
	for(i=0;i<8;i++)
	   {           //BCD����
		tmp=Ds1302_time[i]/16;
		Ds1302_time_BCD[i]=Ds1302_time[i]%16;
		Ds1302_time_BCD[i]=Ds1302_time_BCD[i]+tmp*10;
	   }
}
//set RTC	�趨ʱ������ 
void Ds1302_Set_RTC(void)
{
	unsigned char i,tmp;                
	for(i=0;i<8;i++)
	{                  //BCD����
		tmp=Ds1302_time_BCD[i]/10;
		Ds1302_time[i]=Ds1302_time_BCD[i]%10;
		Ds1302_time[i]=Ds1302_time[i]+tmp*16;
	} 
 	Ds1302_Write(0x8E,0X00);	
	Ds1302_Write(Ds1302_sec_addr,Ds1302_time[0]);
	Ds1302_Write(Ds1302_min_addr,Ds1302_time[1]);
	Ds1302_Write(Ds1302_hr_addr,Ds1302_time[2]);
	Ds1302_Write(Ds1302_date_addr,Ds1302_time[3]);
	Ds1302_Write(Ds1302_month_addr,Ds1302_time[4]);
	Ds1302_Write(Ds1302_day_addr,Ds1302_time[5]);
	Ds1302_Write(Ds1302_year_addr,Ds1302_time[6]);
	Ds1302_Write(0x8E,0x80);
} 
//Ds1302������**********************************************************************************************************

//ADXL345������**********************************************************************************************************
/**************��ʱ*****************/
void delay(unsigned int k) 
{ 
    unsigned int i,j; 
    for(i=0;i<k;i++)
    { 
    for(j=0;j<121;j++) 
    {;}} 
}
/**************************************
��ʱ5΢��(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
}
/**************************************
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5ms()
{
    WORD n = 560;
    while (n--);
}
/**************************************
��ʼ�ź�
**************************************/
void ADXL345_Start()
{
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
}
/**************************************
ֹͣ�ź�
**************************************/
void ADXL345_Stop()
{
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}
/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void ADXL345_SendACK(bit ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}
/**************************************
����Ӧ���ź�
**************************************/
bit ADXL345_RecvACK()
{
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
    return CY;
}
/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void ADXL345_SendByte(BYTE dat)
{
    BYTE i;
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    ADXL345_RecvACK();
}
/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE ADXL345_RecvByte()
{
    BYTE i;
    BYTE dat = 0;
    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= SDA;             //������               
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}
//******���ֽ�д��*******************************************
 
void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    ADXL345_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    ADXL345_Stop();                   //����ֹͣ�ź�
}
//********���ֽڶ�ȡ*****************************************
uchar Single_Read_ADXL345(uchar REG_Address)
{  
uchar REG_data;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ 
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=ADXL345_RecvByte();              //�����Ĵ�������
    ADXL345_SendACK(1);   
    ADXL345_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
//*********************************************************
//��������ADXL345�ڲ����ٶ����ݣ���ַ��Χ0x32~0x37
//*********************************************************
void Multiple_read_ADXL345(void)
{   
	uchar i;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(0x32);                   //���ʹ洢��Ԫ��ַ����0x32��ʼ 
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = ADXL345_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
           ADXL345_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //��ӦACK
        }
    }
    ADXL345_Stop();                          //ֹͣ�ź�
    Delay5ms();
}
//*****************************************************************
//��ʼ��ADXL345��������Ҫ��ο�pdf�����޸�************************
void Init_ADXL345()
{
	TMOD|=0x20;
	SCON|=0x40;
	TH1=0xfd;
	TL1=0xfd;
	PCON|=0x00;
	TI=0;
	RI=0;
	TR1=1; 
    delay(500);			   //�ϵ���ʱ
	//������α��ڳ�ʼ������ǰ��
    Single_Write_ADXL345(0x31,0x0B);   //������Χ,����16g��13λģʽ
    Single_Write_ADXL345(0x2C,0x08);   //�����趨Ϊ12.5 �ο�pdf13ҳ
    Single_Write_ADXL345(0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
    Single_Write_ADXL345(0x2E,0x80);   //ʹ�� DATA_READY �ж�
    Single_Write_ADXL345(0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    Single_Write_ADXL345(0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    Single_Write_ADXL345(0x20,0x05);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
}
void delay_ser(unsigned char x)
{
	unsigned char i,m;
	for(i=255;i>0;i--)
		for(m=x;m>0;m--);
}
void ADXL345_AXYZ(void)
{
	Multiple_Read_ADXL345();        //�����������ݣ��洢��BUF��
	x_value=(BUF[1]*256)|(BUF[0]);
	y_value=(BUF[3]*256)|(BUF[2]);
	z_value=(BUF[5]*256)|(BUF[4]);
	if(BUF[1]>>7)
	{ 		
		x_value=(0-(0x8000-x_value&0x7fff));	
	}
	if(BUF[3]>>7)
	{ 	
		y_value=(0-(0x8000-y_value&0x7fff));
	}
	if(BUF[5]>>7)
	{ 	
		z_value=(0-(0x8000-z_value&0x7fff));
	}	

}
//ADXL345������**********************************************************************************************************

void judge_mode(void)
{
	if(y_value<(-240)) 
	{
		w_status++;
		n_status=0;
		e_status=0;
		s_status=0;
		if(w_status>=Status_num) {w_status=Status_num;modnum=0;}  
		goto status;
	} 
	if(y_value>240)
	{
		w_status=0;
		n_status=0;
		e_status++;
		s_status=0;
		if(e_status>=Status_num) {e_status=Status_num;modnum=2;} 
		goto status;
	}  
	if(x_value>240) 
	{
		w_status=0;
		n_status++;
		e_status=0;
		s_status=0;
		if(n_status>=Status_num) {n_status=Status_num;modnum=1;}  
		goto status;
	}
	if(x_value<(-240)) 
	{
		w_status=0;
		n_status=0;
		e_status=0;
		s_status++;
		if(s_status>=Status_num) {s_status=Status_num;modnum=3;} 
		goto status;
	}
status:
	if(modnum!=modnum_cmp) 
	{
		modnum_cmp=modnum;
		reset=0;
	} 
}
