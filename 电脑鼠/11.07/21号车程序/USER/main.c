#include "MicroMouse_main.h"


/***************************************************************************


天津市启诚伟业科技有限公司

精致II型 竞赛级 智能鼠


***************************************************************************/

void MicroMouse_InitMazeBlockInf()
{
    long i;
    GucMousePos= MIRMOUSE_POS_1;
    GucMouseDir = 0;
    GmcMouseCoor.cX = 0;
    GmcMouseCoor.cY = 0;
    
    for(i=1;i<MAZETYPE-1;i++)
    {
        GucMapBlock[0][i] = 0x08;
        GucMapBlock[i][0] = 0x04;
        GucMapBlock[MAZETYPE-1][i] = 0x02;
        GucMapBlock[i][MAZETYPE-1] = 0x01;
    }
    GucMapBlock[0][0] = 0x1e;
    GucMapBlock[1][0] = 0x0c;
    GucMapBlock[MAZETYPE-1][0] = 0x06;
    GucMapBlock[0][MAZETYPE-1] = 0x09;
    GucMapBlock[MAZETYPE-1][MAZETYPE-1] = 0x03;
}

int  MirMouse_CheckKeyState()
{
    long i = 0;
    int ret = 0;
    for(i=0;i<3000000;i++);
    while(1)
    {

        if(IRDistance[1] < 7)
        {

            Time = 0;
            while(Time < 2);
            if(IRDistance[1] < 7)
            {
                Gyro_Angle = 0;
                GucMouseTask = INITPARAM;
                while(GucMouseTask == INITPARAM);
                MicroMouse_InitMazeBlockInf();
                GucMousePos= MIRMOUSE_POS_2;
                GucMouseTask = MAZESEARCH;
                ret = 1;
                break;
            }
        }
        
        if(IRDistance[2] < 7)
        {

            while(GPIO_ReadInputDataBit(KEY_SPURT_GPIO,KEY_SPURT_PIN) == Bit_RESET);
            GucMouseTask = INITPARAM;
            while(GucMouseTask == INITPARAM);
            MirMouse_ReadMapInf();
            GucMouseTask = SPURT;
            Gyro_Angle = 0;
            break;
        }
        
        if(GPIO_ReadInputDataBit(KEY_SPURT_GPIO,KEY_SPURT_PIN) == Bit_RESET)
        {
            while(GPIO_ReadInputDataBit(KEY_SPURT_GPIO,KEY_SPURT_PIN) == Bit_RESET);
            GucMouseTask = INITPARAM;
            while(GucMouseTask == INITPARAM);
            MirMouse_ReadMapInf();
            GucMouseTask = SPURT;
            Gyro_Angle = 0;
            break;
        }
    }
    return ret;
}
//#define DEBUG_PAULSE      //校准脉冲的参数子函数
//#define DEBUG_IR_GYRO     //调试传感器  红外传感器 和陀螺仪的
//#define DEBUG_MOTER_WIDTH
//#define DEBUG_TURN_RIGHT
//#define DEBUG_TURN_BACK

#ifdef DEBUG_PAULSE 

