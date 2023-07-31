#include <Arduino.h>
#include <PID_v1.h>
#include <FlexiTimer2.h>





#include <util/atomic.h> // For the ATOMIC_BLOCK macro





/*******************以下，编码电机引脚定义*******************/
#define AIN1 22
#define AIN2 23
#define PWMA_LEFT  4//NC
#define ENCODER_LEFT_A_1_PIN 2 //编码器
#define ENCODER_LEFT_B_1_PIN 3 //编码器


// #define AIN3 24
// #define AIN4 25
// #define PWMB_RIGHT 5
// #define ENCODER_RIGHT_A_1_PIN 18 //编码器
// #define ENCODER_RIGHT_B_1_PIN 19 //编码器
// volatile int RIGHT_A_posi=0;
// volatile int RIGHT_B_posi=0;
/*******************以上，编码电机引脚定义*******************/



/*******************以下，编码电机硬件相关**********************/
/*以下，编码电机硬件参数*/
int PPR = 11;                //一圈11个脉冲
volatile long now_encoder=0; //当前编码器数值
/*以上，编码电机硬件参数*/
/*******************以下，编码电机硬件相关**********************/



/*******************以下，PID调参相关**********************/
/*以下，位置环调参*/
int set_REV_location=0;                                      //位置环-目标值
double kp_location=0.5,ki_location=0,kd_location=0;//位置环-kp,ki,kd-参数
// double kp=0.4,ki=0,kd=0.001;                              //位置环-kp,ki,kd-参数--->成功

double input_location=0,output_location=0,setpoint_location=0;
PID motor_A_PID_location(&input_location, &output_location, &setpoint_location, kp_location, ki_location, kd_location, DIRECT);

int MIN_PWM_location=-125, MAX_PWM_location=125;              //位置环-pid限幅
unsigned long SAMPLE_TIME_location = 10;                     //位置环-采样时间-10ms
/*以上，位置环调参*/


/*以下，速度环调参*/
int set_REV_speed=0;                                         //速度环-目标值
double kp_speed=1.12,ki_speed=0,kd_speed=0;                  //速度环-kp,ki,kd-参数

double input_speed=0,output_speed=0,setpoint_speed=0;
PID motor_A_PID_speed(&input_speed, &output_speed, &setpoint_speed, kp_speed, ki_speed, kd_speed, DIRECT);

int MIN_PWM_speed=-200, MAX_PWM_speed=200;
unsigned long SAMPLE_TIME_speed = 10;                        //速度环环-采样时间-10ms
float speed_rmp_in;//当前速度-圈/分钟
/*以下，速度环调参*/
/*******************以上，PID调参相关**********************/




/****************以下，函数声明****************/
/*以下，声明电机，运动相关*/
void initMotors();/*初始化电机*/
void pwmOut(int out);/*pwm输出*/
void forwardA(int pwm);/*前进*/
void reverseA(int pwm);/*后退*/
void brakeA();/*停车*/
/*以上，声明电机，运动相关*/

/*以下，声明-编码器*/
void initEncoders();/*初始化编码器*/
void ISR_Encoder_LEFT_A_1_PIN();/*读取编码器脉冲*/
/*以上，声明-编码器*/


/*以下，声明-位置环*/
void initPID_location();/*初始化PID-位置*/
void PID_control_location();/*位置环*/
/*以上，声明-位置环*/



/*以下，声明-速度环*/
void FlexiTimer2_int_speed();/*PID-速度-定时器初始化*/
void initPID_speed();/*初始化PID-速度*/
void PID_control_speed();/*速度环*/
void PID_encoder_speed();/*速度环-读取转速*/
/*以上，声明-速度环*/
/****************以上，函数声明****************/









/*######################以下，主程序######################*/
void setup() {
  Serial.begin(9600);
  initMotors();/*初始化电机*/
  initEncoders();/*初始化编码器*/
/*此为干扰项，打开编码器读不到正常值，只能读到0或-1*/  // FlexiTimer2_int_speed();/*PID-速度-定时器初始化*/

  // initPID_location();/*初始化PID-位置*/
}

