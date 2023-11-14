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

/*之前測的好的*/
//    grc_l30_a =-0.5918;  //-0.5758
//    grc_l30_b =0.5212; //
//    grc_r30_a =-0.4662;//-0.5049
//    grc_r30_b =0.9794;//0.9150
//    grc_lf_a =-0.5727; //-0.5748
//    grc_lf_b =0.8436;  //0.8264
//    grc_rf_a =-0.5532;//0.5452
//    grc_rf_b =1.0597;//1.0855	
/*第三組*/
//    grc_l30_a =-0.5852;//-0.5544;
//    grc_l30_b =0.5401;//0.6094;
//    grc_r30_a =-0.5095;//-0.5083;
//    grc_r30_b =0.8726;//0.874;
//    grc_lf_a =-0.5698;//-0.5762; 
//    grc_lf_b =0.8206; //0.8198;  
//    grc_rf_a =-0.5572;//-0.5412;
//    grc_rf_b =1.0581;//1.079;


/*第二組*/
    grc_l30_a =		-0.5762;
    grc_l30_b =		0.5552;
    grc_r30_a =		-0.5071;
    grc_r30_b =		0.8747;
    grc_lf_a =	-0.5782; 
    grc_lf_b =	0.8031; 
    grc_rf_a =	-0.5709;
    grc_rf_b =	1.0362;

/*第一組*/
//    grc_l30_a =-0.5544;
//    grc_l30_b =0.6094;
//    grc_r30_a =-0.5083;
//    grc_r30_b =0.874;
//    grc_lf_a =-0.5762; 
//    grc_lf_b =0.8198;  
//    grc_rf_a =-0.5412;
//    grc_rf_b =1.079;


/*法2，第一組*/
//    grc_l30_a =		-0.5698;
//    grc_l30_b =		0.5706;
//    grc_r30_a =		-0.5245;
//    grc_r30_b =		0.8325;
//	grc_lf_a =	-0.5558; 
//    grc_lf_b =	0.8539; 
//    grc_rf_a =	-0.5486;
//    grc_rf_b =	1.06;
	
	
	
//    grc_l30_a =	-0.5468	;
//    grc_l30_b =	0.6336	;
//    grc_r30_a =	-0.5053	;
//    grc_r30_b =	0.8840	;
//    grc_lf_a =	-0.5724; 
//    grc_lf_b =	0.8243; 
//    grc_rf_a =	-0.5399;
//    grc_rf_b =	1.0819;	
	
	
	
//	  grc_l30_a =		-0.5668;
//    grc_l30_b =		0.5826;
//    grc_r30_a =		-0.5065;
//    grc_r30_b =		0.8332;
//    grc_lf_a =	-0.5620; 
//    grc_lf_b =	0.8622; 
//    grc_rf_a =	-0.5419;
//    grc_rf_b =	1.0752;



//	grc_l30_a =		-0.5596;
//    grc_l30_b =		0.5941;
//    grc_r30_a =		-0.5051;
//    grc_r30_b =		0.9041;
//    grc_lf_a =	-0.5704; 
//    grc_lf_b =	0.821; 
//    grc_rf_a =	-0.5288;
//    grc_rf_b =	1.097;


//	grc_l30_a =	-0.5737	;
//    grc_l30_b =	0.5631	;
//    grc_r30_a =	-0.5065	;
//    grc_r30_b =	0.8871	;
//    grc_lf_a =	-0.5684; 
//    grc_lf_b =	0.8264; 
//    grc_rf_a =	-0.5287;
//    grc_rf_b =	1.0995;
	
	
	
//	grc_l30_a =	-0.551	;
//    grc_l30_b =	0.6139	;
//    grc_r30_a =	-0.5022	;
//    grc_r30_b =	0.893	;
//    grc_lf_a =	-0.5697; 
//    grc_lf_b =	0.8342; 
//    grc_rf_a =	-0.5286;
//    grc_rf_b =	1.1154;

    grc_gyro_base =8140;
    grc_gyro_mul_param=0.2;//0.101990;

#endif
}