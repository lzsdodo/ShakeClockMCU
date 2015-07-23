#include <reg52.h>
#include <intrins.h>
#include <math.h>    
#include <stdio.h>   
#include <lcd\lcd.h>
#include <font\font.h>
#include <I2C\I2C.h>
#include <DELAY\DELAY.h>

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
#define SlaveAddress   0xA6   //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�	\
 //ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A
                      

sbit Ds1302_SCK = P2^5;	//ʱ��	
sbit Ds1302_SDA = P2^6;	//����	
sbit Ds1302_RST = P2^7; //DS1302��λ

/*
//������DS1302�ܽŶ���
sbit Ds1302_SCK = P3^6;	//ʱ��	
sbit Ds1302_SDA = P3^4;	//����	
sbit Ds1302_RST = P3^5; //DS1302��λ
*/
//ADXL345���ٶ�оƬ�ܽŶ���
sbit   SCL=P1^1;      //IICʱ�����Ŷ���
sbit   SDA=P1^0;      //IIC�������Ŷ���

sbit BEEP = P3^5;	
  
//sbit CHECK = P0^0;
//sbit CHECK_while1_howlong = P0^1;
//sbit CHECK_ring_shake = P0^2;


//�����־
bit ReadRTC_Flag;									//��DS1302��־
bit reset;											//��ʾ��ʼ����־
bit setmod;											//����ģʽ 	0��ʱ��������ʾ��1��ʱ���������ģʽ
bit clear;
bit ring;											//�л�����ģʽ״̬��־


//DS1302������鶨��
static unsigned char Ds1302_time_BCD[8];//={40,56,11,22,9,4,13,0};				//���ʱ��������13-08-19 09:33:00		 BCD��
static unsigned char Ds1302_time[8];										//���ʱ��������  ds1302�ڲ�
static unsigned char Ds1302_display_time[8];								//ʱ����ʾ��������
//ADXL345������鶨��
static unsigned char BUF[8];                         						//�������ݻ����� 
static int  dis_data;                       								//���� 
static int x_value,y_value,z_value;
static uchar w_static_status,n_static_status,e_static_status,s_static_status;

//EEPROM������鶨��
static unsigned char e2prom_add;

//�������鶨��
static unsigned char Ds1302_Bell_time_BCD[3]={0,1,12};						//����ʱ�䣺��-ʱ						 BCD��
static unsigned char Ds1302_Bell_time[3];									//����ʱ��,16����
static unsigned char Ds1302_Bell_display_time[2];							//����ʱ����ʾ��������
static unsigned char Ds1302_time_tmp;
//ҡһҡ��ض���
static unsigned int  shake_num_value,shake_num,shake_display_num;			//ҡҡ�����Լ�����ʾ����
static unsigned char ring_time_BCD,ring_time_tmp,ring_display_time;			//����ʱ���Լ�����ʾ����
static unsigned char ring_stop_time[2],ring_stop_time_BCD[2];

static unsigned char Static_status_num=10;
static unsigned char shake_flag=0;
static unsigned char xx_now,xx_before,yy_now,yy_before;

//��־λ���
static char k,k_before;
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

//�жϺ�������
void set_parameter(void);
void judge_mode(void);
void judge_degree(void);
void judge_shake(void);
void judge_hlir(void);

//������ʾ��������
void display(void);
void w_mod0(void);
void n_mod1(void);
void e_mod2(void);
void s_mod3(void);
void position(void);
void clear_position(void);


//���ú�������
void set_time(unsigned char shake_flag);
void set_ring(unsigned char shake_flag);
void set_date(unsigned char shake_flag);
void set_shake(unsigned char shake_flag);

//����ת����������
void bell_changeinBCD(void);
void ring_changeinBCD(void);

//��ʼ����������
void Int0_Init(void);
void main_Init(void);
void E2P_readdata(void);
void Data_Init(void);

/******************************************************************/
/*                    �жϺ���                                    */
/******************************************************************/
void INT0_SVC(void) interrupt 0                           
{
	setmod=~setmod;
	if(!setmod) {clear=1;}
	if(setmod) {k=0;k_before=8;clear=0;}	
	delay_10ms(3);
}

