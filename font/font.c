#include <font\font.h>

//0ʱ1��2��3��4��5��6ҡ7һ	 32*32	128
unsigned char code hanzi[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xE0,0x01,
0x80,0xFF,0xFF,0x01,0x80,0xFF,0xFF,0x01,0x00,0x02,0x02,0x01,0x00,0x02,0x02,0x01,
0x00,0x02,0x02,0x03,0x80,0xFF,0xFF,0x03,0x80,0xFF,0xFF,0x03,0x00,0x00,0xC0,0x01,
0x00,0x00,0x08,0x00,0x00,0x80,0x08,0x00,0x00,0xC0,0x08,0x00,0x00,0xE0,0x08,0x00,
0x00,0xE0,0x08,0x00,0x40,0x60,0x18,0x00,0x60,0x00,0x18,0x00,0x78,0x00,0x18,0x30,
0xF8,0xFF,0xFF,0x3F,0xF8,0xFF,0xFF,0x3F,0xF0,0xFF,0xFF,0x1F,0x00,0x00,0x10,0x00,
0x00,0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x30,0x00,
0x00,0x00,0x30,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"ʱ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x07,0x00,0x02,0xC0,0xFF,0xFF,0x03,0xC0,0xFF,0xFF,0x01,0x00,0xE0,0xFF,0x31,
0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x1C,0x00,0x10,0x38,0x1C,0x00,0xFC,0x3F,0x0C,
0x00,0xFC,0x3F,0x00,0x00,0x10,0x23,0x04,0x00,0x10,0x23,0x04,0x00,0x10,0x23,0x04,
0x00,0x10,0x62,0x04,0x00,0x78,0x7F,0x04,0x00,0xFC,0x7F,0x0C,0x00,0xFC,0x7F,0x0C,
0x00,0x00,0x20,0x08,0x40,0x00,0x00,0x08,0x60,0x00,0x00,0x18,0xF8,0x00,0x00,0x1F,
0xF8,0xFF,0xFF,0x1F,0xF8,0xFF,0xFF,0x0F,0xE0,0xFF,0x00,0x0E,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",1*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x40,0x00,0x00,0x00,0xC0,0x8F,0xFF,0x00,0xE0,0xFF,0xFF,0x07,0xE0,0xFF,0xFF,0x07,
0xE0,0x03,0xE0,0x07,0x60,0x00,0xC0,0x00,0x60,0x40,0x60,0x00,0x40,0x60,0x60,0x00,
0x40,0x60,0x40,0x00,0x40,0x60,0x40,0x00,0x40,0x40,0x40,0x00,0xC0,0x40,0x40,0x00,
0x80,0x40,0x40,0x00,0x80,0xFF,0xFF,0x00,0xC0,0xFF,0xFF,0x01,0xC0,0x0F,0xFC,0x01,
0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0xF8,0x07,
0xC0,0xFF,0xFF,0x1F,0xC0,0xFF,0xFF,0x3F,0xC0,0x21,0x04,0x3E,0x40,0x30,0x04,0x0C,
0xC0,0x30,0x04,0x04,0x80,0x20,0x04,0x00,0x80,0xFF,0x0F,0x00,0x80,0xFF,0x7F,0x00,
0x80,0xC1,0xFF,0x01,0x80,0x00,0xE0,0x07,0xC0,0x00,0x04,0x0F,0xC0,0x00,0x04,0x1C,
0xFC,0x00,0xC4,0x19,0xFC,0xFF,0xE7,0x31,0xFC,0xFF,0xE7,0x20,0x84,0x49,0x24,0x00,
0x80,0xC9,0x0C,0x00,0x00,0xC9,0x2C,0x00,0xF0,0xFF,0x6F,0x00,0xF0,0xFF,0xEF,0x00,
0x30,0x03,0xE8,0x01,0x00,0x03,0x98,0x03,0x00,0x03,0x18,0x03,0x00,0x01,0x18,0x06,
0x00,0x00,0x18,0x04,0x00,0x00,0x18,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,/*"��",3*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,
0xE0,0x01,0x00,0x00,0xE0,0x03,0x00,0x00,0xE0,0x07,0x00,0x0C,0xE0,0x00,0x03,0x0E,
0xE0,0x00,0x03,0x0E,0xE0,0x00,0x03,0x0E,0xE0,0x38,0x07,0x0E,0xE0,0x78,0x67,0x06,
0xE0,0xF8,0x77,0x06,0xE0,0x38,0x77,0x06,0xE0,0x38,0x77,0x06,0xE0,0x38,0x77,0x06,
0xC0,0x38,0x7F,0x06,0xC0,0xFC,0x07,0x06,0xC0,0x30,0x07,0x06,0xC0,0x00,0x7F,0x06,
0xC0,0xE0,0x1F,0x06,0xC0,0xFE,0x00,0x06,0xC0,0x00,0x03,0x06,0xC0,0x00,0x07,0x07,
0xC0,0x01,0x0E,0x0E,0xE0,0x01,0x00,0x00,0xE0,0xF1,0x77,0x00,0xE0,0x1F,0xE0,0x00,
0x00,0x00,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",4*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0C,0x00,0x00,0x00,0x1C,0x80,0x01,0x00,0x3C,0x80,0x01,0x00,0x78,0xC0,0x03,
0x00,0x78,0xE0,0x01,0x00,0x0C,0xF0,0x01,0x00,0x0E,0xF8,0x01,0x00,0xC7,0xCF,0x01,
0x80,0x7F,0xC0,0x01,0x00,0x0F,0xC0,0x01,0x00,0x00,0xC0,0xDF,0x00,0x18,0xFC,0x63,
0x7E,0x38,0xFE,0x31,0xFC,0xF0,0xC3,0x19,0xF0,0x81,0xC3,0x1F,0xC0,0x83,0xF1,0x0D,
0x80,0xC7,0x01,0x0E,0x00,0xC6,0x00,0x07,0x00,0xEC,0x00,0x03,0x00,0x78,0xF8,0x03,
0x00,0x70,0xBC,0x01,0x00,0x70,0xC0,0x01,0x00,0x70,0xC0,0x01,0x00,0x78,0xE0,0x00,
0x00,0x78,0xE0,0x00,0x00,0x70,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",5*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,
0x00,0x0F,0xE0,0x00,0x00,0x06,0xF0,0x01,0x00,0x06,0x3C,0x00,0x00,0x06,0x0E,0x06,
0x00,0x86,0x03,0x0F,0x00,0x06,0x18,0x0F,0x00,0xC6,0xB8,0x03,0x00,0xDF,0xF9,0x01,
0xE0,0x87,0x63,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0xC3,0x03,0x00,0x9E,0xFB,0x01,
0x00,0x8E,0x1F,0x00,0x00,0xC7,0x39,0x00,0xC0,0xC7,0x38,0x38,0xF0,0x06,0xF8,0x7F,
0x7C,0xC6,0x3F,0x00,0x3C,0xF6,0x19,0x00,0x00,0x66,0x18,0x07,0x00,0xC7,0x18,0x0E,
0x00,0xC7,0x18,0x0E,0xC0,0xC7,0xFC,0x0F,0x80,0xE7,0x0F,0x0E,0x80,0xE7,0x01,0x0E,
0x00,0x63,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"ҡ",6*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x1F,0x00,0x80,0xFF,0x7F,
0xE0,0xFF,0xFF,0x7F,0xF8,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"һ",7*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x60,0x80,0x01,0x00,0xE0,0xC0,0xC0,0x00,0xC0,0xC1,0xF0,0x03,
0x80,0xC1,0xFF,0x03,0x00,0x60,0xE0,0x00,0x00,0x20,0x20,0x00,0x00,0x10,0x10,0x00,
0x00,0xCA,0x01,0x00,0x00,0xC1,0x00,0x00,0x80,0xC0,0x00,0x00,0xC0,0xC0,0x01,0x00,
0x60,0xC0,0x02,0x00,0x70,0x60,0x06,0x00,0x38,0x60,0x0C,0x00,0x3C,0x30,0x18,0x00,
0x18,0x30,0x38,0x00,0x18,0x18,0xF0,0x00,0x00,0x0C,0xE0,0x03,0x00,0x03,0xE0,0x3F,
0xC0,0x00,0x80,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /*"��",8*/
};