void main()
{  
    long i;
    long Speed;
    char  StepGoBackAllWall = 0;
    
    RCC_Config();
    InitGlobalVar();
    Init_IRSensor();  
    Init_24L01();
    w24L01_SetTxMode(0);
    Init_KeyBoard();
    Init_Moter_PWM();
    Init_Moter_ENC();
    SysTick_Config(SystemCoreClock/1000);
    
    while(1)
    {  
        Check_PowerVoltage();
        MirMouse_CheckKeyState();
        
        if(GucMouseTask == MAZESEARCH)
        {
            GmLeft.ucState      = MOTORRUN;
            GmLeft.ulPulse      = (18*7*1379);	//1379
            GmLeft.ulPulseCtr   = 0; 
            
            GmRight.ucState     = MOTORRUN;
            GmRight.ulPulse     = (18*7*1379);
            GmRight.ulPulseCtr  = 0;
            GucMouseState = GOAHEAD;
            
            Time = 0;
            while(1)
            {
                Speed = Time ;
                if(Speed < 10)
                {
                    GmLeft.lIdleSpeed  = Speed;
                    GmRight.lIdleSpeed = Speed;
                }
                else
                {
                    GmLeft.lIdleSpeed  = 20;
                    GmRight.lIdleSpeed = 20;
                    break;
                }
            }
            
            while(1)
            {
                if(GmRight.ulPulseCtr > (GmRight.ulPulse - 200))
                {
                    Time = 0;
                    while(Speed<= 20)
                    {
                        GmLeft.lIdleSpeed  = 20 - Speed;
                        GmRight.lIdleSpeed = 20 - Speed;
                        Speed = Time;
                    }
                    break;
                }
            }
        }
    }
}
#elif defined DEBUG_TURN_BACK
extern  float     IRDistance[4];
void main()
{  
    long Speed;
    
    RCC_Config();
    InitGlobalVar();
    Init_IRSensor();  
    Init_24L01();
    w24L01_SetTxMode(0);
    Init_KeyBoard();
    Init_Moter_PWM();
    Init_Moter_ENC();
    SysTick_Config(SystemCoreClock/1000);
    int SearchSpeed = 80;
    int AccTime = 40;
    float AngleBack = 165;
    while(1)
    {  
        Check_PowerVoltage();
        MirMouse_CheckKeyState();
        
        if(GucMouseTask == MAZESEARCH)
        {
            GmLeft.ucState      = MOTORRUN;
            GmLeft.ulPulse      = (18*3*1377);
            GmLeft.ulPulseCtr   = 0; 
            
            GmRight.ucState     = MOTORRUN;
            GmRight.ulPulse     = (18*3*1377);
            GmRight.ulPulseCtr  = 0;
            GucMouseState = GOAHEAD;
            
            Time = 0;
            while(1)
            {
                Speed = Time ;
                if(Speed < SearchSpeed)
                {
                    GmLeft.lIdleSpeed  = Speed;
                    GmRight.lIdleSpeed = Speed;
                }
                else
                {
                    GmLeft.lIdleSpeed  = SearchSpeed;
                    GmRight.lIdleSpeed = SearchSpeed;
                    break;
                }
            }
        }
        
        Time = 0;
        while(Time < 200);
        
        Time = 0;
        while(1)
        {
            Speed = Time ;
            if(Speed < SearchSpeed)
            {
                GmLeft.lIdleSpeed  = SearchSpeed - Speed;
                GmRight.lIdleSpeed = SearchSpeed - Speed;
            }
            else
            {
                GmLeft.lIdleSpeed  = 0;
                GmRight.lIdleSpeed = 0;
                break;
            }
        }
        Time = 0;
        while(Time < 20);
        
        Gyro_Angle = 0;
        while(1)
        {
            Speed = Time ;
            if(Speed < AccTime)
            {
                GmLeft.lIdleSpeed  = Speed;
                GmRight.lIdleSpeed = - Speed;
            }
            else
            {
                GmLeft.lIdleSpeed  = AccTime;
                GmRight.lIdleSpeed = -AccTime;
                break;
            }
        }
        
        while(Gyro_Angle < AngleBack);
        
        Time = 0;
        while(1)
        {
            Speed = Time ;
            if(Speed < AccTime)
            {
                GmLeft.lIdleSpeed  = AccTime - Speed;
                GmRight.lIdleSpeed = -AccTime + Speed;
            }
            else
            {
                GmLeft.lIdleSpeed  = 0;
                GmRight.lIdleSpeed = 0;
                break;
            }
        }
        
        Time = 0;
        while(Time < 20);
        Time =0;
        while(1)
        {
            Speed = Time ;
            if(Speed < SearchSpeed)
            {
                GmLeft.lIdleSpeed  = Speed;
                GmRight.lIdleSpeed = Speed;
            }
            else
            {
                GmLeft.lIdleSpeed  = SearchSpeed;
                GmRight.lIdleSpeed = SearchSpeed;
                break;
            }
        }
        DEBUG_LED_ON
            break;
        
        
        
    }
}
#elif defined DEBUG_TURN_RIGHT
extern  float     IRDistance[4];
void main()
{  
    long Speed;
    
    RCC_Config();
    InitGlobalVar();
    Init_IRSensor();  
    Init_24L01();
    w24L01_SetTxMode(0);
    Init_KeyBoard();
    Init_Moter_PWM();
    Init_Moter_ENC();
    SysTick_Config(SystemCoreClock/1000);
    int SearchSpeed = 80;
    int AccTime = 43;
    float AngleRight = 71.5;
    float AngleLeft = 71.5;
    int  TurnRight = 0;
    float   AheadDistRight = 9;
    float   AheadDistLeft = 9;
    while(1)
    {  
        Check_PowerVoltage();
        TurnRight = MirMouse_CheckKeyState();
        
        if(GucMouseTask == MAZESEARCH)
        {
            GmLeft.ucState      = MOTORRUN;
            GmLeft.ulPulse      = (18*3*1377);
            GmLeft.ulPulseCtr   = 0; 
            
            GmRight.ucState     = MOTORRUN;
            GmRight.ulPulse     = (18*3*1377);
            GmRight.ulPulseCtr  = 0;
            GucMouseState = GOAHEAD;
            
            Time = 0;
            while(1)
            {
                Speed = Time ;
                if(Speed < SearchSpeed)
                {
                    GmLeft.lIdleSpeed  = Speed;
                    GmRight.lIdleSpeed = Speed;
                }
                else
                {
                    GmLeft.lIdleSpeed  = SearchSpeed;
                    GmRight.lIdleSpeed = SearchSpeed;
                    break;
                }
            }
        }
        while(1)
        {
            if(TurnRight == 1)
            {
                if(IRDistance[1] > 16)
                {
                    GmLeft.ulPulseCtr = 0;
                    GmRight.ulPulseCtr  = 0;
                    DEBUG_LED_ON 
                        while((GmRight.ulPulseCtr + GmLeft.ulPulseCtr) < (AheadDistRight*1377*2));
                    Time = 0;
                    DEBUG_LED_OFF  
                        Gyro_Angle = 0;
                    while(1)
                    {
                        Speed = Time ;
                        if(Speed < AccTime)
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed + Speed;
                            GmRight.lIdleSpeed = SearchSpeed - Speed;
                        }
                        else
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed + AccTime;
                            GmRight.lIdleSpeed = SearchSpeed - AccTime;
                            break;
                        }
                    }
                    DEBUG_LED_ON 
                        while(Gyro_Angle < AngleRight);
                    DEBUG_LED_OFF  
                        Time =0;
                    while(1)
                    {
                        Speed = Time ;
                        if(Speed < AccTime)
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed - Speed + AccTime;
                            GmRight.lIdleSpeed = SearchSpeed + Speed - AccTime;
                        }
                        else
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed;
                            GmRight.lIdleSpeed = SearchSpeed;
                            break;
                        }
                    }
                    DEBUG_LED_ON 
                        break;
                } 
            }
            else
            {
                
                if(IRDistance[2] > 16)
                {
                    GmLeft.ulPulseCtr = 0;
                    GmRight.ulPulseCtr  = 0;
                    DEBUG_LED_ON 
                        while((GmRight.ulPulseCtr + GmLeft.ulPulseCtr) < (AheadDistLeft*1377*2));
                    Time = 0;
                    DEBUG_LED_OFF  
                        Gyro_Angle = 0;
                    while(1)
                    {
                        Speed = Time ;
                        if(Speed < AccTime)
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed - Speed;
                            GmRight.lIdleSpeed = SearchSpeed + Speed;
                        }
                        else
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed - AccTime;
                            GmRight.lIdleSpeed = SearchSpeed + AccTime;
                            break;
                        }
                    }
                    DEBUG_LED_ON  
                        while(Gyro_Angle > -AngleLeft);
                    DEBUG_LED_OFF  
                        Time =0;
                    while(1)
                    {
                        Speed = Time ;
                        if(Speed < AccTime)
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed + Speed - AccTime;
                            GmRight.lIdleSpeed = SearchSpeed - Speed + AccTime;
                        }
                        else
                        {
                            GmLeft.lIdleSpeed  = SearchSpeed;
                            GmRight.lIdleSpeed = SearchSpeed;
                            break;
                        }
                    }
                    DEBUG_LED_ON 
                        break;
                } 
            }
        }
    }
}

