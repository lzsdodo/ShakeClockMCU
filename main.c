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

//DS1302相关地址定义
#define Ds1302_sec_addr			0x80	//0x81		//秒数据地址	bit1=1时，低功耗，时钟振荡器停止工作；bit1=0时，正常工作。
#define Ds1302_min_addr			0x82	//0x83		//分数据地址
#define Ds1302_hr_addr			0x84	//0x85		//时数据地址	bit7=1时，12小时模式；bit7=0时，24小时模式；
#define Ds1302_date_addr		0x86	//0x87		//日数据地址
#define Ds1302_month_addr		0x88	//0x89		//月数据地址
#define Ds1302_day_addr			0x8a	//0x8b		//星期数据地址
#define Ds1302_year_addr		0x8c	//0x8d		//年数据地址
#define Ds1302_control_addr		0x8e	//0x8f		//控制数据地址		0x80时无法写入DS1302；0x00时可写入；(bit7=1/0,其他全为0；)
#define Ds1302_charger_addr		0x90	//0x91		//涓流充电模式 	 
#define Ds1302_clkburst_addr	0xbe	//0xbf		//时钟突发模式寄存器
#define Ds1302_ramburst_addr	0xfe	//0xff		//RAM突发模式寄存器

//ADXL345相关地址定义
#define SlaveAddress   0xA6   //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改	\
 //ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A
                      

sbit Ds1302_SCK = P2^5;	//时钟	
sbit Ds1302_SDA = P2^6;	//数据	
sbit Ds1302_RST = P2^7; //DS1302复位

/*
//开发板DS1302管脚定义
sbit Ds1302_SCK = P3^6;	//时钟	
sbit Ds1302_SDA = P3^4;	//数据	
sbit Ds1302_RST = P3^5; //DS1302复位
*/
//ADXL345加速度芯片管脚定义
sbit   SCL=P1^1;      //IIC时钟引脚定义
sbit   SDA=P1^0;      //IIC数据引脚定义

sbit BEEP = P3^5;	
  
//sbit CHECK = P0^0;
//sbit CHECK_while1_howlong = P0^1;
//sbit CHECK_ring_shake = P0^2;


//定义标志
bit ReadRTC_Flag;									//读DS1302标志
bit reset;											//显示初始化标志
bit setmod;											//设置模式 	0的时候正常显示，1的时候进入设置模式
bit clear;
bit ring;											//切换闹铃模式状态标志


//DS1302相关数组定义
static unsigned char Ds1302_time_BCD[8];//={40,56,11,22,9,4,13,0};				//秒分时日月周年13-08-19 09:33:00		 BCD码
static unsigned char Ds1302_time[8];										//秒分时日月周年  ds1302内部
static unsigned char Ds1302_display_time[8];								//时间显示缓冲数组
//ADXL345相关数组定义
static unsigned char BUF[8];                         						//接收数据缓存区 
static int  dis_data;                       								//变量 
static int x_value,y_value,z_value;
static uchar w_static_status,n_static_status,e_static_status,s_static_status;

//EEPROM相关数组定义
static unsigned char e2prom_add;

//闹铃数组定义
static unsigned char Ds1302_Bell_time_BCD[3]={0,1,12};						//闹铃时间：分-时						 BCD码
static unsigned char Ds1302_Bell_time[3];									//闹铃时间,16进制
static unsigned char Ds1302_Bell_display_time[2];							//闹铃时间显示缓冲数组
static unsigned char Ds1302_time_tmp;
//摇一摇相关定义
static unsigned int  shake_num_value,shake_num,shake_display_num;			//摇摇次数以及其显示缓冲
static unsigned char ring_time_BCD,ring_time_tmp,ring_display_time;			//响铃时间以及其显示缓冲
static unsigned char ring_stop_time[2],ring_stop_time_BCD[2];

static unsigned char Static_status_num=10;
static unsigned char shake_flag=0;
static unsigned char xx_now,xx_before,yy_now,yy_before;

//标志位相关
static char k,k_before;
static unsigned char modnum,modnum_cmp;