void main()
{ 	
//��ʼ������   
	main_Init();            		
	EEPROM_Init();					//��ʼ��EEPROM
	Init_ADXL345();                 //��ʼ��ADXL345
	Int0_Init();					//��ʼ��INT0�ж�
	E2P_readdata();
	Data_Init();		
	Lcd_Init();   					//tft��ʼ��
//ѭ��������
	while(1)
	{	
		//CHECK_while1_howlong=~CHECK_while1_howlong;
		ADXL345_AXYZ(); 
		while(ReadRTC_Flag)
		{
		    ReadRTC_Flag=0;
			if(setmod==0) 
			{
				if(ring==0) judge_mode();	
				Ds1302_Read_RTC();
			}
			if((ring==0)&&(Ds1302_time[0]==Ds1302_Bell_time[0])&&(Ds1302_time[1]==Ds1302_Bell_time[1])&&(Ds1302_time[2]==Ds1302_Bell_time[2]))
			{
				ring=1;		
				BEEP=0;
				modnum=3;
				reset=0;
				EA = 0;
			}
			if(ring==1)
			{	
				judge_shake();
				judge_hlir();			
				if((shake_num==0)||((Ds1302_time[1]==ring_stop_time[0])&&(Ds1302_time[2]==ring_stop_time[1])))
				{
					ring=0;
					BEEP=1;
					shake_num=shake_num_value;										
					EA = 1;
				}
			}
			if(clear) {clear_position();clear=0;}					
			if(setmod) 
			{
				set_parameter();
				position();
				if(shake_flag!=0) shake_flag=0;
				delay_10ms(1);
			}	
			display();
		} 
		ReadRTC_Flag=1;	
    }

}

//��ʼ������**********************************************************************************************************
//������ʼ��
void main_Init(void)
{
	//��Ļ������ʼ��
	LCD_Clear(BLACK); 			//����
	BACK_COLOR=BLACK;			//����ɫ����
	POINT_COLOR=WHITE; 			//����ɫ: ��
	//ģʽ�Լ���ز�����ʼ��
	w_static_status=0;
	n_static_status=0;
	e_static_status=0;
	s_static_status=0;
	modnum=0;
	modnum_cmp=modnum;
	clear=0;
	BEEP=1;
	setmod=0;
	reset=0;
	ring=0;	
	delay_10ms(3);
}
void Data_Init(void)
{
	unsigned char i;
	Ds1302_Set_RTC();
	//ҡһҡ�����ʼ��ֵ�趨(���ø�Ϊ��ȡEEPROM����)
	shake_num=shake_num_value;
	shake_display_num=shake_num_value;
	ring_time_tmp=ring_time_BCD;
	ring_display_time=ring_time_BCD;
	Ds1302_Bell_time_BCD[0]=0;
	bell_changeinBCD();
	ring_changeinBCD();
	
	//ʱ�����ڳ�ʼ��
	for(i=0;i<8;i++)
	Ds1302_display_time[i]=Ds1302_time[i];
	delay_10ms(30);
}
//�жϳ�ʼ��
void Int0_Init(void)
{
	EA = 0;
    IT0 = 1; //�����ش����ж�
    EX0 = 1; //�����ⲿ�ж�
    EA = 1;
	delay_10ms(3);
}
//��ʼ������**********************************************************************************************************