#elif defined DEBUG_MOTER_WIDTH
void main()
{  
    long i;
    long Speed;
    char  StepGoBackAllWall = 0;
    
    RCC_Config();
    InitGlobalVar();
    Init_IRSensor();  
    Init_24L01();
    w24L01_SetTxMode(0);
    Init_KeyBoard();
    Init_Moter_PWM();
    Init_Moter_ENC();
    SysTick_Config(SystemCoreClock/1000);
    
    while(1)
    {  
        Check_PowerVoltage();
        MirMouse_CheckKeyState();
        
        if(GucMouseTask == MAZESEARCH)
        {
            GmLeft.ucState      = MOTORRUN;
            GmLeft.ulPulse      = (50.24*5*1377);
            GmLeft.ulPulseCtr   = 0; 
            
            GmRight.ucState     = MOTORRUN;
            GmRight.ulPulse     = (50.24*4*1377);
            GmRight.ulPulseCtr  = 0;
            GucMouseState = GOAHEAD;
            
            Time = 0;
            while(1)
            {
                Speed = Time ;
                if(Speed < 50)
                {
                    GmLeft.lIdleSpeed  = Speed;
                    GmRight.lIdleSpeed = Speed;
                }
                else
                {
                    GmLeft.lIdleSpeed  = 50;
                    GmRight.lIdleSpeed = 50;
                    break;
                }
            }
        }
        
        Time = 0;
        while(Time < 100);
        
        Time = 0;
        while(1)
        {
            Speed = Time ;
            if(Speed < 15)
            {
                GmLeft.lIdleSpeed  = 50 - Speed;
                GmRight.lIdleSpeed = 50 + Speed;
            }
            else
            {
                GmLeft.lIdleSpeed  = 35;
                GmRight.lIdleSpeed = 65;
                break;
            }
        }
    }
}

