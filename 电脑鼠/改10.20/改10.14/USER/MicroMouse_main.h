#ifndef _MICROMOUSE_MAIN_H_
#define _MICROMOUSE_MAIN_H_
#include "stm32f10x.h"
#include "MicroMouse_Define.h"
#include "MicroMouse_Device_Init.h"
#include "MicroMouse_24L01.h"
#include "MicroMouse_IRSensor.h"
#include "MicroMouse_Driver.h"
#include "MicroMouse_flash.h"
#include "MicroMouse_GlobalVar.h"
#include "MicroMouse_P_V30_DLIB.h"

MOTER GmRight = {MOTORSTOP,MOTORGOAHEAD,400000,0,0,0}; 
MOTER GmLeft  = {MOTORSTOP,MOTORGOAHEAD,400000,0,0,0};
char              GucMouseState  = STOP;
char              GucMousePos    = MIRMOUSE_POS_1;
char              WallInf = 0x03;
char              GucMouseTask = CHECKKEY;
extern float      IRDistance[4];
long              Time = 0;
long              LeanTime = 0;
extern float      Gyro_Angle;
extern char       CrossWayInf;
extern char       GucMapBlock[MAZETYPE][MAZETYPE];
extern MAZECOOR   GmcMouseCoor;
extern char       GucMouseDir;
extern float      IRDistance[4];
extern long       SpeedChangeTime;
extern BOOL       SavedWallInfToFlash;
extern char       CrossWayInf;
extern BOOL       GoToStart;
extern char       SRC_X,SRC_Y,DEST_X,DEST_Y;
extern char       GucMapStep[MAZETYPE][MAZETYPE];
extern BOOL       SavedWallInfToFlash;
#endif