//���ú���************************************************************************************************************
void set_parameter(void)
{
	judge_degree();
	delay_1ms(1);
	if(shake_flag!=0)
		{
		if(modnum==0) 
		{
			set_time(shake_flag);
			Ds1302_Set_RTC();
			Ds1302_Read_RTC();
		}
		if(modnum==1) 
		{
			set_ring(shake_flag);
			bell_changeinBCD();
			ring_changeinBCD();
		}
		if(modnum==2) 
		{
			set_date(shake_flag);
			Ds1302_Set_RTC();
			Ds1302_Read_RTC();
		}
		if(modnum==3) 
		{
			set_shake(shake_flag);
			//����set_shake_num(num); set_ring_time(num);
			shake_num=shake_num_value;
			ring_changeinBCD();
		}
	}
	delay_1ms(1);
}
void set_time(unsigned char shake_flag)
{
	switch(shake_flag)
    {
/*����*/case 1:
			{
				//k=0	ʱ[2]
				if(k==0)
				{	
					Ds1302_time_BCD[2]+=10;				
					if((Ds1302_time_BCD[2]>23)&&(Ds1302_time_BCD[2]<30))	Ds1302_time_BCD[2]=23;
					if(Ds1302_time_BCD[2]>=30)	Ds1302_time_BCD[2]=(Ds1302_time_BCD[2]%10);
				}
				//k=1	ʱ[2]
				if(k==1)
				{
					Ds1302_time_BCD[2]++;
					if(Ds1302_time_BCD[2]>23)	Ds1302_time_BCD[2]=0;
				}																 
				//k=2	��[1]	
				if(k==2)	Ds1302_time_BCD[1]+=10;
				//k=3	��[1]				
				if(k==3)	Ds1302_time_BCD[1]++;					
				if(Ds1302_time_BCD[1]>=60)	Ds1302_time_BCD[1]=(Ds1302_time_BCD[1]%10);
				//k=4	��[0]
				if(k==4)	Ds1302_time_BCD[0]+=10;
				//k=5	��[0]
				if(k==5)	Ds1302_time_BCD[0]++;
				if(Ds1302_time_BCD[0]>=60) 	Ds1302_time_BCD[0]=(Ds1302_time_BCD[0]%10);
				break;
			}
/*����*/case 2:	
			{
				//k=0	ʱ[2]
				if(k==0)
				{									
					if(Ds1302_time_BCD[2]<10)
					{	
						if(Ds1302_time_BCD[2]>=4)	Ds1302_time_BCD[2]=23;
						if(Ds1302_time_BCD[2]<4)	Ds1302_time_BCD[2]+=20;
					}
					else	if(Ds1302_time_BCD[2]>=10)	Ds1302_time_BCD[2]-=10;			
				}
				//k=1	ʱ[2]				
				if(k==1)
				{
					if(Ds1302_time_BCD[2]==0)	Ds1302_time_BCD[2]=23;
					else	if(Ds1302_time_BCD[2]>0)	Ds1302_time_BCD[2]--;					 
				}
				//k=2	��[1]
				if(k==2)
				{
					if(Ds1302_time_BCD[1]>=10)	Ds1302_time_BCD[1]-=10;
					else	if(Ds1302_time_BCD[1]<10)	Ds1302_time_BCD[1]+=50; 
				}
				//k=3	��[1]
				if(k==3)
				{
					if(Ds1302_time_BCD[1]==0)	Ds1302_time_BCD[1]=59;
					else	if(Ds1302_time_BCD[1]>0)	Ds1302_time_BCD[1]--; 
				}
				//k=4	��[0]
				if(k==4)
				{
					if(Ds1302_time_BCD[0]>=10)	Ds1302_time_BCD[0]-=10;
					else	if(Ds1302_time_BCD[0]<10)	Ds1302_time_BCD[0]+=50; 
				}
				//k=5	��[0]
				if(k==5)
				{	
					if(Ds1302_time_BCD[0]==0)	Ds1302_time_BCD[0]=59;
					else	if(Ds1302_time_BCD[0]>0)	Ds1302_time_BCD[0]--;
				}
				break;
			}
/*����*/case 3:{k_before=k;k--;if(k<0) k=5;break;}
/*����*/case 4:{k_before=k;k++;if(k>5) k=0;break;}
		default:break;
	}
	write_add((0),Ds1302_time_BCD[0]);
	delay_1ms(5);
	write_add((1),Ds1302_time_BCD[1]);
	delay_1ms(5);
	write_add((2),Ds1302_time_BCD[2]);
	delay_1ms(5);	
}

