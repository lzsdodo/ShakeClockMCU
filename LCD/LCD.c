#include <reg52.h>
#include <intrins.h>
#include <font\font.h>
#include <lcd\lcd.h>

void delayms(int count)  // /* X1ms */
{
        int i,j;
        for(i=0;i<count;i++)
                for(j=0;j<1000;j++);
}

unsigned char bdata bitdata;
sbit bit7=bitdata^7;
sbit bit6=bitdata^6;
sbit bit5=bitdata^5;
sbit bit4=bitdata^4;
sbit bit3=bitdata^3;
sbit bit2=bitdata^2;
sbit bit1=bitdata^1;
sbit bit0=bitdata^0;

unsigned int BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ	  
void LCD_Writ_Bus(char da)   //��������д��
{	
	bitdata=da;
	LCD_SDA=bit7;LCD_SCK=0;LCD_SCK=1;
	LCD_SDA=bit6;LCD_SCK=0;LCD_SCK=1;
	LCD_SDA=bit5;LCD_SCK=0;LCD_SCK=1;
	LCD_SDA=bit4;LCD_SCK=0;LCD_SCK=1;
	LCD_SDA=bit3;LCD_SCK=0;LCD_SCK=1;
	LCD_SDA=bit2;LCD_SCK=0;LCD_SCK=1;
	LCD_SDA=bit1;LCD_SCK=0;LCD_SCK=1;
	LCD_SDA=bit0;LCD_SCK=0;LCD_SCK=1; 
} 
void LCD_WR_DATA8(char da) //��������-8λ����
{
    LCD_AO=1;
	LCD_Writ_Bus(da);
}  
 void LCD_WR_DATA(int da)
{
    LCD_AO=1;
	LCD_Writ_Bus(da>>8);
	LCD_Writ_Bus(da);
}	  
void LCD_WR_REG(char da)	 
{
    LCD_AO=0;
	LCD_Writ_Bus(da);
}
 void LCD_WR_REG_DATA(int reg,int da)
{
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);

}
void Lcd_Init(void)
{

//����һ����Щ��������ñ����ʱ����ʾ����
   	LCD_CS =1;
	if(LCD_CS==0)
	{
	   LCD_WR_REG_DATA(0,0);
 	 }

    LCD_REST=1;
    delayms(5);	
	LCD_REST=0;
	delayms(5);
	LCD_REST=1;
	LCD_CS=1; 
	delayms(5);
	LCD_CS =0;  //��Ƭѡʹ��

	LCD_WR_REG(0x11); //Sleep out
	delayms(120); //Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB4); //Dot inversion
	LCD_WR_DATA8(0x03);
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0XC0);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA8(0x1A);
	LCD_WR_REG(0x36); //MX, MY, RGB mode
	LCD_WR_DATA8(0xC0);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0x3A); //65k mode
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x29); //Display on

}
//��������
//Color:Ҫ���������ɫ
void LCD_Clear(unsigned int Color)
{
	unsigned char VH,VL;
	unsigned int i,j;
	VH=Color>>8;
	VL=Color;	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	 LCD_WR_DATA8(VH);
			 LCD_WR_DATA8(VL);	
	    }
	  }
}

 //***************************************���Գ���******************************************************************************
//��ָ��λ����ʾһ������(32*33��С)
//dcolorΪ������ɫ��gbcolorΪ������ɫ
void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;  
	POINT_COLOR=WHITE;  
    Address_set(x,y,x+31,y+31); //��������      
	temp+=index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}

//***************��������
void w_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=w_num;
	POINT_COLOR=WHITE;	 
	Address_set(x,y,x+31,y+15); //��������
	temp+=index*64;  
	for(j=0;j<64;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}
void we_position(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=xiugai;
	POINT_COLOR=WHITE;	 
	Address_set(x,y,x+3,y+15); //��������
	temp+=index*8;  
	for(j=0;j<8;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}
void we_clear(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=xiugai;
	POINT_COLOR=WHITE;	 
	Address_set(x,y,x+3,y+127); //��������
	temp+=index*64;  
	for(j=0;j<64;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}

void e_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=e_num;
	POINT_COLOR=WHITE;
	Address_set(x,y,x+31,y+15); //��������
	temp+=index*64;  
	for(j=0;j<64;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}

void s_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=s_num;
	POINT_COLOR=WHITE;
	Address_set(x,y,x+15,y+31); //��������
	temp+=index*64;  
	for(j=0;j<64;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}

void n_LCD_ShowNum(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=n_num;
	POINT_COLOR=WHITE;
	Address_set(x,y,x+15,y+31); //��������
	temp+=index*64;  
	for(j=0;j<64;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}

void ns_position(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=xiugai;
	POINT_COLOR=WHITE;	 
	Address_set(x,y,x+15,y+3); //��������
	temp+=index*8;  
	for(j=0;j<8;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}
void ns_clear(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=xiugai;
	POINT_COLOR=WHITE;	 
	Address_set(x,y,x+79,y+3); //��������
	temp+=index*40;  
	for(j=0;j<40;j++)
	{
		for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
	}	
}



void showxuexiao(unsigned int x,unsigned int y,unsigned char index)
{
	unsigned char i,j;
	unsigned char *temp=xuexiao;   
	POINT_COLOR=GRAY; 
    Address_set(x,y,x+15,y+15); //��������      
	temp+=index*32;	
	for(j=0;j<32;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
	
}
//***************************************���Գ���******************************************************************************