void loop() {
  /************************以下，添加********************/
int pos = 0; 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
  pos = now_encoder;
}
/************************以上，添加********************/
  // PID_control_speed();
  // PID_control_location();
/*调试*/Serial.print("pos:  "); Serial.println(pos);
}
/*######################以上，主程序######################*/






/*********以下，速度环*********/
/*PID-速度-定时器初始化*/
void FlexiTimer2_int_speed()
{
  FlexiTimer2::set(10, 1.0/1000, PID_encoder_speed);   //10ms
  FlexiTimer2::start();
}

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
  set_REV_speed=100;//速度环目标值设定
  /*调试*/Serial.print("目标速度-rpm/min: "); Serial.print(set_REV_speed); Serial.print("\t");
  setpoint_speed=set_REV_speed;//PID工作时达到此值视为SET值
  input_speed=speed_rmp_in;//被调值
  /*调试*/Serial.print("#####被调前-被调值: "); Serial.print(input_speed); Serial.print("\t");
  motor_A_PID_speed.Compute();
  /*调试*/Serial.print("当前速度-被调值: "); Serial.print(speed_rmp_in); Serial.print("\t");
  pwmOut(output_speed);
  /*调试*/Serial.print("速度-输出情况: "); Serial.println(output_speed);
}

/*速度环-读取转速*/
void PID_encoder_speed()
{
  speed_rmp_in=(6000*now_encoder)/2024.0;//(60*20*电机脉冲)/46*11*4
  now_encoder=0;
}
/*********以上，速度环*********/





/*********以下，位置环*********/
/*初始化PID-位置*/
void initPID_location()
{
  motor_A_PID_location.SetMode(AUTOMATIC);//自动模式
  motor_A_PID_location.SetSampleTime(SAMPLE_TIME_location);//设置采样时间-默认200ms 
  motor_A_PID_location.SetOutputLimits(MIN_PWM_location, MIN_PWM_location);//设定PWM范围
}

/*位置环*/
void PID_control_location()
{
  set_REV_location=11*46*2;
  setpoint_location=set_REV_location;//PID工作时达到此值视为SET值
  /*调试*/Serial.print("目标角度对应脉冲数: "); Serial.print(setpoint_location); Serial.print("\t");
  input_location=now_encoder;//来自编码器的数据被视为过程值
  motor_A_PID_location.Compute();//计算新的值输出
  /*调试*/Serial.print("被调值: "); Serial.print(now_encoder); Serial.print("\t");
  pwmOut(output_location);
  /*调试*/Serial.print("输出情况: "); Serial.println(output_location); 
}
/*********以上，位置环*********/







/***********以下，编码器相关**********/
/*初始化编码器*/
void initEncoders()
{
  pinMode(ENCODER_LEFT_A_1_PIN,INPUT);
  pinMode(ENCODER_LEFT_B_1_PIN,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A_1_PIN), ISR_Encoder_LEFT_A_1_PIN, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A_1_PIN), ISR_Encoder_LEFT_A_1_PIN, CHANGE);
  
  
  // pinMode(ENCODER_RIGHT_A_1_PIN,INPUT);
  // pinMode(ENCODER_RIGHT_A_1_PIN,INPUT);
  // attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A_1_PIN), ISR_Encoder_RIGHT_A_1_PIN, RISING);
  // attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A_1_PIN), ISR_Encoder_RIGHT_B_1_PIN, RISING);
}

/*读取编码器脉冲*/
void ISR_Encoder_LEFT_A_1_PIN()
{
  if(digitalRead(ENCODER_LEFT_A_1_PIN)==HIGH)
  {
    if(digitalRead(ENCODER_LEFT_B_1_PIN)==LOW)
      now_encoder++;
    else
      now_encoder--;
  }
  else 
  {
    if(digitalRead(ENCODER_LEFT_B_1_PIN)==HIGH)
      now_encoder++;
    else
      now_encoder--;
  }
}
/***********以上，编码器相关**********/



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