void set_date(unsigned char shake_flag)
{
	switch(shake_flag)
    {
/*����*/case 1:
			{
				//k=0	��[6]
				if(k==0)	Ds1302_time_BCD[6]+=10;
				//k=1	��[6]
				if(k==1)	Ds1302_time_BCD[6]++;
				//k=2	��[4]
				if(k==2)	Ds1302_time_BCD[4]+=10;								
				if((k==2)&&(Ds1302_time_BCD[4]>12)&&(Ds1302_time_BCD[4]<=20))	Ds1302_time_BCD[4]=12;
				if((k==2)&&(Ds1302_time_BCD[4]>20))	Ds1302_time_BCD[4]=(Ds1302_time_BCD[4]%10);
				//k=3	��[4]
				if(k==3)	Ds1302_time_BCD[4]++;
				if((k==3)&&(Ds1302_time_BCD[4]>12))	Ds1302_time_BCD[4]=1;
				//k=4	��[3]
				if(k==4)	Ds1302_time_BCD[3]+=10;							
				if((k==4)&&(Ds1302_time_BCD[3]<40)&&(Ds1302_time_BCD[3]>31))	Ds1302_time_BCD[3]=31;
				if((k==4)&&(Ds1302_time_BCD[3]>=40))	Ds1302_time_BCD[3]=(Ds1302_time_BCD[3]%10);
				//k=5	��[3]
				if(k==5)	Ds1302_time_BCD[3]++;
				if((k==5)&&(Ds1302_time_BCD[3]>31))	Ds1302_time_BCD[3]=1;
				break;
			} 
/*����*/case 2:
			{
				if(k==0)
				{
				 	if(Ds1302_time_BCD[6]<10)	Ds1302_time_BCD[6]+=30;
					else	if(Ds1302_time_BCD[6]>=10)	Ds1302_time_BCD[6]-=10;	
				}  
				//k=0	��[6] 				
				if(k==1)
				{
					if(Ds1302_time_BCD[6]==0)	Ds1302_time_BCD[6]=30;	
					else	if(Ds1302_time_BCD[6]>0)	Ds1302_time_BCD[6]--;
				}
				//k=2	��[4] 
				if(k==2)
				{
					if(Ds1302_time_BCD[4]<10)
					{
						if(Ds1302_time_BCD[4]>2)	Ds1302_time_BCD[4]=12;
						if(Ds1302_time_BCD[4]<=2) 	Ds1302_time_BCD[4]+=10;
					}
					else	
					{
						if(Ds1302_time_BCD[4]==10)	Ds1302_time_BCD[4]=1;
						if(Ds1302_time_BCD[4]>10)	Ds1302_time_BCD[4]-=10;
					}
				}
				//k=3	��[4]
				if(k==3)
				{
					if(Ds1302_time_BCD[4]==1)	Ds1302_time_BCD[4]=12;
					else	if(Ds1302_time_BCD[4]>1)	Ds1302_time_BCD[4]--;
				}
				//k=4	��[3]
				if(k==4)
				{
					if(Ds1302_time_BCD[3]<10)
					{
						if(Ds1302_time_BCD[3]>1)	Ds1302_time_BCD[3]=31;
						if(Ds1302_time_BCD[3]<=1)	Ds1302_time_BCD[3]+=30;
					}
					else	
					{
						if(Ds1302_time_BCD[3]==10) Ds1302_time_BCD[3]=1;
						if(Ds1302_time_BCD[3]>10) Ds1302_time_BCD[3]-=10;			
					}	
				}
				//k=5	��[3]
				if(k==5)
				{
					if(Ds1302_time_BCD[3]==1) Ds1302_time_BCD[3]=31;
					else	if(Ds1302_time_BCD[3]>1)	Ds1302_time_BCD[3]--;
				}
				break;
			}
/*����*/case 3:{k_before=k;k--;if(k<0) k=5;break;}
/*����*/case 4:{k_before=k;k++;if(k>5) k=0;break;}
		default:break;		
	}
	write_add((3),Ds1302_time_BCD[3]);
	delay_1ms(5);
	write_add((4),Ds1302_time_BCD[4]);
	delay_1ms(5);
	write_add((6),Ds1302_time_BCD[6]);
	delay_1ms(5);

}
void set_ring(unsigned char shake_flag)
{
	//��������ʱ��
	switch(shake_flag)
	{

/*����*/case 1:
			{
				//k=0	ʱ[2]
				if(k==0)
				{	
					Ds1302_Bell_time_BCD[2]+=10;				
					if((Ds1302_Bell_time_BCD[2]>23)&&(Ds1302_Bell_time_BCD[2]<30))	Ds1302_Bell_time_BCD[2]=23;
					if(Ds1302_Bell_time_BCD[2]>=30)	Ds1302_Bell_time_BCD[2]=(Ds1302_Bell_time_BCD[2]%10);
				}
				//k=1	ʱ[2]
				if(k==1)
				{
					Ds1302_Bell_time_BCD[2]++;
					if(Ds1302_Bell_time_BCD[2]>23)	Ds1302_Bell_time_BCD[2]=0;
				}																 
				//k=2	��[1]
				if(k==2)	Ds1302_Bell_time_BCD[1]+=10;
				//k=3	��[1]				
				if(k==3)	Ds1302_Bell_time_BCD[1]++;					
				if(Ds1302_Bell_time_BCD[1]>=60)	Ds1302_Bell_time_BCD[1]=(Ds1302_Bell_time_BCD[1]%10);	
				break;
			}
/*����*/case 2:
			{
				//k=0	ʱ[2]
				if(k==0)
				{									
					if(Ds1302_Bell_time_BCD[2]<10)
					{
						if(Ds1302_Bell_time_BCD[2]>=4)	Ds1302_Bell_time_BCD[2]=23;
						if(Ds1302_Bell_time_BCD[2]<4)	Ds1302_Bell_time_BCD[2]+=20;
					}
					else	if(Ds1302_Bell_time_BCD[2]>=10)	Ds1302_Bell_time_BCD[2]-=10;			
				}
				//k=1	ʱ[2]				
				if(k==1)
				{
					if(Ds1302_Bell_time_BCD[2]==0)	Ds1302_Bell_time_BCD[2]=23;
					else	if(Ds1302_Bell_time_BCD[2]>0)	Ds1302_Bell_time_BCD[2]--;					 
				}
				//k=2	��[1]
				if(k==2)
				{
					if(Ds1302_Bell_time_BCD[1]>=10)	Ds1302_Bell_time_BCD[1]-=10;
					else	if(Ds1302_Bell_time_BCD[1]<10)	Ds1302_Bell_time_BCD[1]+=50; 
				}
				//k=3	��[1]
				if(k==3)
				{
					if(Ds1302_Bell_time_BCD[1]==0)	Ds1302_Bell_time_BCD[1]=59;
					else	if(Ds1302_Bell_time_BCD[1]>0)	Ds1302_Bell_time_BCD[1]--; 
				}
				break;
			}
/*����*/case 3:{k_before=k;k--;if(k<0) k=3;break;}
/*����*/case 4:{k_before=k;k++;if(k>3) k=0;break;}
		default:break;
	}
	delay_1ms(1);	
	write_add(7,Ds1302_Bell_time_BCD[1]);
	delay_1ms(5);
	write_add(8,Ds1302_Bell_time_BCD[2]);
	delay_1ms(5);
}
void set_shake(unsigned char shake_flag)
{
	//��������ʱ��
	switch(shake_flag)
	{

/*����*/case 1:
			{
				if(k==0)	ring_time_BCD+=10;				
				if(k==1)	ring_time_BCD++;
				if(ring_time_BCD==70)	ring_time_BCD=1;			
				if(ring_time_BCD>60)	ring_time_BCD=(ring_time_BCD%10);
				if(k==2)	shake_num_value+=100;				
				if(k==3)	shake_num_value+=10;
				if(k==4)	shake_num_value++;				
				if(shake_num_value>999)	shake_num_value=1;
				break;
			}
/*����*/case 2:
			{
				if(k==0)
				{
					if(ring_time_BCD==10)	ring_time_BCD=1;
					else
					{
						if(ring_time_BCD>10)	ring_time_BCD-=10;
						else	if(ring_time_BCD<10)	ring_time_BCD+=50; 
					}
				}
				if(k==1)
				{
					if(ring_time_BCD==1)	ring_time_BCD=60;
					else	if(ring_time_BCD>1)	ring_time_BCD--; 
				}
				if(k==2)
				{
					if(shake_num_value==100)	shake_num_value=1;
					else
					{
						if(shake_num_value<100)	shake_num_value+=900;
						else if(shake_num_value>100)	shake_num_value-=100;
					}
				}			
				if(k==3)
				{	
					if(shake_num_value==10)	shake_num_value=1;
					else
					{
						if(shake_num_value<10)	shake_num_value+=90;
						else if(shake_num_value>10) shake_num_value-=10;
					}
				}
				if(k==4)
				{
					if(shake_num_value==1)	shake_num_value=999;
					else if(shake_num_value>1)	shake_num_value--;
				}
				break;
			}
/*����*/case 3:{k_before=k;k--;if(k<0) k=4;break;}
/*����*/case 4:{k_before=k;k++;if(k>4) k=0;break;}
		default:break;
	}	
	delay_1ms(1);
	write_add(9,shake_num_value); 	
	delay_1ms(5);
	write_add(10,ring_time_BCD);
	ring_time_tmp=ring_time_BCD;
	delay_1ms(5);
}

