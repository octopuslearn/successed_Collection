/******************************/
/*Ŀ�ģ�ʹ��һ����������С���������˶���ʽ*/
#include "keycord.h"

bit keybackup=1;//�����洢ֵ
bit keydate=0;//������ǰֵ

void Delay100ms();
void carmode(unsigned char mode);

unsigned char cnt=0;//�������´���

/*��������*/
void keyjudgement()
{	
	keyout5=0;//���������ֵ����Ϊ�͵�ƽ
	while(1)
	{
		keydate=keyin4;//����������ֵ��ֵ�����̵�ǰֵ
		if(keydate!=keybackup)
		{
			Delay100ms();//��ʱ100ms
			if(keydate==keyin4)//����������ʱ�󰴼�ֵ��=��ǰֵ
			{
				if(keybackup==1)//�����ϴ�Ϊ1������ǰ��Ϊ��0����ô��Ϊ����
				{
					cnt++;
					carmode(cnt);
					if(cnt>4)
					{
						cnt=0;
					}
				}
				keybackup=keydate;//���µ�ǰֵΪ����
			}
		}
	}
}

void Delay100ms()		//@11.0592MHz,��ʱ100ms
{
	unsigned char i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

/*С���˶�ģʽ������mode-�˶�ģʽ*/
void carmode(unsigned char cnt)
{
	switch(cnt)
	{
		case 1:go();break;//*ǰ��*/
		case 2:back();break;//*����*/
		case 3:turnleft();break;//*��ת*/
		case 4:turnright();break;//*��ת*/
		case 5:stop();break;//*ֹͣ*/
		default:break;
	}
}