/******************************************************************/
/*                    函数声明                                    */
/******************************************************************/
//DS1302函数声明
void Ds1302_Write_Byte(unsigned char temp);
void Ds1302_Write( unsigned char address,unsigned char dat );
unsigned char Ds1302_Read ( unsigned char address );
void Ds1302_Read_RTC(void);
void Ds1302_Set_RTC(void);
//ADXL345函数声明
void delay(unsigned int k);
void Init_ADXL345(void);             //初始化ADXL345
void WriteDataLCM(uchar dataW);
void WriteCommandLCM(uchar CMD,uchar Attribc);
//void DisplayOneChar(uchar X,uchar Y,uchar DData);
void conversion(uint temp_data);
void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   //单个写入数据
uchar Single_Read_ADXL345(uchar REG_Address);                   //单个读取内部寄存器数据
void  Multiple_Read_ADXL345();                                  //连续的读取内部寄存器数据
 
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

//判断函数声明
void set_parameter(void);
void judge_mode(void);
void judge_degree(void);
void judge_shake(void);
void judge_hlir(void);

//彩屏显示函数声明
void display(void);
void w_mod0(void);
void n_mod1(void);
void e_mod2(void);
void s_mod3(void);
void position(void);
void clear_position(void);


//设置函数声明
void set_time(unsigned char shake_flag);
void set_ring(unsigned char shake_flag);
void set_date(unsigned char shake_flag);
void set_shake(unsigned char shake_flag);

//数据转换函数声明
void bell_changeinBCD(void);
void ring_changeinBCD(void);

//初始化函数声明
void Int0_Init(void);
void main_Init(void);
void E2P_readdata(void);
void Data_Init(void);

/******************************************************************/
/*                    中断函数                                    */
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
//初始化参数   
	main_Init();            		
	EEPROM_Init();					//初始化EEPROM
	Init_ADXL345();                 //初始化ADXL345
	Int0_Init();					//初始化INT0中断
	E2P_readdata();
	Data_Init();		
	Lcd_Init();   					//tft初始化
//循环！！！
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

//初始化函数**********************************************************************************************************
//函数初始化
void main_Init(void)
{
	//屏幕参数初始化
	LCD_Clear(BLACK); 			//清屏
	BACK_COLOR=BLACK;			//背景色：黑
	POINT_COLOR=WHITE; 			//点颜色: 白
	//模式以及相关参数初始化
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
	//摇一摇闹铃初始数值设定(可用改为读取EEPROM数据)
	shake_num=shake_num_value;
	shake_display_num=shake_num_value;
	ring_time_tmp=ring_time_BCD;
	ring_display_time=ring_time_BCD;
	Ds1302_Bell_time_BCD[0]=0;
	bell_changeinBCD();
	ring_changeinBCD();
	
	//时间日期初始化
	for(i=0;i<8;i++)
	Ds1302_display_time[i]=Ds1302_time[i];
	delay_10ms(30);
}
//中断初始化
void Int0_Init(void)
{
	EA = 0;
    IT0 = 1; //负边沿触发中断
    EX0 = 1; //允许外部中断
    EA = 1;
	delay_10ms(3);
}
//初始化函数**********************************************************************************************************

//设置函数************************************************************************************************************
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
			//包括set_shake_num(num); set_ring_time(num);
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
/*上移*/case 1:
			{
				//k=0	时[2]
				if(k==0)
				{	
					Ds1302_time_BCD[2]+=10;				
					if((Ds1302_time_BCD[2]>23)&&(Ds1302_time_BCD[2]<30))	Ds1302_time_BCD[2]=23;
					if(Ds1302_time_BCD[2]>=30)	Ds1302_time_BCD[2]=(Ds1302_time_BCD[2]%10);
				}
				//k=1	时[2]
				if(k==1)
				{
					Ds1302_time_BCD[2]++;
					if(Ds1302_time_BCD[2]>23)	Ds1302_time_BCD[2]=0;
				}																 
				//k=2	分[1]	
				if(k==2)	Ds1302_time_BCD[1]+=10;
				//k=3	分[1]				
				if(k==3)	Ds1302_time_BCD[1]++;					
				if(Ds1302_time_BCD[1]>=60)	Ds1302_time_BCD[1]=(Ds1302_time_BCD[1]%10);
				//k=4	秒[0]
				if(k==4)	Ds1302_time_BCD[0]+=10;
				//k=5	秒[0]
				if(k==5)	Ds1302_time_BCD[0]++;
				if(Ds1302_time_BCD[0]>=60) 	Ds1302_time_BCD[0]=(Ds1302_time_BCD[0]%10);
				break;
			}
