#include "Balanced.h"
/*加*/#include "I2Cdev.h"
#include "Motor.h"
//#include "MPU6050.h"
#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"
/*以下，创建对象*/
MPU6050 Y_MPU6050;
KalmanFilter kalmanfilter;
/*以上，创建对象*/

/*以下，平衡所需变量*/
/*Speed value*/
          double pwm_left;
          double pwm_right;
          int encoder_left_pulse_num_speed;
          int encoder_right_pulse_num_speed;
/*Cnt*/
          int Balanced_interrupt_cnt;//累计

/*PID parameter*/
         /*PD_VerticalRing*/
          double kp_balance, kd_balance;
         /*PI_SpeedRing*/
          double kp_speed, ki_speed;
         /*PI_SteeringRing*/
          double kp_turn, kd_turn;

          double speed_filter;
          double speed_filter_old;
          double car_speed_integeral;
          double balance_control_output;
          double speed_control_output;
          double rotation_control_output;
          int setting_turn_speed;
          int setting_car_speed;

         int16_t ax, ay, az, gx, gy, gz;
         float dt, Q_angle, Q_gyro, R_angle, C_0, K1;
/*以上，平衡所需变量*/

/*加*/float kalmanfilter_angle;

char direction[5]={
  FORWARD,BACK,LEFT,RIGHT,STOP
};


extern unsigned long Motor_encoder_count_right_a;//右电机脉冲
extern unsigned long Motor_encoder_count_left_a;//左电机脉冲


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

/*初始化定时器2中断*/
void Timer2_init(int time)
{
  MsTimer2::set(time,Timer2_interrupt);
  MsTimer2::start();
}

/*MPU6050初始化*/
void Mpu6050_init()//MPU6050初始化
{
   Wire.begin();//IIC初始化        
   Y_MPU6050.initialize();//MPU6050初始化    
 }

/*卡尔曼滤波器参数设置*/
void Mpu6050_Mpu6050()//卡尔曼滤波器参数设置
{
    dt = 0.005, Q_angle = 0.001, Q_gyro = 0.005, R_angle = 0.5, C_0 = 1, K1 = 0.05;
}

/*平衡总控制*/
void Timer2_interrupt()//定时器2中断
{ //每5ms获取陀螺仪数据，进行垂直控制
  sei();//enable the global interrupt//使能全局中断
  Balanced_Get_EncoderSpeed();//平衡获取编码器速度
  Mpu6050_DataProcessing();//获取MPU6050数据
 /*加*/ kalmanfilter_angle = kalmanfilter.angle;
  Balanced_PD_VerticalRing();//垂直控制
  Balanced_interrupt_cnt++;
  if(Balanced_interrupt_cnt > 8)//每40ms进行速度控制
  {
    Balanced_interrupt_cnt=0;
    Balanced_PI_SpeedRing();//速度控制
    Balanced_PI_SteeringRing();//转向控制
   }
  Balanced_Total_Control();//(翻译)平衡总体控制
}

/*最后我们可以修改参数根据实际情况以便获得更好的平衡效果。*/
void Balanced_Balanced()/*最后我们可以修改参数根据实际情况以便获得更好的平衡效果。*/
{
  kp_balance = 55, kd_balance = 0.75;
  kp_speed = 10, ki_speed = 0.26;
  kp_turn = 2.5, kd_turn = 0.5;
}



void Balanced_Get_EncoderSpeed()//获取编码器速度
{
  encoder_left_pulse_num_speed += pwm_left < 0 ? (-Motor_encoder_count_left_a) : Motor_encoder_count_left_a;
  encoder_right_pulse_num_speed += pwm_right < 0 ? (-Motor_encoder_count_right_a) :Motor_encoder_count_right_a;

  Motor_encoder_count_left_a=0;
  Motor_encoder_count_right_a=0;
}