//���ú���************************************************************************************************************

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
	delay_1ms(1);
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
	delay_1ms(1);
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
	delay_1ms(1);		
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
	if(ring==0)
	{
		if((reset==0)||(ring_time_BCD!=ring_display_time))
		{
			ring_display_time=ring_time_BCD;
			s_LCD_ShowNum( 24, 56, (ring_display_time/10) );									//
			s_LCD_ShowNum( 40, 56, (ring_display_time%10) );									//
		}
	}
	else if(ring==1)
	{
		if((reset==0)||(ring_time_tmp!=ring_display_time))
		{
			ring_display_time=ring_time_tmp;
			s_LCD_ShowNum( 24, 56, (ring_display_time/10) );									//
			s_LCD_ShowNum( 40, 56, (ring_display_time%10) );									//
		}
	}
	if(reset==0)
	{
		s_LCD_ShowNum( 56, 56, 10 );
		s_LCD_ShowNum( 72, 56, 11 );
		s_LCD_ShowNum( 88, 56, 12 );	
	}
	if(ring==0)
	{
		if((reset==0)||(shake_num_value!=shake_display_num))
		{
			shake_display_num=shake_num_value;
			s_LCD_ShowNum( 24, 96, (shake_display_num/100));									//
			s_LCD_ShowNum( 40, 96, ((shake_display_num/10)%10));									//
			s_LCD_ShowNum( 56, 96, (shake_display_num%10));									//
		}
	}
	else if(ring==1)
	{
		if((reset==0)||(shake_num!=shake_display_num))
		{
			shake_display_num=shake_num;
			s_LCD_ShowNum( 24, 96, (shake_display_num/100));									//
			s_LCD_ShowNum( 40, 96, ((shake_display_num/10)%10));									//
			s_LCD_ShowNum( 56, 96, (shake_display_num%10));
		}
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
	delay_1ms(1);
}
void position(void)
{
	if(modnum==0)
	{
		switch(k)
		{
			case 0:yy_now=16;break;
			case 1:yy_now=32;break;
			case 2:yy_now=64;break;
			case 3:yy_now=80;break;
			case 4:yy_now=112;break;
			case 5:yy_now=128;break;
			default:break;
		}
		switch(k_before)
		{
			case 0:yy_before=16;break;
			case 1:yy_before=32;break;
			case 2:yy_before=64;break;
			case 3:yy_before=80;break;
			case 4:yy_before=112;break;
			case 5:yy_before=128;break;
			default:break;
		}
		we_position(28,yy_before,1);
		we_position(28,yy_now,0);		
		goto sure_position;
	}
	if(modnum==1)
	{
		switch(k)
		{
			case 0:xx_now=88;break;
			case 1:xx_now=72;break;
			case 2:xx_now=40;break;
			case 3:xx_now=24;break;
			default:break;
		}
		switch(k_before)
		{
			case 0:xx_before=88;break;
			case 1:xx_before=72;break;
			case 2:xx_before=40;break;
			case 3:xx_before=24;break;
			default:break;
		}
		ns_position(xx_before,44,1);
		ns_position(xx_now,44,0);
		goto sure_position;
	}
	if(modnum==2)
	{
		switch(k)
		{
			case 0:yy_now=128;break;
			case 1:yy_now=112;break;
			case 2:yy_now=80;break;
			case 3:yy_now=64;break;
			case 4:yy_now=32;break;
			case 5:yy_now=16;break;
			default:break;
		}
		switch(k_before)
		{
			case 0:yy_before=128;break;
			case 1:yy_before=112;break;
			case 2:yy_before=80;break;
			case 3:yy_before=64;break;
			case 4:yy_before=32;break;
			case 5:yy_before=16;break;
			default:break;
		}
		we_position(96,yy_before,1);
		we_position(96,yy_now,0);
		goto sure_position;
	}
	if(modnum==3)
	{
		switch(k)
		{
			case 0:xx_now=24;yy_now=88;break;
			case 1:xx_now=40;yy_now=88;break;
			case 2:xx_now=24;yy_now=128;break;
			case 3:xx_now=40;yy_now=128;break;
			case 4:xx_now=56;yy_now=128;break;
			default:break;
		}
		switch(k_before)
		{
			case 0:xx_before=24;yy_before=88;break;
			case 1:xx_before=40;yy_before=88;break;
			case 2:xx_before=24;yy_before=128;break;
			case 3:xx_before=40;yy_before=128;break;
			case 4:xx_before=56;yy_before=128;break;
			default:break;
		}
		ns_position(xx_before,yy_before,1);
		ns_position(xx_now,yy_now,0);
		goto sure_position;
	}
sure_position:delay_1ms(1);
}

