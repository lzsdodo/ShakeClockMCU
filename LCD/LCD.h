#ifndef __LCD_H
#define __LCD_H		

/* 
����Ĭ��IO���ӷ�ʽ��
sbit LCD_CS     =P0^0;     //Ƭѡ	
sbit LCD_AO     =P0^2;	  //����/�����л�
sbit LCD_SDA    =P1^5;	  //����
sbit LCD_SCK    =P1^7;	  //ʱ��
sbit LCD_REST   =P0^1;	  //��λ  
*/ 
 //����LCD�ĳߴ�	
#define LCD_W 128
#define LCD_H 160

//IO����  
sbit LCD_CS     =P2^0;     //Ƭѡ	
sbit LCD_AO     =P2^2;	  //����/�����л�
sbit LCD_SDA    =P2^3;	  //����
sbit LCD_SCK    =P2^4;	  //ʱ��
sbit LCD_REST   =P2^1;	  //��λ  

extern unsigned int BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

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

//������Գ���
extern void w_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//w������ʾ����
extern void e_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//e������ʾ����

extern void we_position(unsigned int x,unsigned int y,unsigned char index);
extern void we_clear(unsigned int x,unsigned int y,unsigned char index);

extern void s_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//s������ʾ����
extern void n_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index);//n������ʾ����

extern void ns_position(unsigned int x,unsigned int y,unsigned char index);
extern void ns_clear(unsigned int x,unsigned int y,unsigned char index);

extern void showxuexiao(unsigned int x,unsigned int y,unsigned char index);



//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


					  		 
#endif  
	 
	 



