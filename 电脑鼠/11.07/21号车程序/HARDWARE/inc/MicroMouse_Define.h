#ifndef _MIRMOUSE_DEFINE_H_
#define _MIRMOUSE_DEFINE_H_
#define TURN_RADIU                  4.66
#define MIRMOUSE_WIDTH              6.68
#define MIRMOUSE_POS_1	0
#define MIRMOUSE_POS_2	1
#define MIRMOUSE_POS_3	2
#define MIRMOUSE_POS_4	3
#define MIRMOUSE_POS_5	4
#define MIRMOUSE_POS_6	5
#define MIRMOUSE_POS_7	6
#define LEFT              0                                          
#define FRONTL            1                                          
#define FRONT             2                                          
#define FRONTR            3                                          
#define RIGHT             4                                           
#define STOP              0                                           
#define GOAHEAD           1                                           
#define TURNLEFT          3                                           
#define TURNRIGHT         4                                           
#define TURNBACK          5                                          
#define ACC               7
#define SIMPLEAHEAD		    8
#define MOTORSTOP         0                                           
#define WAITONESTEP       1                                           
#define MOTORRUN          2                                           
#define MOTORGOAHEAD      0                                           
#define MOTORGOBACK       1                                           
#define MAZETYPE          16
#define SURESRC           0
#define MAZESEARCH        1
#define SPURT             2
#define GOBACK            3
#define CHECKKEY          4 
#define INITPARAM         5
typedef enum BOOL
{
FALSE = 0,
TRUE  = !FALSE
}BOOL;
typedef struct  motor {
signed char       ucState;                                                    
signed char       ucDir;                                                      
signed long       ulPulse;                                                    
signed long       ulPulseCtr;                                                
signed long       lInfactSpeed;                                              
signed long       lIdleSpeed;

}MOTER;
typedef struct SPEEDCHANGESYS
{
long			Speed_Max;
long			PulseCtl_Dec;
unsigned char	Speed_Start;
unsigned char	Speed_End;
}SPEEDCHANGESYS;
typedef struct mazecoor
{
char cX;
char cY;
}MAZECOOR;
typedef struct SPURTSTEP
{
float dist;
long rad;
long nextrad;
long turnpulse;
long pulsectl;
char NumCoorUpdate;
}SPURTSTEP;
#endif