void clear_position(void)
{
	if(modnum==0) {we_clear(28,16,2);goto clear_over;}
	if(modnum==1) {ns_clear(24,44,4);goto clear_over;}
	if(modnum==2) {we_clear(96,16,2);goto clear_over;}
	if(modnum==3) {ns_clear(24,88,4);ns_clear(24,128,4);goto clear_over;}
clear_over:delay_1ms(1);	
}

void display(void)
{
	if(modnum!=modnum_cmp) 
	{
		modnum_cmp=modnum;
		reset=0;
	} 
	//��ʾ����
	if(reset==0)  LCD_Clear(BLACK);
	if(modnum==0) {w_mod0();goto show;}	
	if(modnum==1) {n_mod1();goto show;}
	if(modnum==2) {e_mod2();goto show;}
	if(modnum==3) {s_mod3();goto show;}
show:delay_1ms(1);
}

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
  	delay_1ms(1);
}
//����ֹͣʱ��BCDת��
void ring_changeinBCD(void)									
{
	unsigned char i,tmp;
	ring_stop_time_BCD[1]=Ds1302_Bell_time_BCD[2];
	ring_stop_time_BCD[0]=Ds1302_Bell_time_BCD[1]+ring_time_BCD;
	if(ring_stop_time_BCD[0]>59)	{ring_stop_time_BCD[0]-=60;ring_stop_time_BCD[1]++;}
	if(ring_stop_time_BCD[1]>23)	ring_stop_time_BCD[1]=0;
	delay_1ms(1);
	for(i=0;i<2;i++)		//BCD����
	{       
		tmp=ring_stop_time_BCD[i]/10;
		ring_stop_time[i]=ring_stop_time_BCD[i]%10;				
		ring_stop_time[i]=ring_stop_time[i]+tmp*16;
	}
	delay_1ms(1);
}
//����ת������**********************************************************************************************************