//0 4 8 12ҡ  1 5 9 13һ  2 6 10 14��  3 7 11 15��  w e s n	  16*16	32
unsigned char code xuexiao[]={
//w
0x00,0x00,0x20,0x00,0x20,0x02,0x40,0x02,0xFE,0x7F,0x80,0x04,0x64,0x00,0xBC,0x06,
0x24,0x09,0xF8,0x18,0x24,0x23,0x28,0x25,0x5C,0x08,0x48,0x08,0x40,0x00,0x00,0x00,/*"ҡ",0*/
0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,
0x80,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,/*"һ",1*/
0x00,0x00,0x00,0x00,0x08,0x00,0xF0,0x0F,0x00,0x22,0xE0,0x12,0x80,0x02,0xF8,0x0F,
0x80,0x12,0x20,0x13,0xE0,0x15,0x04,0x20,0xF6,0x3F,0x0C,0x00,0x00,0x00,0x00,0x00,/*"��",2*/
0x00,0x00,0x80,0x00,0x00,0x01,0x40,0x06,0x7C,0x79,0xC8,0x25,0x90,0x0A,0x40,0x02,
0x80,0x03,0x40,0x02,0xFF,0x3F,0x80,0x04,0x80,0x04,0x00,0x07,0x00,0x04,0x00,0x00,/*"��",3*/
//e
0x00,0x00,0x00,0x02,0x10,0x12,0x10,0x3A,0xA4,0x14,0xC4,0x24,0x18,0x1F,0x90,0x24,
0x60,0x3D,0x00,0x26,0x20,0x01,0xFE,0x7F,0x40,0x02,0x40,0x04,0x00,0x04,0x00,0x00,/*"ҡ",4*/
0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x00,0x01,
0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00,/*"һ",5*/
0x00,0x00,0x00,0x00,0x00,0x30,0xFC,0x6F,0x04,0x20,0xA8,0x07,0xC8,0x04,0x48,0x01,
0xF0,0x1F,0x40,0x01,0x48,0x07,0x44,0x00,0xF0,0x0F,0x00,0x10,0x00,0x00,0x00,0x00,/*"��",6*/
0x00,0x00,0x20,0x00,0xE0,0x00,0x20,0x01,0x20,0x01,0xFC,0xFF,0x40,0x02,0xC0,0x01,
0x40,0x02,0x50,0x09,0xA4,0x13,0x9E,0x3E,0x60,0x02,0x80,0x00,0x00,0x01,0x00,0x00,/*"��",7*/
//s
0x00,0x00,0x10,0x00,0x10,0x0C,0x10,0x02,0x10,0x33,0xB0,0x08,0x9C,0x04,0x10,0x0D,
0xB0,0x02,0x58,0x72,0xD6,0x0F,0x90,0x12,0x90,0x3A,0xD0,0x15,0x10,0x00,0x00,0x00,/*"ҡ",8*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,
0xFE,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"һ",9*/
0x00,0x00,0x00,0x00,0x10,0x18,0x20,0x17,0x88,0x10,0x88,0x14,0xF8,0x13,0x88,0x16,
0xE8,0x15,0xA8,0x14,0xA8,0x16,0x88,0x10,0x84,0x20,0x00,0x38,0x00,0x10,0x00,0x00,/*"��",10*/
0x00,0x00,0x10,0x00,0x30,0x04,0x10,0x04,0x50,0x04,0x28,0x7C,0xC8,0x27,0x34,0x25,
0x62,0x1D,0xB8,0x06,0x10,0x04,0x50,0x04,0x30,0x04,0x10,0x04,0x00,0x04,0x00,0x04,/*"��",11*/
//n
0x00,0x00,0x00,0x08,0xA8,0x0B,0x5C,0x09,0x48,0x09,0xF0,0x6B,0x4E,0x1A,0x40,0x0D,
0xB0,0x08,0x20,0x39,0x10,0x0D,0xCC,0x08,0x40,0x08,0x30,0x08,0x00,0x08,0x00,0x00,/*"ҡ",12*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x7F,
0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"һ",13*/
0x00,0x00,0x08,0x00,0x1C,0x00,0x04,0x21,0x08,0x11,0x68,0x15,0x28,0x15,0xA8,0x17,
0x68,0x11,0xC8,0x1F,0x28,0x11,0x08,0x11,0xE8,0x04,0x18,0x08,0x00,0x00,0x00,0x00,/*"��",14*/
0x20,0x00,0x20,0x00,0x20,0x08,0x20,0x0C,0x20,0x0A,0x20,0x08,0x60,0x1D,0xB8,0x46,
0xA4,0x2C,0xE4,0x13,0x3E,0x14,0x20,0x0A,0x20,0x08,0x20,0x0C,0x00,0x08,0x00,0x00/*"��",15*/
};


