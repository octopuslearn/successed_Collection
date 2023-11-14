#include "MicroMouse_Driver.h"
#include "stm32f10x.h"
#include "MicroMouse_flash.h"
#include "MicroMouse_Device_Init.h"
#include "MicroMouse_P_V30_DLIB.h"
extern  char      GucMousePos;
extern  char      GucMouseState;
extern  char      WallInf;
extern  MOTER     GmRight;
extern  MOTER     GmLeft;
extern  float     IRDistance[4];
extern  float     Gyro_Angle;
char      CrossWayInf;
extern  long      Time;
char      GucMapBlock[MAZETYPE][MAZETYPE];
MAZECOOR  GmcMouseCoor;
char      GucMouseDir = 0;
char      GucMapStep[MAZETYPE][MAZETYPE];
SPURTSTEP SpurtStep[MAZETYPE*MAZETYPE];
long      Step = 0;
char      SRC_X = 0,SRC_Y = 0,DEST_X = 7,DEST_Y = 7;  //终点坐标
long      SpeedChangeTime = 0;
char      Test = 0;
BOOL      Speed_Add = FALSE,Add_Finish = FALSE;
extern  char      GucMouseTask;
BOOL      GoToStart = FALSE;
BOOL      WentToDest = FALSE;
BOOL      SavedWallInfToFlash = FALSE;
SPEEDCHANGESYS SpeedChangeSys[MAZETYPE*MAZETYPE];
extern long LeanTime;
void MicroMouse_GoAhead(float Dist)
{
    long  PulseCtr;
    char  WallInfNow;
		char  StepGoBackAllWall=0;
    BOOL  bPos_Correct = FALSE,bCoorUpdate = FALSE,bCrossWayChoice = FALSE;
    PulseCtr = MICROMOUSE_ONE_BLOCK_PAULSE - (long)(Dist*MICROMOUSE_PAUSE_PER_CM);
    GucMouseState       = GOAHEAD;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = MICROMOUSE_PAUSE_GO_AHEAD;
    GmLeft.ulPulseCtr   = PulseCtr;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = MICROMOUSE_PAUSE_GO_AHEAD;
    GmRight.ulPulseCtr  = PulseCtr;
    WallInfNow = WallInf;
    while(1)
    {
        PulseCtr = GmRight.ulPulseCtr + GmLeft.ulPulseCtr;

        if((PulseCtr > MICROMOUSE_PAULSE_COR_MIN) && (PulseCtr < MICROMOUSE_PAULSE_COR_MAX))
        {
            if(bPos_Correct == FALSE)
            {
                if((WallInfNow & 0x01) && (IRDistance[1] > MICROMOUSE_IR_DIST_COR))
                {
                    Time = 0;
                    while(Time==0);
                    if(IRDistance[1] > MICROMOUSE_IR_DIST_COR)
                    {
                        GmLeft.ulPulseCtr = MICROMOUSE_POS_COR_PAULSE;
                        GmRight.ulPulseCtr = MICROMOUSE_POS_COR_PAULSE;
                    }
                    bPos_Correct = TRUE;
                }
                if((WallInfNow & 0x02) && (IRDistance[2] > MICROMOUSE_IR_DIST_COR))
                {
                    Time = 0;
                    while(Time==0);
                    if(IRDistance[2] > MICROMOUSE_IR_DIST_COR)
                    {
                        GmLeft.ulPulseCtr = MICROMOUSE_POS_COR_PAULSE;
                        GmRight.ulPulseCtr = MICROMOUSE_POS_COR_PAULSE;
                    }
                    bPos_Correct = TRUE;
                }
            }
        }
        if(PulseCtr < MICROMOUSE_POS_1_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_1;
        }
        else if(PulseCtr < MICROMOUSE_POS_2_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_2;
        }
        else if(PulseCtr < MICROMOUSE_POS_3_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_3;
        }
        else if(PulseCtr < MICROMOUSE_POS_4_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_4;
        }
        else if(PulseCtr < MICROMOUSE_POS_5_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_5;
        }
        else if(PulseCtr < MICROMOUSE_POS_6_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_6;
            if(bCoorUpdate == FALSE)
            {
                MicroMouse_CoorUpdate();
                MMSWI();
								MicroMouse_HandleStartPos();
                bCoorUpdate = TRUE;
            }
            if(bCrossWayChoice == FALSE)
            {
                bCrossWayChoice = TRUE;
                if((WentToDest == FALSE) && (GmcMouseCoor.cX == DEST_X) && (GmcMouseCoor.cY == DEST_Y))
                {
                    WentToDest =  TRUE;
										SavedWallInfToFlash = TRUE;
                }
                if(WentToDest == TRUE)
                {
                    MMMGB(SRC_X,SRC_Y,GucMapStep,GucMapBlock);
											StepGoBackAllWall = GucMapStep[DEST_X][DEST_Y];
											MMMSE(SRC_X,SRC_Y,GucMapStep,GucMapBlock);
											if(StepGoBackAllWall == GucMapStep[DEST_X][DEST_Y])
											{
													MMMGB(SRC_X,SRC_Y,GucMapStep,GucMapBlock);
													GoToStart = TRUE;
											}
                }
                else
                {
                    MMMSE(DEST_X,DEST_Y,GucMapStep,GucMapBlock);
                }
                CrossWayInf = MMSEPC();
            }
        }
        else
            break;
    }
}
void MicroMouse_CrosswayChoice(char drad)
{
    if(drad == 0)
    {
        GucMousePos = MIRMOUSE_POS_1;
    }
    else if(drad == 2)
    {
        MicroMouse_TurnRight90();
    }
    else if(drad == 6)
    {
        MicroMouse_TurnLeft90();
    }
    else if(drad == 8)
    {
        MicroMouse_SearchObjectTo();
    }
    else if(drad == 10)
    {
        MicroMouse_SearchObjectTo();
    }
}
void MicroMouse_TurnRight90(void)
{
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 250000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 250000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    Time = 0;
    Gyro_Angle = 0;
    long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
    while(TempTime < MICROMOSUE_TR90_ACC_TIME)
    {
        TempTime  = Time;
        GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED + TempTime;
        GmRight.lIdleSpeed = MAZE_SEARCH_SPEED - TempTime;
    }
    GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED + MICROMOSUE_TR90_ACC_TIME;
    GmRight.lIdleSpeed = MAZE_SEARCH_SPEED - MICROMOSUE_TR90_ACC_TIME;
    LeftSpeed = GmLeft.lIdleSpeed;
    RightSpeed = GmRight.lIdleSpeed;
    while(1)
    {
        if(Gyro_Angle > MICROMOUSE_TR90_GYRO_ANGLE)
        {
            break;
        }
    }
    Time = 0;
    Gyro_Angle = 0;
    TempTime = 0;
    while(TempTime < MICROMOSUE_TR90_ACC_TIME)
    {
        GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
        GmRight.lIdleSpeed = RightSpeed + TempTime;
        TempTime  = Time;
    }
    GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED;
    GmRight.lIdleSpeed = MAZE_SEARCH_SPEED;
    DEBUG_LED_OFF
    GucMouseDir = (GucMouseDir + 2)%8;
    WallInf = 0;
    GucMousePos = MIRMOUSE_POS_5;
}
void MicroMouse_TurnLeft90(void)
{
    GucMouseState = TURNLEFT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 250000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 250000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    Time = 0;
    Gyro_Angle = 0;
    long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
    while(TempTime < MICROMOSUE_TL90_ACC_TIME)
    {
        TempTime  = Time;
        GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED - TempTime;
        GmRight.lIdleSpeed = MAZE_SEARCH_SPEED + TempTime;
    }
    GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED - MICROMOSUE_TL90_ACC_TIME;
    GmRight.lIdleSpeed = MAZE_SEARCH_SPEED + MICROMOSUE_TL90_ACC_TIME;
    LeftSpeed = GmLeft.lIdleSpeed;
    RightSpeed = GmRight.lIdleSpeed;
    while(1)
    {
        if(Gyro_Angle < MICROMOUSE_TL90_GYRO_ANGLE)
        {
            break;
        }
    }
    Time = 0;
    Gyro_Angle = 0;
    TempTime = 0;
    while(TempTime < MICROMOSUE_TL90_ACC_TIME)
    {
        GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
        GmRight.lIdleSpeed = RightSpeed - TempTime;
        TempTime  = Time;
    }
    GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED;
    GmRight.lIdleSpeed = MAZE_SEARCH_SPEED;
    DEBUG_LED_OFF
    GucMouseDir = (GucMouseDir + 6)%8;
    WallInf = 0;
    GucMousePos = MIRMOUSE_POS_5;
}
void MicroMouse_TurnBack(BOOL bACC,BOOL bPosCor)
{
    long Speed;
    float ir_dist_1;
    GucMouseState = TURNBACK;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = MICROMOUSE_PAUSE_TURN_BACK;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = MICROMOUSE_PAUSE_TURN_BACK;
    GmRight.ulPulseCtr  = 0;
    float SpeedCor = 0;
    DEBUG_LED_ON
    if(GucMapBlock[GmcMouseCoor.cX][GmcMouseCoor.cY] & (1 << (GucMouseDir/2)))
    {
        while((IRDistance[0] +  IRDistance[3]) > 16)
        {
            SpeedCor = ((IRDistance[0] - IRDistance[3])*2);
            if(SpeedCor > 5)
                SpeedCor = 5;
            if(SpeedCor < -5)
                SpeedCor = -5;
            GmLeft.lIdleSpeed  = (int)(MAZE_SEARCH_SPEED - SpeedCor);
            GmRight.lIdleSpeed = (int)(MAZE_SEARCH_SPEED + SpeedCor);
        }
    }
    Time = 0;
    while(1)
    {
        Speed = Time ;
        if(Speed < MAZE_SEARCH_SPEED)
        {
            GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED - Speed;
            GmRight.lIdleSpeed = MAZE_SEARCH_SPEED - Speed;
        }
        else
        {
            GmLeft.lIdleSpeed  = 0;
            GmRight.lIdleSpeed = 0;
            break;
        }
    }
    Time = 0;
    while(Time < MICROMOUSE_TB90_SLEEP_TIME);
    ir_dist_1 = (IRDistance[0] +  IRDistance[3])/2.0;
    Gyro_Angle = 0;
    Time = 0;
    while(1)
    {
        Speed = Time ;
        if(Speed < MICROMOSUE_TB90_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = Speed;
            GmRight.lIdleSpeed = -Speed;
        }
        else
        {
            GmLeft.lIdleSpeed  = MICROMOSUE_TB90_ACC_TIME;
            GmRight.lIdleSpeed = -MICROMOSUE_TB90_ACC_TIME;
            if(Gyro_Angle > MICROMOUSE_TB90_GYRO_ANGLE)
                break;
        }
    }
    GucMouseDir = (GucMouseDir + 4)%8;
    Time = 0;
    while(1)
    {
        Speed = Time ;
        if(Speed < MICROMOSUE_TB90_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = MICROMOSUE_TB90_ACC_TIME - Speed;
            GmRight.lIdleSpeed = -MICROMOSUE_TB90_ACC_TIME + Speed;
        }
        else
        {
            GmLeft.lIdleSpeed  = 0;
            GmRight.lIdleSpeed = 0;
            break;
        }
    }
    if(bACC == TRUE)
    {
        Time = 0;
        while(Time < MICROMOUSE_TB90_SLEEP_TIME);
        Time = 0;
        while(1)
        {
            Speed = Time ;
            if(Speed < MAZE_SEARCH_SPEED)
            {
                GmLeft.lIdleSpeed  = Speed;
                GmRight.lIdleSpeed = Speed;
            }
            else
            {
                GmLeft.lIdleSpeed  = MAZE_SEARCH_SPEED;
                GmRight.lIdleSpeed = MAZE_SEARCH_SPEED;
                break;
            }
        }
    }
    else
    {
//        MicroMouse_SimpleAhead(ir_dist_1+2.5,0.0);
			MicroMouse_SimpleAhead(ir_dist_1+3,0.0);
        Time=0;
        while(Time < MICROMOUSE_TB90_SLEEP_TIME);
    }
    DEBUG_LED_OFF
}
void MicroMouse_SimpleAhead(float back_distance,float go_distance)
{
    long back_speed;
    long pulse;
    pulse = (back_distance + go_distance)*MICROMOUSE_PAUSE_PER_CM;
    GucMouseState = SIMPLEAHEAD;
    GmLeft.ulPulse      = pulse;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ulPulse     = pulse;
    GmRight.ulPulseCtr  = 0;
    if(back_distance > 0.0)
    {
        while(1)
        {
            if((GmRight.ulPulse + GmLeft.ulPulse)/2 - abs((GmRight.ulPulseCtr + GmLeft.ulPulseCtr)/2) < 138)
            {
                Time = 0;
                back_speed = Time;
                while(back_speed<= 30)
                {
                    GmLeft.lIdleSpeed  = -30 + back_speed;
                    GmRight.lIdleSpeed = -30 + back_speed;
                    back_speed = Time;
                }
                break;
            }
            else
            {
                GmLeft.lIdleSpeed  = -30;
                GmRight.lIdleSpeed = -30;
            }
        }
    }
    else if(go_distance > 0.0)
    {
        while(1)
        {
            if((GmRight.ulPulse + GmLeft.ulPulse)/2 - abs((GmRight.ulPulseCtr + GmLeft.ulPulseCtr)/2) < 138)
            {
                Time = 0;
                back_speed = Time;
                while(back_speed<= 10)
                {
                    GmLeft.lIdleSpeed  = 10 - back_speed;
                    GmRight.lIdleSpeed = 10 - back_speed;
                    back_speed = Time;
                }
                break;
            }
            else
            {
                GmLeft.lIdleSpeed  = 10;
                GmRight.lIdleSpeed = 10;
            }
        }
    }
    GucMouseState = TURNBACK;
}
void MicroMouse_CoorUpdate(void)
{
    switch(GucMouseDir)
    {
    case 0:
        GmcMouseCoor.cY++;
        break;
    case 2:
        GmcMouseCoor.cX++;
        break;
    case 4:
        GmcMouseCoor.cY--;
        break;
    case 6:
        GmcMouseCoor.cX--;
        break;
    default:
        break;
    }
}


