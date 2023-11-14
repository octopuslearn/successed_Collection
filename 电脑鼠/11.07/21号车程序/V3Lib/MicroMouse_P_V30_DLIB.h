#ifndef _MICROMOUSE_P_V30_DLIB_H_
#define _MICROMOUSE_P_V30_DLIB_H_
#include "MicroMouse_Define.h"
void MMMSE(char xDest,char yDest,char GucMapStep[16][16],char GucMapBlock[16][16]);
void MMMGB(char xDest,char yDest,char GucMapStep[16][16],char GucMapBlock[16][16]);
void SIRP(float rf_a,float rf_b,float r30_a,float r30_b,float l30_a,float l30_b,float lf_a,float lf_b);
void IRCD(unsigned short ADCValue[4],float IRDistance[4]);
void SGP(unsigned short gyro_offset,float gyro_mul_param);
void GCA(float GYRO_ADCVALUE,float *Gyro_Angle);
void GC(float GYRO_ADCVALU,unsigned short *GYRO_OFFSE,float GYRO_MUL_PARA);
void IRSPC(float IRDistance[4],char *GucMouseTask,char FinishState);
void IRDC(void);
void MMSPS(void);
void MMSP(void);
void CDR(long x,long y,float *r,long *rad);
BOOL MMPC45(long Pos,long *PulseCtl);
long SCL(long SpeedCorrect);
long SCR(long SpeedCorrect);
long PIDIRSC(void);
void MMCU04590(void);
void MMSWI(void);
char MMGWI(char Pos);
char MMSPC(void);
char MMSEPC(void);
char MMSPP(char xDest,char yDest,char isSpurt);
BOOL MMLPC(long Pos,float *pMinDist,long *PulseCtrl);
#endif
