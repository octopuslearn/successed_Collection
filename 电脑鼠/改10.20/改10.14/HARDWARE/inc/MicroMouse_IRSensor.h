#ifndef _MIRMOUSE_IRSONSOR_H_
#define _MIRMOUSE_IRSONSOR_H_
#include "stm32f10x.h"
#include "MicroMouse_Define.h"
#include "MicroMouse_GlobalVar.h"
#ifdef DEBUG
#define IR_RF_A             grc_rf_a     
#define IR_RF_B             grc_rf_b     
#define IR_R30_A            grc_r30_a    
#define IR_R30_B            grc_r30_b   
#define IR_L30_A            grc_l30_a
#define IR_L30_B            grc_l30_b
#define IR_LF_A             grc_lf_a         
#define IR_LF_B             grc_lf_b
#define GYRO_OFFSET         grc_gyro_base
#define GYRO_MUL_PARAM      grc_gyro_mul_param       
#endif
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
void Init_IRSensor(void);
#endif

