#include "MicroMouse_GlobalVar.h"
#ifdef DEBUG
char grc_var[GRC_VAR_SIZE];
char grc_frame_inf[GRC_FRAME_SIZE] =
{
    10,
    34,30,32,28,66,64,62,60,68,76
};
unsigned short grc_var_index[GRC_VAR_INDEX_SIZE] =
{
    GRC_MOTER_PID_P_OFFSET,GRC_MOTER_PID_P_SIZE,
    GRC_MOTER_PID_I_OFFSET,GRC_MOTER_PID_I_SIZE,
    GRC_MOTER_PID_D_OFFSET,GRC_MOTER_PID_D_SIZE,
    GRC_IRSensor_PID_P_OFFSET,GRC_IRSensor_PID_P_SIZE,
    GRC_IRSensor_PID_I_OFFSET,GRC_IRSensor_PID_I_SIZE,
    GRC_IRSensor_PID_D_OFFSET,GRC_IRSensor_PID_D_SIZE,
    GRC_IRSensor_RF_A_OFFSET,GRC_IRSensor_RF_A_SIZE,
    GRC_IRSensor_RF_B_OFFSET,GRC_IRSensor_RF_B_SIZE,
    GRC_IRSensor_L30_A_OFFSET,GRC_IRSensor_L30_A_SIZE,
    GRC_IRSensor_L30_B_OFFSET,GRC_IRSensor_L30_B_SIZE,
    GRC_IRSensor_R30_A_OFFSET,GRC_IRSensor_R30_A_SIZE,
    GRC_IRSensor_R30_B_OFFSET,GRC_IRSensor_R30_B_SIZE,
    GRC_IRSensor_LF_A_OFFSET,GRC_IRSensor_LF_A_SIZE,
    GRC_IRSensor_LF_B_OFFSET,GRC_IRSensor_LF_B_SIZE,
    GRC_IRSensor_RF_DIST_OFFSET,GRC_IRSensor_RF_DIST_SIZE,
    GRC_IRSensor_L30_DIST_OFFSET,GRC_IRSensor_L30_DIST_SIZE,
    GRC_IRSensor_R30_DIST_OFFSET,GRC_IRSensor_R30_DIST_SIZE,
    GRC_IRSensor_LF_DIST_OFFSET,GRC_IRSensor_LF_DIST_SIZE,
    GRC_L_MOTER_STATE_OFFSET,GRC_L_MOTER_STATE_SIZE,
    GRC_L_MOTER_DIR_OFFSET,GRC_L_MOTER_DIR_SIZE,
    GRC_L_MOTER_PULSE_OFFSET,GRC_L_MOTER_PULSE_SIZE,
    GRC_L_MOTER_PULSECTRL_OFFSET,GRC_L_MOTER_PULSECTRL_SIZE,
    GRC_L_MOTER_INFACT_SPEED_OFFSET,GRC_L_MOTER_INFACT_SPEED_SIZE,
    GRC_L_MOTER_IDLE_SPEED_OFFSET,GRC_L_MOTER_IDLE_SPEED_SIZE,
    GRC_R_MOTER_STATE_OFFSET,GRC_R_MOTER_STATE_SIZE,
    GRC_R_MOTER_DIR_OFFSET,GRC_R_MOTER_DIR_SIZE,
    GRC_R_MOTER_PULSE_OFFSET,GRC_R_MOTER_PULSE_SIZE,
    GRC_R_MOTER_PULSECTRL_OFFSET,GRC_R_MOTER_PULSECTRL_SIZE,
    GRC_R_MOTER_INFACT_SPEED_OFFSET,GRC_R_MOTER_INFACT_SPEED_SIZE,
    GRC_R_MOTER_IDLE_SPEED_OFFSET,GRC_R_MOTER_IDLE_SPEED_SIZE,
    GRC_IRSensor_ADCVALUE_RF_OFFSET,GRC_IRSensor_ADCVALUE_RF_SIZE,
    GRC_IRSensor_ADCVALUE_R30_OFFSET,GRC_IRSensor_ADCVALUE_R30_SIZE,
    GRC_IRSensor_ADCVALUE_L30_OFFSET,GRC_IRSensor_ADCVALUE_L30_SIZE,
    GRC_IRSensor_ADCVALUE_LF_OFFSET,GRC_IRSensor_ADCVALUE_LF_SIZE,
    GRC_GYRO_ADCVALUE_OFFSET,GRC_GYRO_ADCVALUE_SIZE,
    GRC_VOLTAGE_ADCVALUE_OFFSET,GRC_VOLTAGE_ADCVALUE_SIZE,
    GRC_GYRO_BASE_VALUE_OFFSET,GRC_GYRO_BASE_SIZE,
    GRC_GYRO_MUL_PARAM_OFFSET,GRC_GYRO_MUL_PARAM_SIZE,
    GRC_GYRO_ANGLE_OFFSET,GRC_GYRO_ANGLE_SIZE,
};
#endif
void InitGlobalVar(void)
{
#ifdef DEBUG
    grc_moter_p = 0.5;
    grc_moter_i = 0;
    grc_moter_d = 10;
    grc_irsensor_p = 15;
    grc_irsensor_i = 0;
    grc_irsensor_d = 0;
    grc_rf_dist = 28;
    grc_l30_dist = 29;
    grc_r30_dist = 30;
    grc_lf_dist = 31;
    grc_irsensor_rf_adc = 12;
    grc_irsensor_r30_adc = 13;
    grc_irsensor_L30_adc = 14;
    grc_irsensor_lf_adc = 15;
    grc_gyro_adc = 16;
    grc_voltage_adc = 17;

//    grc_l30_a =-0.5729;
//    grc_l30_b =0.6201;
//    grc_r30_a =-0.5133;
//    grc_r30_b =0.8840;
//    grc_lf_a =-0.5749; 
//    grc_lf_b =0.8385;  
//    grc_rf_a =-0.5366;
//    grc_rf_b =1.1193;
	grc_l30_a =		-0.5652;
    grc_l30_b =		0.5895;
    grc_r30_a =		-0.5063;
    grc_r30_b =		0.8801;
    grc_lf_a =	-0.5651; 
    grc_lf_b =	0.8403; 
    grc_rf_a =	-0.5833;
    grc_rf_b =	1.0174;
	
	
    grc_gyro_base =7870;//8140;
    grc_gyro_mul_param=0.1000990;

#endif
}