//16x32	64
unsigned char code w_num[]={
0x00,0x00,0x00,0x00,0x00,0xF8,0x3F,0x00,0x00,0xFF,0xFF,0x01,0x80,0xFF,0xFF,0x03,
0xE0,0xFF,0xFF,0x07,0xF0,0x07,0xC0,0x0F,0xF0,0x00,0x00,0x0E,0x70,0x00,0x00,0x1C,
0x70,0x00,0x00,0x1C,0x70,0x00,0x00,0x1E,0xF0,0x01,0x00,0x0F,0xE0,0x0F,0xE0,0x0F,
0xC0,0xFF,0xFF,0x07,0x80,0xFF,0xFF,0x03,0x00,0xFE,0xFF,0x00,0x00,0xE0,0x0F,0x00,/*"0",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,
0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x03,0xF0,0xFF,0xFF,0x07,
0xF0,0xFF,0xFF,0x1F,0xF0,0xFF,0xFF,0x1F,0xF0,0xFF,0xFF,0x1F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",1*/
0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0xF0,0x00,0xE0,0x00,0xF0,0x03,0xE0,0x03,
0xF0,0x07,0xE0,0x07,0xF0,0x1F,0xE0,0x0F,0x70,0x3F,0x00,0x0E,0x70,0x7C,0x00,0x1C,
0x70,0xF8,0x00,0x1C,0x70,0xF0,0x03,0x1C,0x70,0xE0,0x0F,0x0E,0x70,0x80,0xFF,0x0F,
0x70,0x00,0xFF,0x0F,0x70,0x00,0xFE,0x07,0x70,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,/*"2",2*/
0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x80,0x0F,0xC0,0x01,0xC0,0x0F,0xC0,0x07,
0xE0,0x0F,0xC0,0x0F,0xF0,0x01,0xC0,0x0F,0x70,0x00,0x00,0x0E,0x70,0x00,0x03,0x1C,
0x70,0x00,0x03,0x1C,0x70,0x80,0x07,0x1C,0xF0,0xC0,0x0F,0x0F,0xF0,0xFF,0xFF,0x0F,
0xE0,0xFF,0xFC,0x07,0xC0,0x7F,0xF8,0x03,0x00,0x3F,0xE0,0x00,0x00,0x00,0x00,0x00,/*"3",3*/
0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0xFE,0x01,0x00,
0x00,0xFE,0x07,0x00,0x00,0xCE,0x1F,0x00,0x00,0x0E,0x7F,0x00,0x00,0x0E,0xFC,0x01,
0x00,0x0E,0xF0,0x07,0xF0,0xFF,0xFF,0x1F,0xF0,0xFF,0xFF,0x1F,0xF0,0xFF,0xFF,0x1F,
0xF0,0xFF,0xFF,0x1F,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,/*"4",4*/
0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x80,0x87,0xFF,0x1F,0xC0,0x87,0xFF,0x1F,
0xE0,0x87,0xFF,0x1F,0xF0,0x81,0xFF,0x1F,0x70,0x00,0x06,0x1C,0x70,0x00,0x0E,0x1C,
0x70,0x00,0x0C,0x1C,0x70,0x00,0x0C,0x1C,0xF0,0x01,0x0F,0x1C,0xE0,0xFF,0x0F,0x1C,
0xC0,0xFF,0x07,0x1C,0x80,0xFF,0x03,0x1C,0x00,0xFE,0x00,0x1C,0x00,0x00,0x00,0x00,/*"5",5*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x3F,0x00,0x80,0xFF,0xFF,0x01,
0xC0,0xFF,0xFF,0x03,0xE0,0xFF,0xFF,0x07,0xF0,0x01,0xC7,0x0F,0x70,0x00,0x0E,0x0E,
0x70,0x00,0x0C,0x1C,0x70,0x00,0x0E,0x1C,0xF0,0x00,0x0E,0x1E,0xF0,0xFF,0x8F,0x0F,
0xE0,0xFF,0x87,0x0F,0xC0,0xFF,0x83,0x07,0x00,0xFF,0x81,0x01,0x00,0x00,0x00,0x00,/*"6",6*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x1C,
0x00,0x00,0x00,0x1C,0x10,0x00,0x00,0x1C,0xF0,0x01,0x00,0x1C,0xF0,0x0F,0x00,0x1C,
0xF0,0xFF,0x00,0x1C,0xC0,0xFF,0x07,0x1C,0x00,0xFE,0x7F,0x1C,0x00,0xE0,0xFF,0x1F,
0x00,0x00,0xFE,0x1F,0x00,0x00,0xE0,0x1F,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x00,/*"7",7*/
0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x80,0x3F,0xF0,0x01,0xC0,0xFF,0xFC,0x07,
0xE0,0xFF,0xFE,0x0F,0xF0,0xE1,0xFF,0x0F,0xF0,0x80,0x07,0x0E,0x70,0x80,0x03,0x1C,
0x70,0x00,0x03,0x1C,0x70,0x80,0x03,0x1C,0xF0,0xC0,0x07,0x0E,0xF0,0xFF,0xFF,0x0F,
0xE0,0xFF,0xFE,0x07,0xC0,0x7F,0xFC,0x03,0x80,0x3F,0xF0,0x00,0x00,0x00,0x00,0x00,/*"8",8*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xFF,0x01,0xC0,0xC3,0xFF,0x07,
0xE0,0xE3,0xFF,0x0F,0xF0,0xE3,0xFF,0x0F,0xF0,0xF0,0x00,0x1E,0x70,0x70,0x00,0x1C,
0x70,0x70,0x00,0x1C,0xF0,0xE0,0x00,0x1E,0xF0,0xE7,0x01,0x0F,0xE0,0xFF,0xFF,0x0F,
0x80,0xFF,0xFF,0x07,0x00,0xFF,0xFF,0x01,0x00,0xF8,0x7F,0x00,0x00,0x00,0x00,0x00,/*"9",9*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xE0,0x00,0x80,0x03,0xE0,0x00,
0x80,0x03,0xE0,0x00,0x80,0x03,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*":",10*/
0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,/*"-",11*/
0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,
0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0xFE,0xFF,0x00,0x00,0xFE,0xFF,0x00,
0x00,0xFE,0xFF,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,
0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00 /*"+",12*/
};