//Ds1302������**********************************************************************************************************
//дһ���ֽ�
void Ds1302_Write_Byte(unsigned char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	//ѭ��8�� д������
	{ 
		_nop_();
		Ds1302_SCK=0;
		_nop_();
		Ds1302_SDA=temp&0x01;     //ÿ�δ�����ֽ� 
		_nop_();
		temp>>=1;  		//����һλ
		_nop_();
		Ds1302_SCK=1;
	}
}  
//д��DS1302
void Ds1302_Write( unsigned char address,unsigned char dat )
{
	Ds1302_RST=0;
	_nop_();
	_nop_();
 	Ds1302_SCK=0;
	_nop_();
	_nop_();
 	Ds1302_RST=1;	
   	_nop_();                    //����
	_nop_();
 	Ds1302_Write_Byte(address);	//���͵�ַ
	_nop_();
	_nop_();
 	Ds1302_Write_Byte(dat);		//��������
 	Ds1302_RST=0;  		            //�ָ�
}
//����DS1302����
unsigned char Ds1302_Read ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	Ds1302_RST=0;
	_nop_();
 	Ds1302_SCK=0;
	_nop_();
 	Ds1302_RST=1;
	_nop_();
	_nop_();
 	Ds1302_Write_Byte(address);
 	for (i=0;i<8;i++) 		//ѭ��8�� ��ȡ����
 	{	
		_nop_();	
 		if(Ds1302_SDA)
 		temp|=0x80;			//ÿ�δ�����ֽ�
		_nop_();
		Ds1302_SCK=0;
		_nop_();
		temp>>=1;			//����һλ
		_nop_();
		Ds1302_SCK=1;
	} 
	_nop_();
 	Ds1302_RST=0;
	_nop_();	          	//����ΪDS1302��λ���ȶ�ʱ��
 	_nop_();
	Ds1302_RST=0;
	_nop_();
	_nop_();
	Ds1302_SCK=0;
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
    {;}
	} 
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

//�жϺ�����*************************************************************************************************************
//ģʽ�жϺ���
void judge_mode(void)
{
	if(y_value<(-230)) 
	{
		w_static_status++;
		n_static_status=0;
		e_static_status=0;
		s_static_status=0;
		if(w_static_status>=Static_status_num) {w_static_status=Static_status_num;modnum=0;}  
		goto Static_status;
	} 
	if(x_value>230) 
	{
		w_static_status=0;
		n_static_status++;
		e_static_status=0;
		s_static_status=0;
		if(n_static_status>=Static_status_num) {n_static_status=Static_status_num;modnum=1;}  
		goto Static_status;
	}
	if(y_value>230)
	{
		w_static_status=0;
		n_static_status=0;
		e_static_status++;
		s_static_status=0;
		if(e_static_status>=Static_status_num) {e_static_status=Static_status_num;modnum=2;} 
		goto Static_status;
	}  
	if(x_value<(-230)) 
	{
		w_static_status=0;
		n_static_status=0;
		e_static_status=0;
		s_static_status++;
		if(s_static_status>=Static_status_num) {s_static_status=Static_status_num;modnum=3;} 
		goto Static_status;
	}
Static_status: delay_10ms(1);
}

