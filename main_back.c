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
#define SlaveAddress   0xA6   //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A

#define Status_num 100


sbit Ds1302_SCK = P3^0;	//时钟	
sbit Ds1302_SDA = P3^1;	//数据	
sbit Ds1302_RST = P3^2; //DS1302复位

/*
//开发板DS1302管脚定义
sbit Ds1302_SCK = P3^6;	//时钟	
sbit Ds1302_SDA = P3^4;	//数据	
sbit Ds1302_RST = P3^5; //DS1302复位
*/
//ADXL345加速度芯片管脚定义
sbit   SCL=P1^1;      //IIC时钟引脚定义
sbit   SDA=P1^0;      //IIC数据引脚定义

//定义标志
bit ReadRTC_Flag;									//读DS1302标志
bit reset;											//显示初始化标志
bit mod;											//模式标志
bit setmod;											//设置模式 	0的时候正常显示，1的时候进入设置模式
bit settime;										//切换设置时钟状态标志
bit setbell;										//切换设置闹铃状态标志
bit ring;											//切换闹铃模式状态标志
bit rering;											//闹铃再次可触发标志

//DS1302相关数组定义
static unsigned char Ds1302_time_BCD[8]={40,34,9,18,9,4,13,0};				//秒分时日月周年13-08-19 09:33:00		 BCD码
static unsigned char Ds1302_time[8];										//秒分时日月周年  ds1302内部
static unsigned char Ds1302_display_time[8];								//时间显示缓冲数组
//ADXL345相关数组定义
static unsigned char BUF[8];                         						//接收数据缓存区 
static int  dis_data;                       								//变量 
static int x_value,y_value,z_value;
static uchar w_status,n_status,e_status,s_status;

//闹铃数组定义
static unsigned char Ds1302_Bell_time_BCD[3]={0,30,6};						//闹铃时间：分-时						 BCD码
static unsigned char Ds1302_Bell_time[3];									//闹铃时间,16进制
static unsigned char Ds1302_Bell_display_time[2];							//闹铃时间显示缓冲数组
//摇一摇相关定义
static unsigned int  shake_num_value,shake_num,shake_display_num;			//摇摇次数以及其显示缓冲
static unsigned char ring_time_BCD,ring_display_time;						//响铃时间以及其显示缓冲
static unsigned char ring_stop_time[2],ring_stop_time_BCD[2];
//标志位相关
static unsigned char setring;												//闹铃设置参数
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

//彩屏显示函数声明
void judge_mode(void);
void w_mod0(void);
void n_mod1(void);
void e_mod2(void);
void s_mod3(void);
//void a_mod4(void);
//数据转换函数声明
void bell_changeinBCD(void);
void ring_changeinBCD(void);
//初始化函数声明
void Int0_Init(void);
void main_Init(void);

/******************************************************************/
/*                    中断函数                                    */
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
//	}		//改时间设定键 ;按下0-1转换，setmod=1时，闹钟模式为0；k=0；
//	if(Key==0x06)	
//	{
//		setbell=~setbell;		
//		if(setbell) {reset=0;settime=0;setring=0;k=0;}
//		if(!setbell) Flash(8);
//	}	//改闹铃设定键 ;按下0-1转换，setbell=1时，时钟模式为0；k=0;重新闹钟显示参量cpr_bell为0；
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
	Lcd_Init();   //tft初始化
	Init_ADXL345();                  //初始化ADXL345
	
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

//初始化函数**********************************************************************************************************
//函数初始化
void main_Init(void)
{
	unsigned char i;
	//屏幕参数初始化
	LCD_Clear(BLACK); 			//清屏
	BACK_COLOR=BLACK;			//背景色：黑
	POINT_COLOR=WHITE; 			//点颜色: 白
	//模式以及相关参数初始化
	w_status=0;
	n_status=0;
	e_status=0;
	s_status=0;
	modnum=0;
	modnum_cmp=modnum;
	setmod=0;
	
	mod=0;						  //模式初始化 mod是显示时分秒/年月日按键7改;
	settime=0;
	setbell=0;
	setring=0;	
	ring=0;
	rering=0;
	reset=0;
	//摇一摇闹铃初始数值设定(可用改为读取EEPROM数据)
	shake_num_value=10;
	shake_num=shake_num_value;
	shake_display_num=shake_num;
	ring_time_BCD=10;
	ring_display_time=ring_time_BCD;
	
	for(i=0;i<8;i++)
	Ds1302_display_time[i]=Ds1302_time[i];	
}
//中断初始化
void Int0_Init(void)
{
	EA = 0;
    IT0 = 1; //负边沿触发中断
    EX0 = 1; //允许外部中断
    EA = 1;
}
//初始化函数**********************************************************************************************************

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
/*加速度显示
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
}
//闹钟停止时间BCD转化
void ring_changeinBCD(void)									
{
	unsigned char i,tmp;
	ring_stop_time_BCD[1]=Ds1302_Bell_time_BCD[2];
	ring_stop_time_BCD[0]=Ds1302_Bell_time_BCD[1]+ring_time_BCD;
	if(ring_stop_time_BCD[0]>59)	{ring_stop_time_BCD[0]-=60;ring_stop_time_BCD[1]++;}
	if(ring_stop_time_BCD[1]>23)	ring_stop_time_BCD[1]=0;
	for(i=0;i<8;i++)_nop_();
	for(i=0;i<2;i++)		//BCD处理
	{       
		tmp=ring_stop_time_BCD[i]/10;
		ring_stop_time[i]=ring_stop_time_BCD[i]%10;				
		ring_stop_time[i]=ring_stop_time[i]+tmp*16;
	}
	for(i=0;i<8;i++)_nop_(); 
}
//数据转换函数**********************************************************************************************************

//Ds1302函数区**********************************************************************************************************
//写一个字节
void Ds1302_Write_Byte(unsigned char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	//循环8次 写入数据
	{ 
		Ds1302_SCK=0;
		Ds1302_SDA=temp&0x01;     //每次传输低字节 
		temp>>=1;  		//右移一位
		Ds1302_SCK=1;
	}
}  
//写入DS1302
void Ds1302_Write( unsigned char address,unsigned char dat )
{
	Ds1302_RST=0;
	_nop_();
 	Ds1302_SCK=0;
	_nop_();
 	Ds1302_RST=1;	
   	_nop_();                    //启动
 	Ds1302_Write_Byte(address);	//发送地址
 	Ds1302_Write_Byte(dat);		//发送数据
 	Ds1302_RST=0;  		            //恢复
}
//读出DS1302数据
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
 	for (i=0;i<8;i++) 		//循环8次 读取数据
 	{		
 		if(Ds1302_SDA)
 		temp|=0x80;			//每次传输低字节
		Ds1302_SCK=0;
		temp>>=1;			//右移一位
		_nop_();
		_nop_();
		_nop_();
		Ds1302_SCK=1;
	} 
 	Ds1302_RST=0;
	_nop_();	          	//以下为DS1302复位的稳定时间
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
    {;}} 
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
void delay_ser(unsigned char x)
{
	unsigned char i,m;
	for(i=255;i>0;i--)
		for(m=x;m>0;m--);
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
