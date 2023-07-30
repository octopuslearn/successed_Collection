#include <PID_v1.h>
#include <Encoder.h>
#include <Arduino.h>

/*以下，电机相关*/
#define AIN1 22
#define AIN2 23
#define PWMA_LEFT  4//NC
#define ENCODER_LEFT_A_1_PIN 2 //编码器
#define ENCODER_LEFT_B_1_PIN 3 //编码器
/*以上，电机相关*/


/*以下，编码器相关*/
Encoder leftEncoder(ENCODER_LEFT_A_1_PIN,ENCODER_LEFT_B_1_PIN);     // 左轮编码器引脚 ， 需要检查 
long leftPosition;
long readEncoder(int i);      /*获取编码器值*/
void resetEncoder();          /*获取编码电机当前值*/
/*以上，编码器相关*/


/*******************以下，PID调参相关**********************/
/*以下，位置环调参*/
double currentEncoder_location,lastEncoder_location;//本次，上一次编码器值-位置


double set_REV_location=0;//位置环-目标值                                     //位置环-目标值
double kp_location=0.5,ki_location=0,kd_location=0;//位置环-kp,ki,kd-参数
// double kp=0.4,ki=0,kd=0.001;                              //位置环-kp,ki,kd-参数--->成功

double input_location=0,output_location=0,setpoint_location=0;
PID motor_A_PID_location(&input_location, &output_location, &setpoint_location, kp_location, ki_location, kd_location, DIRECT);

int MIN_PWM_location=-125, MAX_PWM_location=125;              //位置环-pid限幅
double SAMPLE_TIME_location = 10;                     //位置环-采样时间-10ms
/*以上，位置环调参*/


/*以下，速度环调参*/
double currentEncoder_speed,lastEncoder_speed;//本次，上一次编码器值-速度
double error_speed;//目标值和当前值误差
long nextmotion;//时间？？？





double set_REV_speed=0;                                         //速度环-目标值
double kp_speed=1.12,ki_speed=0,kd_speed=0;                  //速度环-kp,ki,kd-参数

double input_speed=0,output_speed=0,setpoint_speed=0;
PID motor_A_PID_speed(&input_speed, &output_speed, &setpoint_speed, kp_speed, ki_speed, kd_speed, DIRECT);

int MIN_PWM_speed=-200, MAX_PWM_speed=200;
double SAMPLE_TIME_speed = 10.0;                        //速度环环-采样时间-10ms
float speed_rmp_in;//当前速度-圈/分钟
/*以下，速度环调参*/
/*******************以上，PID调参相关**********************/



//车轮配置
/*****************************  第一步修改 电机外输出轴 转动一圈 所输出的总脉冲数  ************

   由于是采用的中断方式捕获电机的霍尔脉冲，并且使用的是边沿触发方式，所以电机的编码值计算方法如下：
    encoder = （边沿触发）2 x 霍尔编码器相数量（如：2） x 霍尔编码器线束 (如 13 ) x 电机减速比 (如：30)

************************************************************************************/

//double encoderresolution = 2496.0; //编码器输出脉冲数/圈 2*2*13*90 = 2496    TT-motor encoder
//double encoderresolution = 4680.0; //编码器输出脉冲数/圈 2*2*13*90 = 4680    TT-motor encoder 1:90 金属
double encoderresolution = 2024.0;   //编码器输出脉冲数/圈 2*2*11*46 = 1560    37-motor encoder




/*以下，声明电机，运动相关*/
void initMotors();/*初始化电机*/
void pwmOut(int out);/*pwm输出*/
void forwardA(int pwm);/*前进*/
void reverseA(int pwm);/*后退*/
void brakeA();/*停车*/
/*以上，声明电机，运动相关*/


/*以下，声明-位置环*/
void initPID_location();/*初始化PID-位置*/
void PID_control_location();/*位置环*/
/*以上，声明-位置环*/



/*以下，声明-速度环*/
void initPID_speed();/*初始化PID-速度*/
void PID_control_speed();/*速度环*/
/*以上，声明-速度环*/

























void setup()
{
  Serial.begin(9600);
/*调试*/  initMotors();//电机初始化
  resetEncoder();/*获取编码电机当前值*/
  initPID_speed();/*初始化PID-速度*/

}