#elif defined DEBUG_IR_GYRO
void main()
{  
    long i;
    long Speed;
    char  StepGoBackAllWall = 0;
    
    RCC_Config();
    InitGlobalVar();
    Init_IRSensor();  
    Init_24L01();
    w24L01_SetTxMode(0);
    Init_KeyBoard();
    Init_Moter_PWM();
    Init_Moter_ENC();
    SysTick_Config(SystemCoreClock/1000);
    
    while(1)
    {  
//        Check_PowerVoltage();
//        MirMouse_CheckKeyState();
    }
}
#else
int main()
{  
    long i;
    long Speed;
    char  StepGoBackAllWall = 0;
    int   Debug = 0;
    RCC_Config();
    InitGlobalVar();
    Init_IRSensor();  
    Init_24L01();
    w24L01_SetTxMode(0);
    Init_KeyBoard();
    Init_Moter_PWM();
    Init_Moter_ENC();
    SysTick_Config(SystemCoreClock/1000);
    
    while(1)
    {  
        Check_PowerVoltage();
        MirMouse_CheckKeyState();
        
        if(GucMouseTask == MAZESEARCH)
        {
            GmLeft.ucState      = MOTORRUN;
            GmLeft.ulPulse      = 50000;
            GmLeft.ulPulseCtr   = 0; 
            
            GmRight.ucState     = MOTORRUN;
            GmRight.ulPulse     = 50000;
            GmRight.ulPulseCtr  = 0;
            GucMouseState = GOAHEAD;
            
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
        
        
        while(1)
        {
            if(GucMouseTask == MAZESEARCH)
            {
                switch(GucMousePos)
                {
                case MIRMOUSE_POS_1:
                    MicroMouse_GoAhead(18);
                    break; 
                case MIRMOUSE_POS_5:
                    MicroMouse_GoAhead(MICROMOUSE_TRUN_DIST_GO_AHEAD);
                    break; 
                case MIRMOUSE_POS_2:
                    MicroMouse_GoAhead(14);
                    break; 
                }
                if(Debug == 0)
                    DEBUG_LED_ON
                else
                    DEBUG_LED_OFF
                Debug = (Debug + 1)%2;
                MicroMouse_CrosswayChoice(CrossWayInf);/**/
                
                if(CrossWayInf == 10)
                { 
                    GucMouseTask = SPURT;
                    GucMouseState = GOAHEAD;
                    SpeedChangeTime = 0;
                    while(SpeedChangeTime < 150);
                    GucMouseState = TURNBACK;
					MirMouse_WriteMapInf();
                    MicroMouse_TurnBack(FALSE,TRUE);
                    for(i = 0;i<3000000;i++);    
                }
            }
            else if(GucMouseTask == SPURT)
            {
                MicroMouse_SpurtToGoal();
                if(GoToStart == TRUE)
                {
                    GucMouseTask = GOBACK;
                } 
                else
                {
                    MMMGB(SRC_X,SRC_Y,GucMapStep,GucMapBlock);
                    StepGoBackAllWall = GucMapStep[DEST_X][DEST_Y];
                    MMMSE(SRC_X,SRC_Y,GucMapStep,GucMapBlock);
                    if(StepGoBackAllWall == GucMapStep[DEST_X][DEST_Y])
                    {
                        MMMGB(SRC_X,SRC_Y,GucMapStep,GucMapBlock);
                        GoToStart = TRUE;
                    }
                    
                    CrossWayInf = MMSEPC();
                    
                    MicroMouse_CrosswayChoice(CrossWayInf);
                    
                    if(CrossWayInf == 10)
                    { 
                        GucMouseTask = SPURT;
                        GucMouseState = GOAHEAD;
                        Time = 0;
                        while(Time < 180);
                        GucMouseState = TURNBACK;
												MirMouse_WriteMapInf();
                        MicroMouse_TurnBack(FALSE,TRUE);
                        for(i = 0;i<3000000;i++);    
                    }
                    else
                    {
                        GucMouseTask = MAZESEARCH;
                    }
                }
            }
            else if(GucMouseTask == GOBACK)
            {
                MMMGB(SRC_X,SRC_Y,GucMapStep,GucMapBlock);
                MMSPP(SRC_X,SRC_Y,0);
                
                MicroMouse_SearchObjectTo();
                
                GucMouseState = GOAHEAD;
                Time = 0;
                while(Time < 150);

                GmLeft.ucState      = MOTORSTOP;
                GmRight.ucState      = MOTORSTOP;
                
                while(1);
            }
        }
    }
}
#endif 



#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop */
    while (1)
    {
    }
}

#endif

