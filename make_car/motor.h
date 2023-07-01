#ifndef _MOTOR_H_
#define _MOTOR_H_
#include <reg52.h>


sbit In1=P1^0;//左电机-左前
sbit In2=P1^1;
sbit In5=P1^4;//左后
sbit In6=P1^5;

sbit In3=P1^2;//右电机-右前
sbit In4=P1^3;
sbit In7=P1^6;//右后
sbit In8=P1^7;

sbit ENA=P2^0;//左前
sbit ENB=P2^1;//右前
sbit ENC=P2^2;//左后
sbit END=P2^3;//右后

void lef1_go();//*左前前*/
void lef1_back();//*左前后*/
void rig1_go();//*右前前*/
void rig1_back();//*右前后*/
void lef2_go();//*左后前*/
void lef2_back();//*左后后*/
void rig2_go();//*右后前*/
void rig2_back();//*右后后*/
void lef1_stop();//*左前停止*/
void rig1_stop();//*右前停止*/
void lef2_stop();//*左后停止*/
void rig2_stop();//*右后停止*/


#endif