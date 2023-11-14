#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_
#include "MicroMouse_Define.h"
#include "MicroMouse_GlobalVar.h"

#ifdef DEBUG
  #define K_P                   grc_moter_p   
  #define K_I                   grc_moter_i
  #define K_D                   grc_moter_d
#endif

#define PID_MAX                 1000
#define PID_MIN                 -1000

#define S_P                     grc_irsensor_p  
#define S_I                     grc_irsensor_i 
#define S_D                     grc_irsensor_d

#define PID_ERROR_MAX           50
#define PID_ERROR_MIN           -50

#define PID_IR_MAX              30
#define PID_IR_MIN              -30

#define PID_LEAN_IR_MAX         0.1
#define PID_LEAN_IR_MIN         -0.1

#define PID_IR_DEADBAND         0.3

#define LR45_DIST                8.5

#define LR45_DIST_MIN            8.5
#define LR45_DIST_MAX            9.5

#define LR_CASE_DIST             8


#define IR__LR_HAVE_WALL_DIST    16
#define IR__FT_HAVE_WALL_DIST    50

long PID_RMoter_Control(long Idle_Speed,long Infact_Speed_Now);
long PID_LMoter_Control(long Idle_Speed,long Infact_Speed_Now);
#endif