void MicroMouse_MazeBlockDataGet(char  ucDirTemp,char *Block,char *Step)
{
    char cX = 0,cY = 0;
    switch (ucDirTemp)
    {
    case 0:
        ucDirTemp = GucMouseDir/2;
        break;
    case 6:
        ucDirTemp = ((GucMouseDir + 6) % 8)/2;
        break;
    case 2:
        ucDirTemp = ((GucMouseDir + 2) % 8)/2;
        break;
    case 4:
        ucDirTemp = ((GucMouseDir + 4) % 8)/2;
        break;
    default:
        break;
    }
    switch (ucDirTemp)
    {
    case 0:
        cX = GmcMouseCoor.cX;
        cY = GmcMouseCoor.cY + 1;
        break;
    case 1:
        cX = GmcMouseCoor.cX + 1;
        cY = GmcMouseCoor.cY;
        break;
    case 2:
        cX = GmcMouseCoor.cX;
        cY = GmcMouseCoor.cY - 1;
        break;
    case 3:
        cX = GmcMouseCoor.cX - 1;
        cY = GmcMouseCoor.cY;
        break;
    default:
        break;
    }
    *Block	= GucMapBlock[cX][cY];
    *Step	= GucMapStep[cX][cY];
}
void MicroMouse_SearchGoAhead(long Pos)
{
    char WallInf_Next=0,Wall_Result,cBlock = 0;
    long PulseCtl = 0,Pulse_Total,TurnPulse;
    BOOL Sys_Init = FALSE,Gain_WallInf = FALSE,Coor_Updata = FALSE,ReGain_WallInf = FALSE;
    BOOL Pos_Correct = FALSE,Get_Pos_WallInf = FALSE;
    BOOL SpeedChange = FALSE;
    GucMouseState       = GOAHEAD;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = SpurtStep[Pos].turnpulse + 20000;
    GmLeft.ulPulseCtr   = SpurtStep[Pos].pulsectl;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = SpurtStep[Pos].turnpulse + 20000;
    GmRight.ulPulseCtr  = SpurtStep[Pos].pulsectl;
    TurnPulse = SpurtStep[Pos].turnpulse*2;
    Pulse_Total = (long)(SpurtStep[Pos].dist*2);
    if(SpurtStep[Pos].pulsectl > 13*MICROMOUSE_PAUSE_PER_CM)
    {
        WallInf = MMGWI(1);
    }
    else
    {
        WallInf = MMGWI(0);
    }
    while(1)
    {
        if(PulseCtl < TurnPulse)
        {
            PulseCtl = GmLeft.ulPulseCtr + GmRight.ulPulseCtr;
        }
        else
        {
            Speed_Add = FALSE;
            Add_Finish = FALSE;
            break;
        }
        if(SpeedChange == FALSE)
        {
            MicroMouse_Speed_Change_Sys(Pos,&SpeedChange,PulseCtl,0.5);
        }
        if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SGA_POS_1_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_1;
            if(Sys_Init == FALSE)
            {
                Sys_Init = TRUE;
                Gain_WallInf = FALSE;
                Coor_Updata  = FALSE;
                ReGain_WallInf = FALSE;
                Get_Pos_WallInf = FALSE;
                Pos_Correct  =  FALSE;
            }
        }
        else if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SGA_POS_2_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_2;
            if(Gain_WallInf == FALSE)
            {
                Gain_WallInf = TRUE;
                WallInf_Next = MMGWI(1);
                if(WallInf & WallInf_Next)
                {
                    WallInf &= WallInf_Next;
                    if(WallInf_Next == 0x03)
                        WallInf = 0x03;
                    ReGain_WallInf = TRUE;
                }
                else
                {
                    WallInf = 0;
                    ReGain_WallInf = FALSE;
                }
            }
        }
        else if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SGA_POS_3_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_3;
            if(ReGain_WallInf == FALSE)
            {
                ReGain_WallInf = TRUE;
                WallInf_Next = MMGWI(1);
                WallInf = WallInf_Next;
            }
        }
        else if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SGA_POS_4_PAULSE)
        {
            GucMousePos = MIRMOUSE_POS_4;
            if(Coor_Updata == FALSE)
            {
                Coor_Updata = TRUE;
                Sys_Init = FALSE;
                cBlock ++;
                MMCU04590();
            }
        }
        if((PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE > MICROMOUSE_PAULSE_COR_MIN)
                && (PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_PAULSE_COR_MAX))
        {
            if(Get_Pos_WallInf == FALSE)
            {
                Get_Pos_WallInf = TRUE;
                Wall_Result = MMSPC();
            }
            if(Pos_Correct == FALSE)
            {
                if((Wall_Result == 1) || (Wall_Result == 2))
                {
                    if(IRDistance[Wall_Result] > MICROMOUSE_IR_DIST_COR)
                    {
                        Time = 0;
                        while(Time==0);
                        if(IRDistance[Wall_Result] > MICROMOUSE_IR_DIST_COR)
                        {
                            Pos_Correct = TRUE;
                            GmLeft.ulPulseCtr =MICROMOUSE_ONE_BLOCK_PAULSE*cBlock + MICROMOUSE_POS_COR_PAULSE;
                            GmRight.ulPulseCtr=MICROMOUSE_ONE_BLOCK_PAULSE*cBlock + MICROMOUSE_POS_COR_PAULSE;
                            PulseCtl = GmLeft.ulPulseCtr + GmRight.ulPulseCtr;
                        }
                    }
                }
            }
        }
    }
}
void MicroMouse_SearchObjectTo(void)
{
    long i;
    GucMouseTask = SPURT;
    for(i = 0; i< Step; i++)
    {
        MicroMouse_SearchGoAhead(i);
        if(SpurtStep[i].nextrad == 2)
        {
            MicroMouse_TurnRight90();
        }
        else if(SpurtStep[i].nextrad == 4)
        {
            MicroMouse_TurnBack(TRUE,TRUE);
        }
        else if(SpurtStep[i].nextrad == 6)
        {
            MicroMouse_TurnLeft90();
        }
    }
    GucMouseTask = MAZESEARCH;
    GucMousePos = MIRMOUSE_POS_5;
}