/*下移*/case 2:	
			{
				//k=0	时[2]
				if(k==0)
				{									
					if(Ds1302_time_BCD[2]<10)
					{	
						if(Ds1302_time_BCD[2]>=4)	Ds1302_time_BCD[2]=23;
						if(Ds1302_time_BCD[2]<4)	Ds1302_time_BCD[2]+=20;
					}
					else	if(Ds1302_time_BCD[2]>=10)	Ds1302_time_BCD[2]-=10;			
				}
				//k=1	时[2]				
				if(k==1)
				{
					if(Ds1302_time_BCD[2]==0)	Ds1302_time_BCD[2]=23;
					else	if(Ds1302_time_BCD[2]>0)	Ds1302_time_BCD[2]--;					 
				}
				//k=2	分[1]
				if(k==2)
				{
					if(Ds1302_time_BCD[1]>=10)	Ds1302_time_BCD[1]-=10;
					else	if(Ds1302_time_BCD[1]<10)	Ds1302_time_BCD[1]+=50; 
				}
				//k=3	分[1]
				if(k==3)
				{
					if(Ds1302_time_BCD[1]==0)	Ds1302_time_BCD[1]=59;
					else	if(Ds1302_time_BCD[1]>0)	Ds1302_time_BCD[1]--; 
				}
				//k=4	秒[0]
				if(k==4)
				{
					if(Ds1302_time_BCD[0]>=10)	Ds1302_time_BCD[0]-=10;
					else	if(Ds1302_time_BCD[0]<10)	Ds1302_time_BCD[0]+=50; 
				}
				//k=5	秒[0]
				if(k==5)
				{	
					if(Ds1302_time_BCD[0]==0)	Ds1302_time_BCD[0]=59;
					else	if(Ds1302_time_BCD[0]>0)	Ds1302_time_BCD[0]--;
				}
				break;
			}
/*左移*/case 3:{k_before=k;k--;if(k<0) k=5;break;}
/*右移*/case 4:{k_before=k;k++;if(k>5) k=0;break;}
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
/*上移*/case 1:
			{
				//k=0	年[6]
				if(k==0)	Ds1302_time_BCD[6]+=10;
				//k=1	年[6]
				if(k==1)	Ds1302_time_BCD[6]++;
				//k=2	月[4]
				if(k==2)	Ds1302_time_BCD[4]+=10;								
				if((k==2)&&(Ds1302_time_BCD[4]>12)&&(Ds1302_time_BCD[4]<=20))	Ds1302_time_BCD[4]=12;
				if((k==2)&&(Ds1302_time_BCD[4]>20))	Ds1302_time_BCD[4]=(Ds1302_time_BCD[4]%10);
				//k=3	月[4]
				if(k==3)	Ds1302_time_BCD[4]++;
				if((k==3)&&(Ds1302_time_BCD[4]>12))	Ds1302_time_BCD[4]=1;
				//k=4	日[3]
				if(k==4)	Ds1302_time_BCD[3]+=10;							
				if((k==4)&&(Ds1302_time_BCD[3]<40)&&(Ds1302_time_BCD[3]>31))	Ds1302_time_BCD[3]=31;
				if((k==4)&&(Ds1302_time_BCD[3]>=40))	Ds1302_time_BCD[3]=(Ds1302_time_BCD[3]%10);
				//k=5	日[3]
				if(k==5)	Ds1302_time_BCD[3]++;
				if((k==5)&&(Ds1302_time_BCD[3]>31))	Ds1302_time_BCD[3]=1;
				break;
			} 
/*下移*/case 2:
			{
				if(k==0)
				{
				 	if(Ds1302_time_BCD[6]<10)	Ds1302_time_BCD[6]+=30;
					else	if(Ds1302_time_BCD[6]>=10)	Ds1302_time_BCD[6]-=10;	
				}  
				//k=0	年[6] 				
				if(k==1)
				{
					if(Ds1302_time_BCD[6]==0)	Ds1302_time_BCD[6]=30;	
					else	if(Ds1302_time_BCD[6]>0)	Ds1302_time_BCD[6]--;
				}
				//k=2	月[4] 
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
				//k=3	月[4]
				if(k==3)
				{
					if(Ds1302_time_BCD[4]==1)	Ds1302_time_BCD[4]=12;
					else	if(Ds1302_time_BCD[4]>1)	Ds1302_time_BCD[4]--;
				}
				//k=4	日[3]
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
				//k=5	日[3]
				if(k==5)
				{
					if(Ds1302_time_BCD[3]==1) Ds1302_time_BCD[3]=31;
					else	if(Ds1302_time_BCD[3]>1)	Ds1302_time_BCD[3]--;
				}
				break;
			}
