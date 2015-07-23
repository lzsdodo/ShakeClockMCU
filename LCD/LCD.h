#ifndef __LCD_H
#define __LCD_H		

/* 
程序默认IO连接方式：
sbit LCD_CS     =P0^0;     //片选	
sbit LCD_AO     =P0^2;	  //数据/命令切换
sbit LCD_SDA    =P1^5;	  //数据
sbit LCD_SCK    =P1^7;	  //时钟
sbit LCD_REST   =P0^1;	  //复位  
*/ 
 //定义LCD的尺寸	
#define LCD_W 128
#define LCD_H 160

//IO连接  
sbit LCD_CS     =P2^0;     //片选	
sbit LCD_AO     =P2^2;	  //数据/命令切换
sbit LCD_SDA    =P2^3;	  //数据
sbit LCD_SCK    =P2^4;	  //时钟
sbit LCD_REST   =P2^1;	  //复位  

extern unsigned int BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void delayms(int count);

void LCD_Writ_Bus(char da);
void LCD_WR_DATA8(char da);
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);
void LCD_WR_REG_DATA(int reg,int da);

extern void Lcd_Init(void); 
extern void LCD_Clear(unsigned int Color); 
extern void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
extern void showhanzi(unsigned int x,unsigned int y,unsigned char index);

//下面测试程序
extern void w_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//w方向显示数字
extern void e_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//e方向显示数字

extern void we_position(unsigned int x,unsigned int y,unsigned char index);
extern void we_clear(unsigned int x,unsigned int y,unsigned char index);

extern void s_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//s方向显示数字
extern void n_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//n方向显示数字

extern void ns_position(unsigned int x,unsigned int y,unsigned char index);
extern void ns_clear(unsigned int x,unsigned int y,unsigned char index);

extern void showxuexiao(unsigned int x,unsigned int y,unsigned char index);



//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


					  		 
#endif  
	 
	 