void loop()
{ 
// /*调试*/   pwmOut(100);
// /*调试*/   resetEncoder();
/*调试*/   Serial.print("当前值");Serial.print("\t");Serial.println(leftPosition);
  if(nextmotion<=millis())
  {
    PID_control_speed();

    nextmotion=millis()+SAMPLE_TIME_speed;

    /*********   PID 调试时使用 正常使用应注释掉 ***********/
    /*********   格式严格 “:” 与“,”不可以注释掉 ***********/
    
   Serial.print("Left_Encoder_value: ");
   Serial.print(input_speed);
   Serial.print(",");
   Serial.print("Target_encoder: ");
   Serial.println(setpoint_speed);
        
    /******** PID 参数调试完毕后 应注释掉上述代码*********/
    /************************************************/
  }


}

























/*以下，encoder库-获取编码电机相关*/
/*获取编码器值*/
long readEncoder()
{
    return leftEncoder.read()-leftPosition;
}

/*获取编码电机当前值*/
void resetEncoder()
{
    leftPosition=leftEncoder.read();    
}
/*以下，encoder库-获取编码电机相关*/







/**********以下，声明电机，运动相关**********/
/*初始化电机*/
void initMotors(){
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA_LEFT, OUTPUT);
  

  // pinMode(AIN3, OUTPUT);
  // pinMode(AIN4, OUTPUT);
  // pinMode(PWMB_RIGHT, OUTPUT);
}


/*pwm输出*/
void pwmOut(int out)
{
  if(out>0)
  {
    forwardA(out);
  }
  else
  {
    reverseA(abs(out));
  }
}

/*前进*/
void forwardA(int pwm)
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA_LEFT, pwm);
}

/*后退*/
 void reverseA(int pwm)
 {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA_LEFT, pwm);
} 

/*停车*/
void brakeA()
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}
/**********以上，声明电机，运动相关**********/


/*********以下，速度环*********/
/*初始化PID-速度*/
void initPID_speed()
{
  motor_A_PID_speed.SetMode(AUTOMATIC);//自动模式
  motor_A_PID_speed.SetSampleTime(SAMPLE_TIME_speed);//设置采样时间-默认200ms 
  motor_A_PID_speed.SetOutputLimits(MIN_PWM_speed, MAX_PWM_speed);//设定PWM范围
}

/*速度环*/
void PID_control_speed()
{
  currentEncoder_speed=readEncoder();                  // 读取编码器累计记录值
  input_speed=currentEncoder_speed-lastEncoder_speed;//当前PID周期内实际检测到的脉冲值
  input_speed=set_REV_speed;//速度环-pid输入
  setpoint_speed=100.0;
  // error_speed=setpoint_speed-input_speed;// 目标脉冲值 与 当前PID周期实际检测的脉冲值之间 的差值。
  motor_A_PID_speed.Compute();                  // 根据 p i d 参数以及目标脉冲与实际检测脉冲之间的关系 计算这一次的输出结果，作为电机驱动器的 PWM 值
  lastEncoder_speed=readEncoder();
  pwmOut(output_speed);
}
/*********以上，速度环*********/







// /*********以下，位置环*********/
// /*初始化PID-位置*/
// void initPID_location()
// {
//   motor_A_PID_location.SetMode(AUTOMATIC);//自动模式
//   motor_A_PID_location.SetSampleTime(SAMPLE_TIME_location);//设置采样时间-默认200ms 
//   motor_A_PID_location.SetOutputLimits(MIN_PWM_location, MIN_PWM_location);//设定PWM范围
// }

// /*位置环*/
// void PID_control_location()
// {
//   set_REV_location=11*46*2;
//   setpoint_location=set_REV_location;//PID工作时达到此值视为SET值
//   /*调试*/Serial.print("目标角度对应脉冲数: "); Serial.print(setpoint_location); Serial.print("\t");
//   input_location=now_encoder;//来自编码器的数据被视为过程值
//   motor_A_PID_location.Compute();//计算新的值输出
//   /*调试*/Serial.print("被调值: "); Serial.print(now_encoder); Serial.print("\t");
//   pwmOut(output_location);
//   /*调试*/Serial.print("输出情况: "); Serial.println(output_location); 
// }
// /*********以上，位置环*********/