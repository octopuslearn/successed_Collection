#ifndef _LCD_H
#define _LCD_H

typedef unsigned char uchar;
typedef unsigned int uint;

#define IO P0
sbit RW=P3^0;
sbit RS=P3^1;
sbit EN=P3^2;

void Lcd1602_Write_com(uchar com);
void Lcd1602_Write_dat(uchar dat);
void Lcd_Init();



#endif