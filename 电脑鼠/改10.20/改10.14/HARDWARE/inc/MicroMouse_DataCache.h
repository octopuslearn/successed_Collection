
#ifndef _MIRMOUSE_DATACACHE_H_
#define _MIRMOUSE_DATACACHE_H_

#include "MicroMouse_GlobalVar.h"
#include "MicroMouse_24L01.h"


#define DTCH_MAX_NUM        10
#define DTCH_SIZE           32

extern char g_DataCathe[DTCH_MAX_NUM*DTCH_SIZE];
extern char g_RecDataCache[DTCH_SIZE];

void PushOneDataFrame(void);
void SendOneDataFrame(void);
char * GetAddBufferPointer(void);
void ReadOneFrameData(void);
#endif 