/*左移*/case 3:{k_before=k;k--;if(k<0) k=5;break;}
/*右移*/case 4:{k_before=k;k++;if(k>5) k=0;break;}
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
	//设置闹钟时间
	switch(shake_flag)
	{

/*上移*/case 1:
			{
				//k=0	时[2]
				if(k==0)
				{	
					Ds1302_Bell_time_BCD[2]+=10;				
					if((Ds1302_Bell_time_BCD[2]>23)&&(Ds1302_Bell_time_BCD[2]<30))	Ds1302_Bell_time_BCD[2]=23;
					if(Ds1302_Bell_time_BCD[2]>=30)	Ds1302_Bell_time_BCD[2]=(Ds1302_Bell_time_BCD[2]%10);
				}
				//k=1	时[2]
				if(k==1)
				{
					Ds1302_Bell_time_BCD[2]++;
					if(Ds1302_Bell_time_BCD[2]>23)	Ds1302_Bell_time_BCD[2]=0;
				}																 
				//k=2	分[1]
				if(k==2)	Ds1302_Bell_time_BCD[1]+=10;
				//k=3	分[1]				
				if(k==3)	Ds1302_Bell_time_BCD[1]++;					
				if(Ds1302_Bell_time_BCD[1]>=60)	Ds1302_Bell_time_BCD[1]=(Ds1302_Bell_time_BCD[1]%10);	
				break;
			}
/*下移*/case 2:
			{
				//k=0	时[2]
				if(k==0)
				{									
					if(Ds1302_Bell_time_BCD[2]<10)
					{
						if(Ds1302_Bell_time_BCD[2]>=4)	Ds1302_Bell_time_BCD[2]=23;
						if(Ds1302_Bell_time_BCD[2]<4)	Ds1302_Bell_time_BCD[2]+=20;
					}
					else	if(Ds1302_Bell_time_BCD[2]>=10)	Ds1302_Bell_time_BCD[2]-=10;			
				}
				//k=1	时[2]				
				if(k==1)
				{
					if(Ds1302_Bell_time_BCD[2]==0)	Ds1302_Bell_time_BCD[2]=23;
					else	if(Ds1302_Bell_time_BCD[2]>0)	Ds1302_Bell_time_BCD[2]--;					 
				}
				//k=2	分[1]
				if(k==2)
				{
					if(Ds1302_Bell_time_BCD[1]>=10)	Ds1302_Bell_time_BCD[1]-=10;
					else	if(Ds1302_Bell_time_BCD[1]<10)	Ds1302_Bell_time_BCD[1]+=50; 
				}
				//k=3	分[1]
				if(k==3)
				{
					if(Ds1302_Bell_time_BCD[1]==0)	Ds1302_Bell_time_BCD[1]=59;
					else	if(Ds1302_Bell_time_BCD[1]>0)	Ds1302_Bell_time_BCD[1]--; 
				}
				break;
			}
/*左移*/case 3:{k_before=k;k--;if(k<0) k=3;break;}
/*右移*/case 4:{k_before=k;k++;if(k>3) k=0;break;}
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
	//设置闹钟时间
	switch(shake_flag)
	{

/*上移*/case 1:
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
/*下移*/case 2:
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
/*左移*/case 3:{k_before=k;k--;if(k<0) k=4;break;}
/*右移*/case 4:{k_before=k;k++;if(k>4) k=0;break;}
		default:break;
	}	
	delay_1ms(1);
	write_add(9,shake_num_value); 	
	delay_1ms(5);
	write_add(10,ring_time_BCD);
	ring_time_tmp=ring_time_BCD;
	delay_1ms(5);
}

//设置函数************************************************************************************************************