/*
*虽然在上述垂直环的控制下已经能够保持平衡，但由于精度问题，传感器实际测量的角度总是偏离车辆模型的角度，因此车辆模
*型实际上并不垂直于车辆模型。地面，但有一个倾斜角度，在重力的作用下，车辆模型会朝着倾斜的方向加速。所以我们需要引入
*一个速度环来维持速度稳定，让平衡车能够保持静止并平稳移动
*/
void Balanced_PI_SpeedRing()//速度环控制
{
   double car_speed=(encoder_left_pulse_num_speed + encoder_right_pulse_num_speed) * 0.5;
   encoder_left_pulse_num_speed = 0;
   encoder_right_pulse_num_speed = 0;
   speed_filter = speed_filter_old * 0.7 + car_speed * 0.3;//速度过滤器 = 旧速度过滤器 * 0.7 + 汽车速度 * 0.3
   /*进行低通滤波，减缓速度差，扰乱立柱。*/
   speed_filter_old = speed_filter;
   car_speed_integeral += speed_filter;
   car_speed_integeral += -setting_car_speed;
   /*积分速度*/ 
   /*限制积分值来确定速度的最大上限。*/
   car_speed_integeral = constrain(car_speed_integeral, -3000, 3000);//汽车速度整数 = 约束(汽车速度整数, -3000, 3000);


   speed_control_output = -kp_speed * speed_filter - ki_speed * car_speed_integeral;//汽车速度整数
}

void Balanced_PD_VerticalRing()//垂直环PD控制
{
  balance_control_output= kp_balance * (kalmanfilter.angle - 0) + kd_balance * (kalmanfilter.Gyro_x - 0);
}

/*转向环的实现主要是获取数据
*z轴陀螺仪作为转向速度偏差进行P控制。 目标是保持设定的转向速度
*价值。
*/
void Balanced_PI_SteeringRing()//转向控制
{  
   rotation_control_output = setting_turn_speed + kd_turn * kalmanfilter.Gyro_z;////control with Z-axis gyroscope
}


/*MPU6050通过卡尔曼滤波器获得小车的倾斜角和角速度*/
void Mpu6050_DataProcessing()//获取MPU6050数据
{  
  Y_MPU6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);// Data acquisition of MPU6050 Y_gyroscope and accelerometer
  kalmanfilter.Angletest(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);// Obtaining Angle by Kalman Filter
  // kalmanfilter.Angle(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);// Obtaining Angle by Kalman Filter
}

/*最后只需将三个环的控制输出量相加即可。*/
void Balanced_Total_Control()//将三个环的控制输出量相加
{
  pwm_left = balance_control_output - speed_control_output - rotation_control_output;//垂直速度转向环叠加
  pwm_right = balance_control_output - speed_control_output + rotation_control_output;//垂直速度转向环叠加

  pwm_left = constrain(pwm_left, -255, 255);
  pwm_right = constrain(pwm_right, -255, 255);

  /*这是为了停止平衡取车时或倾斜太多。*/
  while(EXCESSIVE_ANGLE_TILT || PICKED_UP)
  { 
    Mpu6050_DataProcessing();
    Motor_Stop();
  }
  
  (pwm_left < 0) ?  (Motor_Control(AIN1,1,PWMA_LEFT,-pwm_left)):
                    (Motor_Control(AIN1,0,PWMA_LEFT,pwm_left));
  
  (pwm_right < 0) ? (Motor_Control(BIN1,1,PWMB_RIGHT,-pwm_right)): 
                    (Motor_Control(BIN1,0,PWMB_RIGHT,pwm_right));
}


/*
*最终，在平衡的前提下，我们只需要修改目标速度和目标转向速度来
*控制运动平衡车随意。
*/
void Balanced_Motion_Control(char direction)
{
  switch(direction)
  {
    case STOP:
                  Balanced_Stop();break;
    case FORWARD:
                  Balanced_Forward(40);break;
    case BACK:
                  Balanced_Back(40);break;
    case LEFT:
                  Balanced_Left(50);break;
    case RIGHT:
                  Balanced_Right(50);break;
    default:      
                  Balanced_Stop();break;
  }
}

void Balanced_Stop()
{
  setting_car_speed = 0;
  setting_turn_speed = 0;
}

void Balanced_Forward(int speed)
{
  setting_car_speed = speed;
  setting_turn_speed = 0;
}

void Balanced_Back(int speed)
{
  setting_car_speed = -speed;
  setting_turn_speed = 0;
}

void Balanced_Left(int speed)
{
  setting_car_speed = 0;
  setting_turn_speed = speed;
}

void Balanced_Right(int speed)
{
  setting_car_speed = 0;
  setting_turn_speed = -speed;
}