unsigned char code e_num[]={
0x00,0xF0,0x07,0x00,0x00,0xFF,0x7F,0x00,0xC0,0xFF,0xFF,0x01,0xE0,0xFF,0xFF,0x03,
0xF0,0x07,0xF0,0x07,0xF0,0x00,0x80,0x0F,0x78,0x00,0x00,0x0E,0x38,0x00,0x00,0x0E,
0x38,0x00,0x00,0x0E,0x70,0x00,0x00,0x0F,0xF0,0x03,0xE0,0x0F,0xE0,0xFF,0xFF,0x07,
0xC0,0xFF,0xFF,0x01,0x80,0xFF,0xFF,0x00,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,/*"0",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x0F,0xF8,0xFF,0xFF,0x0F,0xF8,0xFF,0xFF,0x0F,
0xE0,0xFF,0xFF,0x0F,0xC0,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,
0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",1*/
0x00,0x00,0x00,0x00,0x80,0x0F,0x00,0x0E,0xE0,0x7F,0x00,0x0E,0xF0,0xFF,0x00,0x0E,
0xF0,0xFF,0x01,0x0E,0x70,0xF0,0x07,0x0E,0x38,0xC0,0x0F,0x0E,0x38,0x00,0x1F,0x0E,
0x38,0x00,0x3E,0x0E,0x70,0x00,0xFC,0x0E,0xF0,0x07,0xF8,0x0F,0xE0,0x07,0xE0,0x0F,
0xC0,0x07,0xC0,0x0F,0x00,0x07,0x00,0x0F,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,/*"2",2*/
0x00,0x00,0x00,0x00,0x00,0x07,0xFC,0x00,0xC0,0x1F,0xFE,0x03,0xE0,0x3F,0xFF,0x07,
0xF0,0xFF,0xFF,0x0F,0xF0,0xF0,0x03,0x0F,0x38,0xE0,0x01,0x0E,0x38,0xC0,0x00,0x0E,
0x38,0xC0,0x00,0x0E,0x70,0x00,0x00,0x0E,0xF0,0x03,0x80,0x0F,0xF0,0x03,0xF0,0x07,
0xE0,0x03,0xF0,0x03,0x80,0x03,0xF0,0x01,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,/*"3",3*/
0x00,0x00,0x70,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x70,0x00,0xF8,0xFF,0xFF,0x0F,
0xF8,0xFF,0xFF,0x0F,0xF8,0xFF,0xFF,0x0F,0xF8,0xFF,0xFF,0x0F,0xE0,0x0F,0x70,0x00,
0x80,0x3F,0x70,0x00,0x00,0xFE,0x70,0x00,0x00,0xF8,0x73,0x00,0x00,0xE0,0x7F,0x00,
0x00,0x80,0x7F,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,/*"4",4*/
0x00,0x00,0x00,0x00,0x38,0x00,0x7F,0x00,0x38,0xC0,0xFF,0x01,0x38,0xE0,0xFF,0x03,
0x38,0xF0,0xFF,0x07,0x38,0xF0,0x80,0x0F,0x38,0x30,0x00,0x0E,0x38,0x30,0x00,0x0E,
0x38,0x70,0x00,0x0E,0x38,0x60,0x00,0x0E,0xF8,0xFF,0x81,0x0F,0xF8,0xFF,0xE1,0x07,
0xF8,0xFF,0xE1,0x03,0xF8,0xFF,0xE1,0x01,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,/*"5",5*/
0x00,0x00,0x00,0x00,0x80,0x81,0xFF,0x00,0xE0,0xC1,0xFF,0x03,0xF0,0xE1,0xFF,0x07,
0xF0,0xF1,0xFF,0x0F,0x78,0x70,0x00,0x0F,0x38,0x70,0x00,0x0E,0x38,0x30,0x00,0x0E,
0x70,0x70,0x00,0x0E,0xF0,0xE3,0x80,0x0F,0xE0,0xFF,0xFF,0x07,0xC0,0xFF,0xFF,0x03,
0x80,0xFF,0xFF,0x01,0x00,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"6",6*/
0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0xF8,0x07,0x00,0x00,0xF8,0x7F,0x00,0x00,
0xF8,0xFF,0x07,0x00,0x38,0xFE,0x7F,0x00,0x38,0xE0,0xFF,0x03,0x38,0x00,0xFF,0x0F,
0x38,0x00,0xF0,0x0F,0x38,0x00,0x80,0x0F,0x38,0x00,0x00,0x08,0x38,0x00,0x00,0x00,
0x38,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"7",7*/
0x00,0x00,0x00,0x00,0x00,0x0F,0xFC,0x01,0xC0,0x3F,0xFE,0x03,0xE0,0x7F,0xFF,0x07,
0xF0,0xFF,0xFF,0x0F,0x70,0xE0,0x03,0x0F,0x38,0xC0,0x01,0x0E,0x38,0xC0,0x00,0x0E,
0x38,0xC0,0x01,0x0E,0x70,0xE0,0x01,0x0F,0xF0,0xFF,0x87,0x0F,0xF0,0x7F,0xFF,0x07,
0xE0,0x3F,0xFF,0x03,0x80,0x0F,0xFC,0x01,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,/*"8",8*/
0x00,0x00,0x00,0x00,0x00,0xFE,0x1F,0x00,0x80,0xFF,0xFF,0x00,0xE0,0xFF,0xFF,0x01,
0xF0,0xFF,0xFF,0x07,0xF0,0x80,0xE7,0x0F,0x78,0x00,0x07,0x0F,0x38,0x00,0x0E,0x0E,
0x38,0x00,0x0E,0x0E,0x78,0x00,0x0F,0x0F,0xF0,0xFF,0xC7,0x0F,0xF0,0xFF,0xC7,0x07,
0xE0,0xFF,0xC3,0x03,0x80,0xFF,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"9",9*/
0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,
0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00 /*"-",10*/
};

