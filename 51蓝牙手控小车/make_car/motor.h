#ifndef _MOTOR_H_
#define _MOTOR_H_
#include <reg52.h>


sbit In1=P1^0;//����-��ǰ
sbit In2=P1^1;
sbit In5=P1^4;//���
sbit In6=P1^5;

sbit In3=P1^2;//�ҵ��-��ǰ
sbit In4=P1^3;
sbit In7=P1^6;//�Һ�
sbit In8=P1^7;

sbit ENA=P2^0;//��ǰ
sbit ENB=P2^1;//��ǰ
sbit ENC=P2^2;//���
sbit END=P2^3;//�Һ�

void lef1_go();//*��ǰǰ*/
void lef1_back();//*��ǰ��*/
void rig1_go();//*��ǰǰ*/
void rig1_back();//*��ǰ��*/
void lef2_go();//*���ǰ*/
void lef2_back();//*����*/
void rig2_go();//*�Һ�ǰ*/
void rig2_back();//*�Һ��*/
void lef1_stop();//*��ǰֹͣ*/
void rig1_stop();//*��ǰֹͣ*/
void lef2_stop();//*���ֹͣ*/
void rig2_stop();//*�Һ�ֹͣ*/


#endif