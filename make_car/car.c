#include "car.h"

/*ǰ��*/
void go()
{
	lef1_go();//*��ǰǰ*/
	rig1_go();//*��ǰǰ*/
	lef2_go();//*���ǰ*/
	rig2_go();//*�Һ�ǰ*/
}
/*����*/
void back()
{
	lef1_back();//*��ǰ��*/
	rig1_back();//*��ǰ��*/
	lef2_back();//*����*/
	rig2_back();//*�Һ��*/
}
/*��ת*/
void turnleft()
{
	rig1_go();//*��ǰǰ*/
	rig2_go();//*�Һ�ǰ*/
	lef1_stop();//*��ǰֹͣ*/
	lef2_stop();//*���ֹͣ*/
}
/*��ת*/
void turnright()
{
	lef1_go();//*��ǰǰ*/
	lef2_go();//*���ǰ*/
	rig1_stop();//*��ǰֹͣ*/
	rig2_stop();//*�Һ�ֹͣ*/
}
/*ֹͣ*/
void stop()
{
	lef1_stop();//*��ǰֹͣ*/
	rig1_stop();//*��ǰֹͣ*/
	lef2_stop();//*���ֹͣ*/
	rig2_stop();//*�Һ�ֹͣ*/
}