unsigned char code s_num[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xE0,0x0F,0xF0,0x1F,0x78,0x3E,0x3C,0x3C,
0x3C,0x78,0x3C,0x78,0x1E,0x78,0x1E,0x70,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,
0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0x70,0x1E,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x3C,
0x78,0x3C,0xF0,0x1F,0xF0,0x0F,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"0",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x80,0x07,0xC0,0x07,0xF8,0x07,
0xF8,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,
0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,
0x80,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",1*/
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xE0,0x1F,0xF0,0x3F,0x78,0x3C,0x38,0x78,
0x3C,0x78,0x3C,0x78,0x3C,0x78,0x00,0x78,0x00,0x3C,0x00,0x3C,0x00,0x3E,0x00,0x1E,
0x00,0x0F,0x80,0x07,0xC0,0x07,0xE0,0x03,0xE0,0x01,0xF0,0x00,0x78,0x00,0x78,0x00,
0x3C,0x00,0xFC,0x7F,0xFE,0x7F,0xFE,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"2",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xF0,0x0F,0xF8,0x1F,0x78,0x3C,0x3C,0x3C,
0x3C,0x78,0x3C,0x78,0x00,0x78,0x00,0x38,0x00,0x3C,0x00,0x1E,0x80,0x0F,0x80,0x0F,
0x00,0x1E,0x00,0x3C,0x00,0x78,0x00,0x78,0x1E,0x78,0x1E,0x78,0x1C,0x78,0x3C,0x78,
0x3C,0x3C,0xF8,0x3F,0xF0,0x1F,0xE0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"3",3*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x1E,0x00,0x1F,0x00,0x1F,0x80,0x1F,
0x80,0x1F,0xC0,0x1F,0xC0,0x1F,0xE0,0x1F,0xE0,0x1E,0xF0,0x1E,0x70,0x1E,0x78,0x1E,
0x38,0x1E,0x3C,0x1E,0x1C,0x1E,0x1E,0x1E,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0x00,0x1E,
0x00,0x1E,0x00,0x1E,0x00,0x1E,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"4",4*/
0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7F,0xFC,0x7F,0xFC,0x7F,0x3C,0x00,0x3C,0x00,
0x3C,0x00,0x3C,0x00,0x3C,0x00,0x3C,0x00,0xBC,0x0F,0xFC,0x1F,0xFC,0x3C,0x3C,0x3C,
0x3C,0x78,0x00,0x78,0x00,0x78,0x00,0x78,0x00,0x78,0x1E,0x78,0x1C,0x78,0x3C,0x3C,
0x3C,0x3C,0xF8,0x1F,0xF0,0x0F,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"5",5*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x1F,0xE0,0x3F,0xF0,0x3C,0x78,0x78,
0x78,0x78,0x78,0x00,0x3C,0x00,0x3C,0x00,0xBC,0x0F,0xFC,0x1F,0xFC,0x3E,0x7C,0x78,
0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x38,0x78,0x78,0x78,
0x78,0x3C,0xF0,0x3F,0xE0,0x1F,0xC0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"6",6*/
0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7F,0xFC,0x7F,0xFC,0x7F,0x00,0x78,0x00,0x38,
0x00,0x38,0x00,0x3C,0x00,0x3C,0x00,0x1C,0x00,0x1C,0x00,0x1E,0x00,0x1E,0x00,0x0E,
0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x07,0x80,0x07,0x80,0x07,0x80,0x07,0xC0,0x03,
0xC0,0x03,0xC0,0x03,0xC0,0x01,0xE0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"7",7*/
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xF0,0x0F,0xF8,0x1F,0x78,0x3C,0x3C,0x38,
0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x38,0x38,0x78,0x3C,0xF0,0x1F,0xE0,0x0F,
0xF8,0x1E,0x38,0x3C,0x3C,0x78,0x1C,0x78,0x1E,0x78,0x1E,0x78,0x1C,0x78,0x3C,0x78,
0x7C,0x7C,0xF8,0x3F,0xF0,0x1F,0xE0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"8",8*/
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0xF0,0x0F,0xF8,0x1F,0x78,0x1E,0x3C,0x3C,
0x3C,0x38,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x78,0x3C,0x7C,
0x78,0x7E,0xF8,0x7F,0xF0,0x7F,0xC0,0x79,0x00,0x78,0x00,0x3C,0x3C,0x3C,0x3C,0x3C,
0x7C,0x1E,0xF8,0x0F,0xF0,0x0F,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"9",9*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0xEE,0x1C,0x9C,0x33,0x8C,0x31,
0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,
0x8C,0x31,0x8C,0x31,0xDE,0x7B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"m",10*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0x80,0x03,
0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x01,0x80,0x01,0x80,0x01,
0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
0x80,0x01,0x80,0x01,0xF8,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"i",11*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x1E,0x0F,0xD8,0x18,0x38,0x30,
0x18,0x30,0x18,0x30,0x18,0x30,0x18,0x30,0x18,0x30,0x18,0x30,0x18,0x30,0x18,0x30,
0x18,0x30,0x18,0x30,0x7E,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /*"n",12*/
};