void MicroMouse_Speed_Change_Sys(long Pos,BOOL *Dec_Finish,long PulseCtl,float acc)
{
    static BOOL bMAX_SPEED = FALSE;
    long   Speed;
    
    *Dec_Finish = FALSE;
    if((SpeedChangeSys[Pos].Speed_Start != 0) && (PulseCtl < (6*MICROMOUSE_PAUSE_PER_CM  + 2*SpurtStep[Pos].pulsectl)))
        return ;
    if(Speed_Add == FALSE)
    {
        Speed_Add = TRUE;
        
        SpeedChangeTime = 0;
        if(SpeedChangeSys[Pos].Speed_Max == MICROMOUSE_SPURT_MAX_SPEED)
        {
            bMAX_SPEED = TRUE;
        }
        else
        {
            bMAX_SPEED = FALSE;
        }
    }
    
    if(Add_Finish == FALSE)
    {
        if(GmLeft.lIdleSpeed < SpeedChangeSys[Pos].Speed_Max)
        {
            Speed =(int)(SpeedChangeTime*acc);
            GmLeft.lIdleSpeed  = (long)(SpeedChangeSys[Pos].Speed_Start + Speed);
            GmRight.lIdleSpeed = (long)(SpeedChangeSys[Pos].Speed_Start + Speed);
        }
        else
        {
            GmLeft.lIdleSpeed  = SpeedChangeSys[Pos].Speed_Max;
            GmRight.lIdleSpeed = SpeedChangeSys[Pos].Speed_Max;
        }
        
        if((bMAX_SPEED == FALSE) && (GmLeft.lIdleSpeed >= SpeedChangeSys[Pos].Speed_Max))
        {
            Add_Finish = TRUE;
            SpeedChangeTime = 0;
        }
        else if((bMAX_SPEED == TRUE) && (SpeedChangeSys[Pos].PulseCtl_Dec*1.5 <= PulseCtl))
        {
            Add_Finish = TRUE;
            SpeedChangeTime = 0;
        }
    }
    
    if(Add_Finish == TRUE)
    {
        if(GmLeft.lIdleSpeed > SpeedChangeSys[Pos].Speed_End)
        {
            Speed = (int)(SpeedChangeTime*acc);
            GmLeft.lIdleSpeed  = (long)(SpeedChangeSys[Pos].Speed_Max - Speed);
            GmRight.lIdleSpeed = (long)(SpeedChangeSys[Pos].Speed_Max - Speed);
        }
        else
        {
            GmLeft.lIdleSpeed  = SpeedChangeSys[Pos].Speed_End;
            GmRight.lIdleSpeed = SpeedChangeSys[Pos].Speed_End;
            
            Speed_Add = FALSE;
            Add_Finish = FALSE;
            
            *Dec_Finish = TRUE; 
        }
    }
}

