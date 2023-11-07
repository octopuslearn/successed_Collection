#ifndef MIRMOUSE_MOTER_CONTROL_H_
#define MIRMOUSE_MOTER_CONTROL_H_

#include "stm32f10x.h"
#include "MicroMouse_Define.h"
#include "MicroMouse_PID_Control.h"

s32 ENC_Calc_Rot_Speed_L(void);
s32 ENC_Calc_Rot_Speed_R(void);

#endif

