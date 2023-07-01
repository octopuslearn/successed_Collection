#include "JDY-31.h"

unsigned char JDY31_mode;

void UartInit()		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8???,?????
	TMOD &= 0x0F;		//???????
	TMOD |= 0x20;		//???????
	TL1 = 0xFD;			//???????
	TH1 = 0xFD;			//???????
	ET1 = 0;			//???????
	TR1 = 1;			//???1????
	ES=1;
}

void JDY31_interrupt() interrupt 4
{
	if(RI)
	{
		RI=0;
		JDY31_mode=SBUF;
	}
}

void JDY31_judgement()
{
	switch(JDY31_mode)
	{
		case '1':go();break;
		case '2':back();break;
		case '3':turnleft();break;
		case '4':turnright();break;
		case '5':stop();break;
	}
}