void MicroMouse_SpurtGoAhead(long Pos)
{
    char WallInf_Next,Wall_Result,cBlock = 0;
    long PulseCtl = 0,Pulse_Total,TurnPulse;
    BOOL Sys_Init = FALSE,Gain_WallInf = FALSE,Coor_Updata = FALSE,ReGain_WallInf = FALSE;
    BOOL Pos_Correct = FALSE,Get_Pos_WallInf = FALSE;
    BOOL SpeedChange = FALSE;
    float MinDist = 300.0;
    BOOL Pos_Correct45 = TRUE,Finish_Coor = FALSE;
    GucMouseState       = GOAHEAD;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = SpurtStep[Pos].turnpulse + 7000;
    GmLeft.ulPulseCtr   = SpurtStep[Pos].pulsectl;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = SpurtStep[Pos].turnpulse + 7000;
    GmRight.ulPulseCtr  = SpurtStep[Pos].pulsectl;
    WallInf = MMGWI(0);
    TurnPulse = SpurtStep[Pos].turnpulse*2;
    Pulse_Total = (long)(SpurtStep[Pos].dist*2);
    if(GucMouseDir%2)
    {
        while(1)
        {
            if(PulseCtl + 37.6*MICROMOUSE_PAUSE_PER_CM > Pulse_Total)
            {
                if(Pos_Correct45 == TRUE)
                {
                    Pos_Correct45 = MMLPC(Pos,&MinDist,&PulseCtl);
                }
            }
            if(PulseCtl < TurnPulse)
            {
                PulseCtl = GmLeft.ulPulseCtr + GmRight.ulPulseCtr;
            }
            else
            {
                if((Pos_Correct45 == FALSE) )
                {
                    Speed_Add = FALSE;
                    Add_Finish = FALSE;
                    break;
                }
            }
            if(PulseCtl + 30*MICROMOUSE_PAUSE_PER_CM > Pulse_Total )
            {
                if(SpurtStep[Pos].nextrad > 0)
                {
                    GucMousePos = MIRMOUSE_POS_2;
                }
                else if(SpurtStep[Pos].nextrad < 0)
                {
                    GucMousePos = MIRMOUSE_POS_3;
                }
            }
            else
                GucMousePos = MIRMOUSE_POS_1;
            if(SpeedChange == FALSE)
            {
                MicroMouse_Speed_Change_Sys(Pos,&SpeedChange,PulseCtl,1);
            }
            if(Finish_Coor == FALSE)
            {
                if(PulseCtl%(int)(25.452*MICROMOUSE_PAUSE_PER_CM) < 24*MICROMOUSE_PAUSE_PER_CM)
                {
                    Coor_Updata = FALSE;
                }
                else if(PulseCtl%(int)(25.452*MICROMOUSE_PAUSE_PER_CM) < (int)(25.452*MICROMOUSE_PAUSE_PER_CM))
                {
                    if(Coor_Updata == FALSE)
                    {
                        Coor_Updata = TRUE;
                        if(PulseCtl + MICROMOUSE_SPURT_LEAN_STARTPOSCOR_PAULSE> Pulse_Total)
                            Finish_Coor = TRUE;
                        MMCU04590();
                    }
                }
            }
        }
    }
    else
    {
        while(1)
        {
            Test = WallInf;
            if(PulseCtl < TurnPulse)
            {
                PulseCtl = GmLeft.ulPulseCtr + GmRight.ulPulseCtr;
            }
            else
            {
                if(Pos_Correct == TRUE)
                {
                    Speed_Add = FALSE;
                    Add_Finish = FALSE;
                    break;
                }
            }
            if(SpeedChange == FALSE)
            {
                MicroMouse_Speed_Change_Sys(Pos,&SpeedChange,PulseCtl,1);
            }
            if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SPURT_POS_1_PAUSE)
            {
                GucMousePos = MIRMOUSE_POS_1;
                if(Sys_Init == FALSE)
                {
                    Sys_Init = TRUE;
                    Gain_WallInf = FALSE;
                    Coor_Updata  = FALSE;
                    ReGain_WallInf = FALSE;
                    Get_Pos_WallInf = FALSE;
                    Pos_Correct  =  FALSE;
                }
            }
            else if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SPURT_POS_2_PAUSE)
            {
                GucMousePos = MIRMOUSE_POS_2;
                if(Gain_WallInf == FALSE)
                {
                    Gain_WallInf = TRUE;
                    WallInf_Next = MMGWI(1);
                    if(WallInf & WallInf_Next)
                    {
                        WallInf &= WallInf_Next;
                        if(WallInf_Next == 0x03)
                            WallInf = 0x03;
                        ReGain_WallInf = TRUE;
                    }
                    else
                    {
                        WallInf = 0;
                        ReGain_WallInf = FALSE;
                    }
                }
            }
            else if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SPURT_POS_3_PAUSE)
            {
                GucMousePos = MIRMOUSE_POS_3;
                if(ReGain_WallInf == FALSE)
                {
                    ReGain_WallInf = TRUE;
                    WallInf = WallInf_Next;
                }
            }
            else if(PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SPURT_POS_4_PAUSE)
            {
                GucMousePos = MIRMOUSE_POS_4;
                if(Coor_Updata == FALSE)
                {
                    Coor_Updata = TRUE;
                    Sys_Init = FALSE;
                    cBlock ++;
                    MMCU04590();
                }
            }
            if((PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE > MICROMOUSE_SPURT_BEGIN_POSCOR_DIST) && (PulseCtl%MICROMOUSE_D_ONE_BLOCK_PAULSE < MICROMOUSE_SPURT_END_POSCOR_DIST))
            {
                if(Get_Pos_WallInf == FALSE)
                {
                    Get_Pos_WallInf = TRUE;
                    Wall_Result = MMSPC();
                }

                if(Pos_Correct == FALSE)
                {
                    if((Wall_Result == 1) || (Wall_Result == 2))
                    {
                        if(IRDistance[Wall_Result] > MICROMOUSE_IR_DIST_COR)
                        {
                            Time = 0;
                            while(Time==0);
                            if(IRDistance[Wall_Result] > MICROMOUSE_IR_DIST_COR)
                            {
                                Pos_Correct = TRUE;
                                GmLeft.ulPulseCtr =(long)(MICROMOUSE_ONE_BLOCK_PAULSE*cBlock + MICROMOUSE_POS_COR_DIST_CM*MICROMOUSE_PAUSE_PER_CM);
                                GmRight.ulPulseCtr=(long)(MICROMOUSE_ONE_BLOCK_PAULSE*cBlock + MICROMOUSE_POS_COR_DIST_CM*MICROMOUSE_PAUSE_PER_CM);
                                PulseCtl = GmLeft.ulPulseCtr + GmRight.ulPulseCtr;
                            }
                        }
                    }
                    else
                    {
                        if(PulseCtl + MICROMOUSE_SPURT_STARTADJ_FRONT_CORPAULSE> Pulse_Total)
                        {
                            Pos_Correct = TRUE;
                        }
                    }
                }
            }
        }
    }
}
void MicroMouse_Speed_Change_Calc(void)
{
    long Speed_Max = 0,Pulse = 0,i=0;
    SpeedChangeSys[0].Speed_Start = 0;
    switch((int)fabs(SpurtStep[0].nextrad))
    {
    case 45:
        SpeedChangeSys[0].Speed_End	 = 110;
        break;
    case 90:
        SpeedChangeSys[0].Speed_End	 = 110;
        break;
    case 135:
        SpeedChangeSys[0].Speed_End	 = 110;
        break;
    case 180:
        SpeedChangeSys[0].Speed_End	 = 110;
        break;
    }
    Pulse =  SpurtStep[0].turnpulse -  SpurtStep[0].pulsectl - MICROMOUSE_SPURT_BEGIN_ADD_PAULSE-MICROMOUSE_SPURT_DEC_FINSH_PAULSE;
    if(Pulse > MICROMOUSE_SPURT_ADD_MIN_PAULSE)
    {
        Speed_Max = MICROMOUSE_SPURT_MAX_SPEED;
        if(Speed_Max < MICROMOUSE_SPURT_MAX_SPEED)
        {
            SpeedChangeSys[0].Speed_Max	= Speed_Max;
            SpeedChangeSys[0].PulseCtl_Dec	= 0;
        }
        else
        {
            SpeedChangeSys[0].Speed_Max		= MICROMOUSE_SPURT_MAX_SPEED;
            Pulse = SpurtStep[0].turnpulse - MICROMOUSE_SPURT_DEC_FINSH_PAULSE - (MICROMOUSE_SPURT_MAX_SPEED*MICROMOUSE_SPURT_MAX_SPEED - SpeedChangeSys[0].Speed_End*SpeedChangeSys[0].Speed_End)/2;
            SpeedChangeSys[0].PulseCtl_Dec	= Pulse;
        }
    }
    else
    {
        SpeedChangeSys[0].Speed_Max	= SpeedChangeSys[0].Speed_End;
        SpeedChangeSys[0].PulseCtl_Dec	= 0;
    }
    for(i = 1; i<Step; i++)
    {
        switch((int)fabs(SpurtStep[i-1].nextrad))
        {
        case 45:
            SpeedChangeSys[i].Speed_Start	 = 110;
            break;
        case 90:
            if((int)fabs(SpurtStep[i-1].rad)%90 == 0)
            {
                SpeedChangeSys[i].Speed_Start	 = 110;
            }
            else
            {
                SpeedChangeSys[i].Speed_Start	 = 110;
            }
            break;
        case 135:
            SpeedChangeSys[i].Speed_Start	 = 110;
            break;
        case 180:
            SpeedChangeSys[i].Speed_Start	 = 110;
            break;
        }
        switch((int)fabs(SpurtStep[i].nextrad))
        {
        case 45:
            SpeedChangeSys[i].Speed_End	 = 110;
            break;
        case 90:
            if((int)fabs(SpurtStep[i].rad)%90 == 0)
            {
                SpeedChangeSys[i].Speed_End	 = 110;
            }
            else
            {
                SpeedChangeSys[i].Speed_End	 = 110;
            }
            break;
        case 135:
            SpeedChangeSys[i].Speed_End	 = 110;
            break;
        case 180:
            SpeedChangeSys[i].Speed_End	 = 110;
            break;
        case 0:
            SpeedChangeSys[i].Speed_End	 = MAZE_SEARCH_SPEED;
            break;
        }
        Pulse =  SpurtStep[i].turnpulse -  SpurtStep[i].pulsectl - MICROMOUSE_SPURT_BEGIN_ADD_PAULSE-MICROMOUSE_SPURT_DEC_FINSH_PAULSE;
        if(Pulse > MICROMOUSE_SPURT_ADD_MIN_PAULSE)
        {
            Speed_Max = MICROMOUSE_SPURT_MAX_SPEED;
            if(Speed_Max < MICROMOUSE_SPURT_MAX_SPEED)
            {
                SpeedChangeSys[i].Speed_Max	= Speed_Max;
                SpeedChangeSys[i].PulseCtl_Dec	= 0;
            }
            else
            {
                SpeedChangeSys[i].Speed_Max	= MICROMOUSE_SPURT_MAX_SPEED;
                Pulse = SpurtStep[i].turnpulse - MICROMOUSE_SPURT_DEC_FINSH_PAULSE - (MICROMOUSE_SPURT_MAX_SPEED*MICROMOUSE_SPURT_MAX_SPEED - SpeedChangeSys[i].Speed_End*SpeedChangeSys[i].Speed_End)/2;
                SpeedChangeSys[i].PulseCtl_Dec	= Pulse;
            }
        }
        else
        {
            SpeedChangeSys[i].Speed_Max = SpeedChangeSys[i].Speed_Start;
            if(SpeedChangeSys[i].Speed_Start < SpeedChangeSys[i].Speed_End)
                SpeedChangeSys[i].Speed_Max = SpeedChangeSys[i].Speed_End;
            SpeedChangeSys[i].PulseCtl_Dec	= MICROMOUSE_SPURT_BEGIN_ADD_PAULSE;
        }
    }
}
void MicroMouse_SpurtToGoal(void)
{
    long i;
    GucMousePos    = MIRMOUSE_POS_1;
    GucMouseDir = 0;
    GmcMouseCoor.cX = 0;
    GmcMouseCoor.cY = 0;
    MMMGB(DEST_X,DEST_Y,GucMapStep,GucMapBlock);
    MMSPS();
    MMSP();
    MicroMouse_Speed_Change_Calc();
    WallInf = 0x03;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 500000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 500000;
    GmRight.ulPulseCtr  = 0;
    GmcMouseCoor.cX = 0;
    GmcMouseCoor.cY = 0;
    GucMouseDir = 0;
    GucMouseState = GOAHEAD;
    GucMouseState = GOAHEAD;
    for(i = 0 ; i < Step; i++)
    {
        MicroMouse_SpurtGoAhead(i);
        MicroMouse_SpurtCrosswayChoice(SpurtStep[i].nextrad);
    }
    GmcMouseCoor.cX = DEST_X;
    GmcMouseCoor.cY = DEST_Y;
}
void MicroMouse_SpurtCrosswayChoice(long drad)
{
    if(drad == -45)
    {
        MicroMouse_SpurtTurnRight45();
    }
    else if(drad == 45)
    {
        MicroMouse_SpurtTurnLeft45();
    }
    else if(drad == -90)
    {
        MicroMouse_SpurtTurnRight90();
    }
    else if(drad == 90)
    {
        MicroMouse_SpurtTurnLeft90();
    }
    else if(drad == -135)
    {
        MicroMouse_SpurtTurnRight135();
    }
    else if(drad == 135)
    {
        MicroMouse_SpurtTurnLeft135();
    }
    else if(drad == -180)
    {
        MicroMouse_SpurtTurnRight180();
    }
    else if(drad == 180)
    {
        MicroMouse_SpurtTurnLeft180();
    }
}
void MicroMouse_SpurtTurnRight45(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    if(GucMouseDir%2 == 0)
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TR45_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TR45_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TR45_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle > MICROMOUSE_SPURT_TR45_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                GucMouseDir = (GucMouseDir + 1)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TR45_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
            GmRight.lIdleSpeed = RightSpeed + TempTime;
            TempTime  = Time;
        }
    }
    else
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TR45X_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TR45X_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TR45X_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle > MICROMOUSE_SPURT_TR45X_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                GucMouseDir = (GucMouseDir + 1)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TR45X_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
            GmRight.lIdleSpeed = RightSpeed + TempTime;
            TempTime  = Time;
        }
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
void MicroMouse_SpurtTurnRight90(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    if(GucMouseDir%2 == 0)
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TR90_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TR90_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TR90_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle > MICROMOUSE_SPURT_TR90_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                MMCU04590();
                GucMouseDir = (GucMouseDir + 2)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TR90_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
            GmRight.lIdleSpeed = RightSpeed + TempTime;
            TempTime  = Time;
        }
    }
    else
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TR90V_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TR90V_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TR90V_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle > MICROMOUSE_SPURT_TR90V_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                GucMouseDir = (GucMouseDir + 2)%8;
            }
        }

        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TR90V_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
            GmRight.lIdleSpeed = RightSpeed + TempTime;
            TempTime  = Time;
        }
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
void MicroMouse_SpurtTurnRight135(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    if(GucMouseDir%2 == 0)
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TR135_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TR135_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TR135_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle > MICROMOUSE_SPURT_TR135_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                MMCU04590();
                GucMouseDir = (GucMouseDir + 3)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TR135_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
            GmRight.lIdleSpeed = RightSpeed + TempTime;
            TempTime  = Time;
        }
    }
    else
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TR135Z_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TR135Z_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TR135Z_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle > MICROMOUSE_SPURT_TR135Z_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                MMCU04590();
                GucMouseDir = (GucMouseDir + 3)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TR135Z_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
            GmRight.lIdleSpeed = RightSpeed + TempTime;
            TempTime  = Time;
        }
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
void MicroMouse_SpurtTurnRight180(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    Time = 0;
    Gyro_Angle = 0;
    long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
    while(TempTime < MICROMOUSE_SPURT_TR180_ACC_TIME)
    {
        TempTime  = Time;
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + TempTime;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - TempTime;
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TR180_ACC_TIME;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TR180_ACC_TIME;
    LeftSpeed = GmLeft.lIdleSpeed;
    RightSpeed = GmRight.lIdleSpeed;
    while(1)
    {
        if(Gyro_Angle > MICROMOUSE_SPURT_TR180_GYRO_ANGLE)
        {
            break;
        }
        if(CoorUpdate == FALSE)
        {
            CoorUpdate = TRUE;
            MMCU04590();
            MMCU04590();
            MMCU04590();
            GucMouseDir = (GucMouseDir + 4)%8;
        }
    }
    Time = 0;
    Gyro_Angle = 0;
    TempTime = 0;
    while(TempTime < MICROMOUSE_SPURT_TR180_ACC_TIME)
    {
        GmLeft.lIdleSpeed  = LeftSpeed - TempTime;
        GmRight.lIdleSpeed = RightSpeed + TempTime;
        TempTime  = Time;
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
void MicroMouse_SpurtTurnLeft45(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    if(GucMouseDir%2 == 0)
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TL45_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TL45_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TL45_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle < MICROMOUSE_SPURT_TL45_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                GucMouseDir = (GucMouseDir + 7)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TL45_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
            GmRight.lIdleSpeed = RightSpeed - TempTime;
            TempTime  = Time;
        }
    }
    else
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TL45X_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TL45X_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TL45X_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle < MICROMOUSE_SPURT_TL45X_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                GucMouseDir = (GucMouseDir + 7)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TL45X_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
            GmRight.lIdleSpeed = RightSpeed - TempTime;
            TempTime  = Time;
        }
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
void MicroMouse_SpurtTurnLeft90(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    if(GucMouseDir%2 == 0)
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TL90_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TL90_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TL90_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle < MICROMOUSE_SPURT_TL90_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                MMCU04590();
                GucMouseDir = (GucMouseDir + 6)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TL90_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
            GmRight.lIdleSpeed = RightSpeed - TempTime;
            TempTime  = Time;
        }
    }
    else
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TL90V_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TL90V_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TL90V_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle < MICROMOUSE_SPURT_TL90V_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                GucMouseDir = (GucMouseDir + 6)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TL90V_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
            GmRight.lIdleSpeed = RightSpeed - TempTime;
            TempTime  = Time;
        }
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
void MicroMouse_SpurtTurnLeft135(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    if(GucMouseDir%2 == 0)
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TL135_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TL135_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TL135_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle < MICROMOUSE_SPURT_TL135_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                MMCU04590();
                GucMouseDir = (GucMouseDir + 5)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TL135_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
            GmRight.lIdleSpeed = RightSpeed - TempTime;
            TempTime  = Time;
        }
    }
    else
    {
        Time = 0;
        Gyro_Angle = 0;
        long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
        while(TempTime < MICROMOUSE_SPURT_TL135Z_ACC_TIME)
        {
            TempTime  = Time;
            GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - TempTime;
            GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + TempTime;
        }
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TL135Z_ACC_TIME;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TL135Z_ACC_TIME;
        LeftSpeed = GmLeft.lIdleSpeed;
        RightSpeed = GmRight.lIdleSpeed;
        while(1)
        {
            if(Gyro_Angle < MICROMOUSE_SPURT_TL135Z_GYRO_ANGLE)
            {
                break;
            }
            if(CoorUpdate == FALSE)
            {
                CoorUpdate = TRUE;
                MMCU04590();
                MMCU04590();
                GucMouseDir = (GucMouseDir + 5)%8;
            }
        }
        Time = 0;
        Gyro_Angle = 0;
        TempTime = 0;
        while(TempTime < MICROMOUSE_SPURT_TL135Z_ACC_TIME)
        {
            GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
            GmRight.lIdleSpeed = RightSpeed - TempTime;
            TempTime  = Time;
        }
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
void MicroMouse_SpurtTurnLeft180(void)
{
    BOOL CoorUpdate = FALSE;
    GucMouseState = TURNRIGHT;
    GmLeft.ucState      = MOTORRUN;
    GmLeft.ulPulse      = 70000;
    GmLeft.ulPulseCtr   = 0;
    GmRight.ucState     = MOTORRUN;
    GmRight.ulPulse     = 70000;
    GmRight.ulPulseCtr  = 0;
    DEBUG_LED_ON
    Time = 0;
    Gyro_Angle = 0;
    long TempTime = 0,LeftSpeed = 0,RightSpeed = 0;
    while(TempTime < MICROMOUSE_SPURT_TL180_ACC_TIME)
    {
        TempTime  = Time;
        GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - TempTime;
        GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + TempTime;
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED - MICROMOUSE_SPURT_TL180_ACC_TIME;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED + MICROMOUSE_SPURT_TL180_ACC_TIME;
    LeftSpeed = GmLeft.lIdleSpeed;
    RightSpeed = GmRight.lIdleSpeed;
    while(1)
    {
        if(Gyro_Angle < MICROMOUSE_SPURT_TL180_GYRO_ANGLE)
        {
            break;
        }
        if(CoorUpdate == FALSE)
        {
            CoorUpdate = TRUE;
            MMCU04590();
            MMCU04590();
            MMCU04590();
            GucMouseDir = (GucMouseDir + 4)%8;
        }
    }
    Time = 0;
    Gyro_Angle = 0;
    TempTime = 0;
    while(TempTime < MICROMOUSE_SPURT_TL180_ACC_TIME)
    {
        GmLeft.lIdleSpeed  = LeftSpeed + TempTime;
        GmRight.lIdleSpeed = RightSpeed - TempTime;
        TempTime  = Time;
    }
    GmLeft.lIdleSpeed  = MICROMOUSE_SPURT_SPEED;
    GmRight.lIdleSpeed = MICROMOUSE_SPURT_SPEED;
    DEBUG_LED_OFF
}
#define MICROMOUSE_LEFT_STARTPOS_X      0
#define MICROMOUSE_LEFT_STARTPOS_Y      0
#define MICROMOUSE_RIGHT_STARTPOS_X     15
#define MICROMOUSE_RIGHT_STARTPOS_Y     0
void MicroMouse_HandleStartPos(void)
{
    static BOOL NotSureStartPos = TRUE;
    long i = 0;
    char WallInf;
    WallInf = GucMapBlock[GmcMouseCoor.cX][GmcMouseCoor.cY];

   if((NotSureStartPos)&&((WallInf & 0x02) == 0))    // 右边没墙  0 0 开始的
    {
        NotSureStartPos = FALSE;
        SRC_X = MICROMOUSE_LEFT_STARTPOS_X;
        SRC_Y = MICROMOUSE_LEFT_STARTPOS_Y;
    }
}

void MicroMouse_SearchSpeedChangeCalc(void)
{
    long Speed_Max = 0,Pulse = 0,i=0;
    for(i = 0; i<Step; i++)
    {
        SpeedChangeSys[i].Speed_Start	 = MAZE_SEARCH_SPEED;
        SpeedChangeSys[i].Speed_End	 = MAZE_SEARCH_SPEED;
        Pulse =  SpurtStep[i].turnpulse -  SpurtStep[i].pulsectl - MICROMOUSE_SEARCH_BEGIN_ADD_PAULSE-MICROMOUSE_SEARCH_DEC_FINSH_PAULSE;
        if(Pulse > MICROMOUSE_SEARCH_ADD_MIN_PAULSE)
        {
            Speed_Max = MICROMOUSE_SEARCH_MAX_SPEED;
            if(Speed_Max < MICROMOUSE_SEARCH_MAX_SPEED)
            {
                SpeedChangeSys[i].Speed_Max	= Speed_Max;
                SpeedChangeSys[i].PulseCtl_Dec	= 0;
            }
            else
            {
                SpeedChangeSys[i].Speed_Max	= MICROMOUSE_SEARCH_MAX_SPEED;
                Pulse = (int)(SpurtStep[i].turnpulse - MICROMOUSE_SEARCH_DEC_FINSH_PAULSE - (MICROMOUSE_SEARCH_MAX_SPEED*MICROMOUSE_SEARCH_MAX_SPEED - SpeedChangeSys[i].Speed_End*SpeedChangeSys[i].Speed_End));
                SpeedChangeSys[i].PulseCtl_Dec	= Pulse;
            }
        }
        else
        {
            SpeedChangeSys[i].Speed_Max = 80;
            if(SpeedChangeSys[i].Speed_Start < 80)
                SpeedChangeSys[i].Speed_Max = 80;
            SpeedChangeSys[i].PulseCtl_Dec	= MICROMOUSE_SEARCH_BEGIN_ADD_PAULSE;
        }
    }
}

