#ifndef _KEYCORD_H_
#define _KEYCORD_H_
#include "car.h"

sbit keyin4=P2^4;//��������
sbit keyout5=P2^5;//�������ֵ
extern bit keybackup;//�����洢ֵ
extern bit keydate;//������ǰֵ

void keyjudgement();/*�����жϺ���*/
void Delay100ms();		//@11.0592MHz,��ʱ100ms
void carmode(unsigned char cnt);

#endif