//显示函数区**********************************************************************************************************
//显示模式0：时:分:秒
void w_mod0(void)
{
	//汉字部分
	if(reset==0)
	{
		showhanzi(80,40,0);
		showhanzi(80,88,1);
	}
	//数字部分
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

//显示模式1：显示闹铃时间: 时:分
void n_mod1(void)		  
{
	//汉字部分
	if(reset==0)
	{
		showhanzi(72,112,4);
		showhanzi(24,112,5);
	}
	//数字部分
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
//显示模式2：年-月-日
void e_mod2(void)
{
	//汉字部分
	if(reset==0)
	{
		showhanzi(16,88,2);
		showhanzi(16,40,3);
	}
	//数字部分
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

//闹铃延时显示:闹铃延续时间以及摇一摇的次数
void s_mod3(void)
{
	//汉字部分
	if(reset==0)
	{
		showhanzi(16,16,6);
		showhanzi(48,16,7);
		showhanzi(80,16,6);	
	}
	//数字部分
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
	//显示部分
	if(reset==0)  LCD_Clear(BLACK);
	if(modnum==0) {w_mod0();goto show;}	
	if(modnum==1) {n_mod1();goto show;}
	if(modnum==2) {e_mod2();goto show;}
	if(modnum==3) {s_mod3();goto show;}
show:delay_1ms(1);
}

//显示函数区************************************************************************************************************

//数据转换函数**********************************************************************************************************
//闹钟时间BCD转化
void bell_changeinBCD(void)
{
	unsigned char i,tmp;
	for(i=0;i<3;i++)		//BCD处理
	{       
		tmp=Ds1302_Bell_time_BCD[i]/10;
		Ds1302_Bell_time[i]=Ds1302_Bell_time_BCD[i]%10;
		Ds1302_Bell_time[i]=Ds1302_Bell_time[i]+tmp*16;
	} 
  	delay_1ms(1);
}
//闹钟停止时间BCD转化
void ring_changeinBCD(void)									
{
	unsigned char i,tmp;
	ring_stop_time_BCD[1]=Ds1302_Bell_time_BCD[2];
	ring_stop_time_BCD[0]=Ds1302_Bell_time_BCD[1]+ring_time_BCD;
	if(ring_stop_time_BCD[0]>59)	{ring_stop_time_BCD[0]-=60;ring_stop_time_BCD[1]++;}
	if(ring_stop_time_BCD[1]>23)	ring_stop_time_BCD[1]=0;
	delay_1ms(1);
	for(i=0;i<2;i++)		//BCD处理
	{       
		tmp=ring_stop_time_BCD[i]/10;
		ring_stop_time[i]=ring_stop_time_BCD[i]%10;				
		ring_stop_time[i]=ring_stop_time[i]+tmp*16;
	}
	delay_1ms(1);
}
//数据转换函数**********************************************************************************************************

//Ds1302函数区**********************************************************************************************************
//写一个字节
void Ds1302_Write_Byte(unsigned char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	//循环8次 写入数据
	{ 
		_nop_();
		Ds1302_SCK=0;
		_nop_();
		Ds1302_SDA=temp&0x01;     //每次传输低字节 
		_nop_();
		temp>>=1;  		//右移一位
		_nop_();
		Ds1302_SCK=1;
	}
}  
//写入DS1302
void Ds1302_Write( unsigned char address,unsigned char dat )
{
	Ds1302_RST=0;
	_nop_();
	_nop_();
 	Ds1302_SCK=0;
	_nop_();
	_nop_();
 	Ds1302_RST=1;	
   	_nop_();                    //启动
	_nop_();
 	Ds1302_Write_Byte(address);	//发送地址
	_nop_();
	_nop_();
 	Ds1302_Write_Byte(dat);		//发送数据
 	Ds1302_RST=0;  		            //恢复
}
//读出DS1302数据
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
 	for (i=0;i<8;i++) 		//循环8次 读取数据
 	{	
		_nop_();	
 		if(Ds1302_SDA)
 		temp|=0x80;			//每次传输低字节
		_nop_();
		Ds1302_SCK=0;
		_nop_();
		temp>>=1;			//右移一位
		_nop_();
		Ds1302_SCK=1;
	} 
	_nop_();
 	Ds1302_RST=0;
	_nop_();	          	//以下为DS1302复位的稳定时间
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
	return (temp);			//返回
}
//read RTC 读时钟数据
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
	   {           //BCD处理
		tmp=Ds1302_time[i]/16;
		Ds1302_time_BCD[i]=Ds1302_time[i]%16;
		Ds1302_time_BCD[i]=Ds1302_time_BCD[i]+tmp*10;
	   }
}
//set RTC	设定时钟数据 
void Ds1302_Set_RTC(void)
{
	unsigned char i,tmp;                
	for(i=0;i<8;i++)
	{                  //BCD处理
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
//Ds1302函数区**********************************************************************************************************

//ADXL345函数区**********************************************************************************************************
/**************延时*****************/
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
延时5微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数，注意时钟过快时需要修改
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
}
/**************************************
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5ms()
{
    WORD n = 560;
    while (n--);
}
/**************************************
起始信号
**************************************/
void ADXL345_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}
/**************************************
停止信号
**************************************/
void ADXL345_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}
/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void ADXL345_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}
/**************************************
接收应答信号
**************************************/
bit ADXL345_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return CY;
}
/**************************************
向IIC总线发送一个字节数据
**************************************/
void ADXL345_SendByte(BYTE dat)
{
    BYTE i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    ADXL345_RecvACK();
}
/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE ADXL345_RecvByte()
{
    BYTE i;
    BYTE dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}
