#ifndef _KEYCORD_H_
#define _KEYCORD_H_
#include "car.h"

sbit keyin4=P2^4;//按键输入
sbit keyout5=P2^5;//按键输出值
extern bit keybackup;//按键存储值
extern bit keydate;//按键当前值

void keyjudgement();/*按键判断函数*/
void Delay100ms();		//@11.0592MHz,延时100ms
void carmode(unsigned char cnt);

#endif
