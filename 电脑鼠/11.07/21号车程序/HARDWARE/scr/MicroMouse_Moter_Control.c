#include "MicroMouse_Moter_Control.h"
#include "MicroMouse_P_V30_DLIB.h"
extern MOTER GmRight;
extern MOTER GmLeft;
BOOL  Moter_Started_R = FALSE;
BOOL  Moter_Started_L = FALSE;
s32   countLast = 0,countCurrent=0;
float CorrectFactor = 1.0f;
s32 ENC_Calc_Rot_Speed_L(void)
{
    static u16 hPrevious_Count = 0;
    s16 wDelta_angle;
    u16 hCurrent_Count;
    hCurrent_Count = TIM3->CNT;
    if(Moter_Started_L == FALSE)
    {
        hPrevious_Count = hCurrent_Count;
        Moter_Started_L  = TRUE;
    }
    wDelta_angle = (s16)(hCurrent_Count - hPrevious_Count);
    if(wDelta_angle < -5000)
        wDelta_angle += 10000;
    if(wDelta_angle > 5000)
        wDelta_angle -= 10000;
    hPrevious_Count = hCurrent_Count;
    return(wDelta_angle);
}
s32 ENC_Calc_Rot_Speed_R(void)
{
    static u16 hPrevious_Count = 0;
    s16 wDelta_angle;
    u16 hCurrent_Count;
    hCurrent_Count = TIM4->CNT;
    if(Moter_Started_R == FALSE)
    {
        hPrevious_Count = hCurrent_Count;
        Moter_Started_R = TRUE;
    }
    wDelta_angle = (s16)(hCurrent_Count - hPrevious_Count);
    if(wDelta_angle < -5000)
        wDelta_angle += 10000;
    if(wDelta_angle > 5000)
        wDelta_angle -= 10000;
    hPrevious_Count = hCurrent_Count;
    return(wDelta_angle);
}