//******单字节写入*******************************************
 
void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //起始信号
    ADXL345_SendByte(SlaveAddress);   //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);    //内部寄存器地址，请参考中文pdf22页 
    ADXL345_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf22页 
    ADXL345_Stop();                   //发送停止信号
}
//********单字节读取*****************************************
uchar Single_Read_ADXL345(uchar REG_Address)
{  
uchar REG_data;
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress);           //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);            //发送存储单元地址，从0开始 
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=ADXL345_RecvByte();              //读出寄存器数据
    ADXL345_SendACK(1);   
    ADXL345_Stop();                           //停止信号
    return REG_data; 
}	   
//*********************************************************
//连续读出ADXL345内部加速度数据，地址范围0x32~0x37
//*********************************************************
void Multiple_read_ADXL345(void)
{   
	uchar i;
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress);           //发送设备地址+写信号
    ADXL345_SendByte(0x32);                   //发送存储单元地址，从0x32开始 
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
    {
        BUF[i] = ADXL345_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 5)
        {
           ADXL345_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //回应ACK
        }
    }
    ADXL345_Stop();                          //停止信号
    Delay5ms();
}
//*****************************************************************
//初始化ADXL345，根据需要请参考pdf进行修改************************
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
    delay(500);			   //上电延时
	//上面这段本在初始化函数前面
    Single_Write_ADXL345(0x31,0x0B);   //测量范围,正负16g，13位模式
    Single_Write_ADXL345(0x2C,0x08);   //速率设定为12.5 参考pdf13页
    Single_Write_ADXL345(0x2D,0x08);   //选择电源模式   参考pdf24页
    Single_Write_ADXL345(0x2E,0x80);   //使能 DATA_READY 中断
    Single_Write_ADXL345(0x1E,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
    Single_Write_ADXL345(0x1F,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
    Single_Write_ADXL345(0x20,0x05);   //Z 偏移量 根据测试传感器的状态写入pdf29页
}
void ADXL345_AXYZ(void)
{
	Multiple_Read_ADXL345();        //连续读出数据，存储在BUF中
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
//ADXL345函数区**********************************************************************************************************

//判断函数区*************************************************************************************************************
//模式判断函数
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
//modnum=0时
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
//modnum=1时
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
//modnum=2时
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
//modnum=3时
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
//判断函数区*************************************************************************************************************

//EEPROM数据读写函数区***************************************************************************************************
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
//EEPROM数据读写函数区***************************************************************************************************

void judge_hlir(void)
{
     //BCD处理
	Ds1302_time_tmp=(Ds1302_time[1]%16)+(Ds1302_time[1]/16)*10;

	if(Ds1302_time_tmp-Ds1302_Bell_time_BCD[1]>0) 
		ring_time_tmp=ring_time_BCD-(Ds1302_time_tmp-Ds1302_Bell_time_BCD[1]);

	else if(Ds1302_time_tmp-Ds1302_Bell_time_BCD[1]<0) 
		ring_time_tmp=(60-ring_time_BCD)+Ds1302_time_tmp;

	if(ring_time_tmp>60) ring_time_tmp-=60;
	delay_1ms(5);
}
 