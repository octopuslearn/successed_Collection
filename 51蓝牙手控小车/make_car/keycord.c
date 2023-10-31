/******************************/
/*目的，使用一个按键控制小车的五种运动方式*/
#include "keycord.h"

bit keybackup=1;//按键存储值
bit keydate=0;//按键当前值

void Delay100ms();
void carmode(unsigned char mode);

unsigned char cnt=0;//按键按下次数

/*按键函数*/
void keyjudgement()
{	
	keyout5=0;//将键盘输出值设置为低电平
	while(1)
	{
		keydate=keyin4;//将键盘输入值赋值给键盘当前值
		if(keydate!=keybackup)
		{
			Delay100ms();//延时100ms
			if(keydate==keyin4)//倘若经过延时后按键值仍=当前值
			{
				if(keybackup==1)//倘若上次为1，而当前变为了0，那么视为按下
				{
					cnt++;
					carmode(cnt);
					if(cnt>4)
					{
						cnt=0;
					}
				}
				keybackup=keydate;//更新当前值为备份
			}
		}
	}
}

void Delay100ms()		//@11.0592MHz,延时100ms
{
	unsigned char i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

/*小车运动模式函数，mode-运动模式*/
void carmode(unsigned char cnt)
{
	switch(cnt)
	{
		case 1:go();break;//*前进*/
		case 2:back();break;//*后退*/
		case 3:turnleft();break;//*左转*/
		case 4:turnright();break;//*右转*/
		case 5:stop();break;//*停止*/
		default:break;
	}
}