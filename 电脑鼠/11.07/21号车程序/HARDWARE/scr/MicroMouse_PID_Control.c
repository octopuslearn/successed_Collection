#include "MicroMouse_PID_Control.h"
extern char     GucMouseState ;
extern char     GucMouseTask  ;
extern char     GucMousePos   ;
extern char     GucMouseDir   ;
extern char     WallInf       ;
extern float    IRDistance[4] ;
extern char     Test;
extern MOTER    GmRight;
long Dist_Correct = 0;
long PID_RMoter_Control(long Idle_Speed,long Infact_Speed_Now)
{
    signed long PWM_Width,Error = 0;
    static signed long AllError = 0;static signed long LastError = 0;
    Error = Idle_Speed - Infact_Speed_Now;
    AllError += Error;
    if(AllError > 2000)
        AllError = (signed long)(2000);
    if(AllError < -2000)
        AllError = (signed long)(-2000);
    PWM_Width = (signed long)(0.3*AllError+9*(AllError-LastError));LastError = AllError;
    if(PWM_Width > PID_MAX)
        PWM_Width = PID_MAX;
    if(PWM_Width < PID_MIN)
        PWM_Width = PID_MIN;
    return PWM_Width;
}
long PID_LMoter_Control(long Idle_Speed,long Infact_Speed_Now)
{
    signed long PWM_Width,Error = 0;
    static signed long AllError = 0;static signed long LastError = 0;
    Error = Idle_Speed - Infact_Speed_Now;
    AllError += Error;
    if(AllError > 2000)
        AllError = (signed long)(2000);
    if(AllError < -2000)
        AllError = (signed long)(-2000);
    PWM_Width = (signed long)(0.3*AllError+9*(AllError-LastError));LastError = AllError;
    if(PWM_Width > PID_MAX)
        PWM_Width = PID_MAX;
    if(PWM_Width < PID_MIN)
        PWM_Width = PID_MIN;
    return PWM_Width;
}