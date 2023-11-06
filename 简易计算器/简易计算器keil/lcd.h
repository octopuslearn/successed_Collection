#ifndef _LCD_H
#define _LCD_H

typedef unsigned char uchar;
typedef unsigned int uint;

#define IO P0
sbit RW=P2^5;
sbit RS=P2^6;
sbit EN=P2^7;

void Lcd1602_Write_com(uchar com);
void Lcd1602_Write_dat(uchar dat);
void Lcd_Init();



#endif