void judge_degree(void)
{
//modnum=0ʱ
	if(modnum==0)
	{
		if((y_value>(-221))&&(y_value<(-128))) 
		{
			if((z_value>(-221))&&(z_value<(-128)))	shake_flag=1;
			if((z_value>128)&&(z_value<221))		shake_flag=2;
			if((x_value>128)&&(x_value<221))		shake_flag=3;
			if((x_value>(-221))&&(x_value<(-128)))	shake_flag=4;	
			goto Shake_status;
		} 
	}
//modnum=1ʱ
	if(modnum==1)
	{
		if((x_value>128)&&(x_value<221)) 
		{
			if((z_value>(-221))&&(z_value<(-128)))	shake_flag=1;
			if((z_value>128)&&(z_value<221))		shake_flag=2;
			if((y_value>128)&&(y_value<221))		shake_flag=3;
			if((y_value>(-221))&&(y_value<(-128)))	shake_flag=4;	
			goto Shake_status;
		} 
	}
//modnum=2ʱ
	if(modnum==2)
	{
		if((y_value>128)&&(y_value<221)) 
		{
			if((z_value>(-221))&&(z_value<(-128)))	shake_flag=1;
			if((z_value>128)&&(z_value<221))		shake_flag=2;
			if((x_value>(-221))&&(x_value<(-128)))	shake_flag=3;
			if((x_value>128)&&(x_value<221))		shake_flag=4;	
			goto Shake_status;
		} 
	}
//modnum=3ʱ
	if(modnum==3)
	{
		if((x_value>(-221))&&(x_value<(-128))) 
		{
			if((z_value>(-221))&&(z_value<(-128)))	shake_flag=1;
			if((z_value>128)&&(z_value<221))		shake_flag=2;
			if((y_value>(-221))&&(y_value<(-128)))	shake_flag=3;
			if((y_value>128)&&(y_value<221))		shake_flag=4;		
			goto Shake_status;
		} 
	}

Shake_status: delay_10ms(1);
}
void judge_shake(void)
{
	if((x_value>400)||(y_value>400)||(z_value>400)||(x_value<(-400))||(y_value<(-400))||(z_value<(-400)) )
	shake_num--;//CHECK_ring_shake=~CHECK_ring_shake;
	delay_10ms(1);
}
//�жϺ�����*************************************************************************************************************

//EEPROM���ݶ�д������***************************************************************************************************
void E2P_readdata(void)
{
	Ds1302_time_BCD[0]=read_add(0);
	delay_1ms(5);
	Ds1302_time_BCD[1]=read_add(1);
	delay_1ms(5);
	Ds1302_time_BCD[2]=read_add(2);
	delay_1ms(5);
   	Ds1302_time_BCD[3]=read_add(3);
	delay_1ms(5);
	Ds1302_time_BCD[4]=read_add(4);
	delay_1ms(5);
	Ds1302_time_BCD[6]=read_add(6);
	delay_1ms(5);	

	Ds1302_Bell_time_BCD[1]=read_add(7);
	delay_1ms(5);
 	Ds1302_Bell_time_BCD[2]=read_add(8);
	delay_1ms(5);

	shake_num_value=read_add(9); 
	delay_1ms(5);	
	ring_time_BCD=read_add(10);
	delay_1ms(5);
}
//EEPROM���ݶ�д������***************************************************************************************************

void judge_hlir(void)
{
     //BCD����
	Ds1302_time_tmp=(Ds1302_time[1]%16)+(Ds1302_time[1]/16)*10;

	if(Ds1302_time_tmp-Ds1302_Bell_time_BCD[1]>0) 
		ring_time_tmp=ring_time_BCD-(Ds1302_time_tmp-Ds1302_Bell_time_BCD[1]);

	else if(Ds1302_time_tmp-Ds1302_Bell_time_BCD[1]<0) 
		ring_time_tmp=(60-ring_time_BCD)+Ds1302_time_tmp;

	if(ring_time_tmp>60) ring_time_tmp-=60;
	delay_1ms(5);
}
 