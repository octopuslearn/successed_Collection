#ifndef _BALANCED_h
#define _BALANCED_h

#include "MsTimer2.h"
#include "KalmanFilter.h"


#define ANGLE_MIN -27
#define ANGLE_MAX 27
#define EXCESSIVE_ANGLE_TILT (kalmanfilter.angle < ANGLE_MIN || ANGLE_MAX < kalmanfilter.angle)
#define PICKED_UP (kalmanfilter.angle6 < -10 || 22 < kalmanfilter.angle6)
#define TIMERE 5


#define FORWARD 'F'
#define BACK    'B'
#define LEFT    'L'
#define RIGHT   'R'
#define STOP    'S'
extern char direction[5];

extern         int16_t ax, ay, az, gx, gy, gz;
extern         float dt, Q_angle, Q_gyro, R_angle, C_0, K1;

/*Speed value*/
extern           double pwm_left;
extern           double pwm_right;
extern           int encoder_left_pulse_num_speed;
extern           int encoder_right_pulse_num_speed;

/*Cnt*/
extern          int Balanced_interrupt_cnt;

/*PID parameter*/
         /*PD_VerticalRing*/
extern           double kp_balance, kd_balance;
         /*PI_SpeedRing*/
extern           double kp_speed, ki_speed;
         /*PI_SteeringRing*/
extern           double kp_turn, kd_turn;

extern            double speed_filter;
extern            double speed_filter_old;
extern            double car_speed_integeral;
extern            double balance_control_output;
extern            double speed_control_output;
extern            double rotation_control_output;
extern            int setting_turn_speed;
extern            int setting_car_speed;


/*以下，函数声明*/
void Mpu6050_DataProcessing();//获取陀螺仪数据
void Balanced_Get_EncoderSpeed();//获取编码器速度
void Mpu6050_DataProcessing();//获取MPU6050数据
void Balanced_PD_VerticalRing();//垂直环PD控制
void Balanced_PI_SpeedRing();//速度控制速
void Balanced_PI_SteeringRing();//转向控制
void Balanced_Total_Control();//将三个环的控制输出量相加


void Timer2_init(int time);/*初始化定时器2中断*/
void Mpu6050_init();//MPU6050初始化
void Mpu6050_Mpu6050();//卡尔曼滤波器参数设置
void Balanced_Balanced();/*最后我们可以修改参数根据实际情况以便获得更好的平衡效果。*/

void Timer2_interrupt();//定时器2中断/*平衡总控制*/


void Balanced_Right(int speed);
void Balanced_Left(int speed);
void Balanced_Back(int speed);
void Balanced_Forward(int speed);
void Balanced_Stop();
void Balanced_Motion_Control(char direction);
/*以下，函数声明*/


#endif
