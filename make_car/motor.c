#include "motor.h"

/*��ǰǰ*/
void lef1_go()
{
	In1=1;//��ǰǰ
	In2=0;
}
/*��ǰ��*/
void lef1_back()
{
	In1=0;//��ǰ��
	In2=1;
}
/*��ǰǰ*/
void rig1_go()
{
	In3=1;//��ǰǰ
	In4=0;
}
/*��ǰ��*/
void rig1_back()
{
	In3=0;//��ǰ��
	In4=1;
}
/*���ǰ*/
void lef2_go()
{
	In5=0;//���ǰ
	In6=1;
}
/*����*/
void lef2_back()
{
	In5=1;//����
	In6=0;
}
/*�Һ�ǰ*/
void rig2_go()
{
	In7=1;//�Һ�ǰ
	In8=0;
}
/*�Һ��*/
void rig2_back()
{
	In7=0;//�Һ��
	In8=1;
}
/*��ǰֹͣ*/
void lef1_stop()
{
	In1=1;
	In2=1;
}
/*��ǰֹͣ*/
void rig1_stop()
{
	In3=1;
	In4=1;
}
/*���ֹͣ*/
void lef2_stop()
{
	In5=1;
	In6=1;
}
/*�Һ�ֹͣ*/
void rig2_stop()
{
	In7=1;
	In8=1;
}