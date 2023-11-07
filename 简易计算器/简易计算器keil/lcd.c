#include "reg51.h"
#include "lcd.h"
#include "intrins.h"

void delay1ms(void)   //Îó²î 0us
{
    unsigned char a,b;
    for(b=199;b>0;b--)
        for(a=1;a>0;a--);
}
void delay5ms(void)   //Îó²î 0us
{
    unsigned char a,b;
    for(b=185;b>0;b--)
        for(a=12;a>0;a--);
}

void Lcd1602_Write_com(uchar com)
{
	RW=0;
	RS=0;
	_nop_();
	EN=0;
	IO=com;	
	delay1ms();
	EN=1;
	delay5ms();
	EN=0;
}

void Lcd1602_Write_dat(uchar dat)
{
	RW=0;
	RS=1;
	_nop_();
	EN=0;
	IO=dat;	
	delay1ms();
	EN=1;
	delay5ms();
	EN=0;	
}

void Lcd_Init()
{
	Lcd1602_Write_com(0x38);
	Lcd1602_Write_com(0x0c);
	Lcd1602_Write_com(0x06);
	Lcd1602_Write_com(0x01);
	Lcd1602_Write_com(0x80);
}