unsigned char code n_num[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x07,0xF0,0x0F,0xF8,0x0F,0x3C,0x1E,
0x3C,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x78,0x0E,0x78,0x0F,0x78,0x0F,0x78,0x0F,0x78,
0x0F,0x78,0x0F,0x78,0x0F,0x78,0x0F,0x78,0x0E,0x78,0x1E,0x78,0x1E,0x3C,0x1E,0x3C,
0x3C,0x3C,0x7C,0x1E,0xF8,0x0F,0xF0,0x07,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,/*"0",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,
0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,
0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x1F,
0xE0,0x1F,0xE0,0x03,0xE0,0x01,0xE0,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",1*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0xFE,0x7F,0xFE,0x3F,0x00,0x3C,
0x00,0x1E,0x00,0x1E,0x00,0x0F,0x80,0x07,0xC0,0x07,0xE0,0x03,0xE0,0x01,0xF0,0x00,
0x78,0x00,0x7C,0x00,0x3C,0x00,0x3C,0x00,0x1E,0x00,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,
0x1E,0x1C,0x3C,0x1E,0xFC,0x0F,0xF8,0x07,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,/*"2",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x07,0xF8,0x0F,0xFC,0x1F,0x3C,0x3C,
0x1E,0x3C,0x1E,0x38,0x1E,0x78,0x1E,0x78,0x1E,0x00,0x1E,0x00,0x3C,0x00,0x78,0x00,
0xF0,0x01,0xF0,0x01,0x78,0x00,0x3C,0x00,0x1C,0x00,0x1E,0x00,0x1E,0x3C,0x1E,0x3C,
0x3C,0x3C,0x3C,0x1E,0xF8,0x1F,0xF0,0x0F,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,/*"3",3*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x78,0x00,0x78,0x00,0x78,0x00,
0x78,0x00,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0x78,0x78,0x78,0x38,0x78,0x3C,0x78,0x1C,
0x78,0x1E,0x78,0x0E,0x78,0x0F,0x78,0x07,0xF8,0x07,0xF8,0x03,0xF8,0x03,0xF8,0x01,
0xF8,0x01,0xF8,0x00,0xF8,0x00,0x78,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"4",4*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x07,0xF0,0x0F,0xF8,0x1F,0x3C,0x3C,
0x3C,0x3C,0x1E,0x38,0x1E,0x78,0x1E,0x00,0x1E,0x00,0x1E,0x00,0x1E,0x00,0x1E,0x3C,
0x3C,0x3C,0x3C,0x3F,0xF8,0x3F,0xF0,0x3D,0x00,0x3C,0x00,0x3C,0x00,0x3C,0x00,0x3C,
0x00,0x3C,0x00,0x3C,0xFE,0x3F,0xFE,0x3F,0xFE,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,/*"5",5*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x03,0xF8,0x07,0xFC,0x0F,0x3C,0x1E,
0x1E,0x1E,0x1E,0x1C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,
0x1E,0x3E,0x7C,0x3F,0xF8,0x3F,0xF0,0x3D,0x00,0x3C,0x00,0x3C,0x00,0x1E,0x1E,0x1E,
0x1E,0x1E,0x3C,0x0F,0xFC,0x07,0xF8,0x03,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"6",6*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x07,0x80,0x03,0xC0,0x03,0xC0,0x03,
0xC0,0x03,0xE0,0x01,0xE0,0x01,0xE0,0x01,0xE0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,
0x70,0x00,0x78,0x00,0x78,0x00,0x38,0x00,0x38,0x00,0x3C,0x00,0x3C,0x00,0x1C,0x00,
0x1C,0x00,0x1E,0x00,0xFE,0x3F,0xFE,0x3F,0xFE,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,/*"7",7*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x07,0xF8,0x0F,0xFC,0x1F,0x3E,0x3E,
0x1E,0x3C,0x1E,0x38,0x1E,0x78,0x1E,0x78,0x1E,0x38,0x1E,0x3C,0x3C,0x1C,0x78,0x1F,
0xF0,0x07,0xF8,0x0F,0x3C,0x1E,0x1C,0x1C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,
0x1C,0x3C,0x3C,0x1E,0xF8,0x1F,0xF0,0x0F,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,/*"8",8*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x07,0xF0,0x0F,0xF0,0x1F,0x78,0x3E,
0x3C,0x3C,0x3C,0x3C,0x3C,0x00,0x1E,0x00,0x9E,0x03,0xFE,0x0F,0xFE,0x1F,0x7E,0x1E,
0x3E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1E,0x3C,0x1C,0x3C,
0x3C,0x3C,0x78,0x1E,0xF8,0x1F,0xF0,0x0F,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,/*"9",9*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,
0xC0,0x03,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0xC0,0x03,0xC0,0x03,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /*":",10*/
};

unsigned char code xiugai[]={
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,/*"_",0*/
//w�޸�ָʾλ,16*4
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",1*/
//e�޸�ָʾλ,16*4
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,/*"_",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",3*/
//n�޸�ָʾλ,4*16
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,/*"_",3*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",4*/
//s�޸�ָʾλ,4*16
0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,/*"_",5*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",6*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /*"clear",2,3*/
};