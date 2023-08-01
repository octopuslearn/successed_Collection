#include <Arduino.h>
/*******************以下，编码电机引脚定义*******************/
#define AIN1 22
#define AIN2 23
#define PWMA_LEFT  4//NC
#define ENCODER_LEFT_A_1_PIN 2 //编码器
#define ENCODER_LEFT_B_1_PIN 3 //编码器
/*******************以上，编码电机引脚定义*******************/


/*******************以下，PID调参相关**********************/
long prevT=0; //更新上一次的时间
int posPrev = 0;

 volatile int now_encoder=0; //当前编码器数值
volatile float velocity_i = 0;//中断内经过的时间
volatile long prevT_i = 0;


float v1Filt = 0;
float v1Prev = 0;
float v2Filt = 0;
float v2Prev = 0;


float eintergal=0;//微分项
int e;//误差==
float u;//pid解算结果==
 float pwr;//pwm输出==


long currT;   //当前时间==
float deltaT; //迭代时间==//当前时间与上一个时间的差异
float velocity1;//==//当前和上一个编码器计数之间的差异，以秒为单位

int pos = 0;//当前脉冲数 ==


float v1; // Convert count/s to RPM//将计数/秒转换为 RPM==
float v2; // Convert count/s to RPM//将计数/秒转换为 RPM==


float velocity2;//==



/*以下，速度环调参*/   
double kp_speed=10,ki_speed=0,kd_speed=0;//== 
float output_speed=0,setpoint_speed=0;//==




void counts_RPMs();/*Compute velocity with method 1//用方法1计算速度*/
void low_pass_filter();/*Low-pass filter (25 Hz cutoff)//低通滤波器（25 Hz 截止）*/
/*以下，速度环调参*/
/*******************以上，PID调参相关**********************/

/****************以下，函数声明****************/
/*以下，声明电机，运动相关*/
void initMotors();/*初始化电机*/
void pwmOut(int dir,int pwr);/*pwm输出*/
void forwardA(int pwm);/*前进*/
void reverseA(int pwm);/*后退*/
void brakeA();/*停车*/
/*以上，声明电机，运动相关*/

/*以下，声明-编码器*/
void initEncoders();/*初始化编码器*/
void ISR_Encoder_LEFT_A_1_PIN();/*读取编码器脉冲*/
/*以上，声明-编码器*/


/*以下，声明-速度环*/
void pid_calculate();/*pid解算&限幅*/
void pid_get_timer();/*获取编码器时间*/
/*以上，声明-速度环*/
/****************以上，函数声明****************/

/*######################以下，主程序######################*/
void setup() {
  Serial.begin(9600);
  initMotors();/*初始化电机*/
  initEncoders();/*初始化编码器*/
/*此为干扰项，打开编码器读不到正常值，只能读到0或-1*/  // FlexiTimer2_int_speed();/*PID-速度-定时器初始化*/
  Serial.println("target pos");
}

void loop() {
  /*以下，代替原子的方法*/
  noInterrupts(); // 
  pos = now_encoder;
  velocity2 = velocity_i;
  interrupts(); // turn interrupts back on
  /*以上，代替原子的方法*/

  pid_get_timer();  // Compute velocity with method 1//用方法1计算速度
  
  counts_RPMs();/*Compute velocity with method 1//用方法1计算速度*/

  low_pass_filter();/*Low-pass filter (25 Hz cutoff)//低通滤波器（25 Hz 截止）*/


  
  setpoint_speed=100*(sin(currT/1e6)>0);
//setpoint_speed=1000;// Set a target//设置目标
//setpoint_speed= 250*sin(prevT/1e6);
  

  pid_calculate();/*pid解算&限幅*/

}
// /*######################以上，主程序######################*/


// /************以下，可爱的小姐**************/
// /*pid解算&限幅*/
void pid_calculate()
{ 
  e = setpoint_speed - v1Filt;//速度环-速度差值//目标速度和滤波速度之间的误差

  eintergal += e*deltaT;//微分项

  u=kp_speed*e + ki_speed*eintergal;//pid解算结果

  //Set the motor speed and direction
  int dir = 1;
  if (u<0){
    dir = -1;
  }
  int pwr = (int) fabs(u);
  if(pwr > 255){
    pwr = 255;
  }
  pwmOut(dir,pwr);

  Serial.print("setpoint_speed: "); Serial.print(setpoint_speed);
  Serial.print(" ");
  Serial.print("v1Filt:  ");  Serial.print(v1Filt);
  Serial.println();
  delay(1);
}

/*获取编码器时间*/
void pid_get_timer()
{
  currT=micros();//获取当前时间
  deltaT=((float)(currT-prevT)/(1.0e6));//迭代时间
  velocity1=(pos-posPrev)/deltaT;//当前和上一个编码器计数之间的差异，以秒为单位
  posPrev=pos;
  prevT=currT;//更新上一次的值
}

/*Convert count/s to RPM//将计数/秒转换为 RPM*/
void counts_RPMs()
{
  // Convert count/s to RPM//将计数/秒转换为 RPM
  v1 = velocity1/506.0*60.0;
  v2 = velocity2/506.0*60.0;
}


/*Low-pass filter (25 Hz cutoff)//低通滤波器（25 Hz 截止）*/
void low_pass_filter()
{
  v1Filt = 0.854*v1Filt + 0.0728*v1 + 0.0728*v1Prev;
  v1Prev = v1;
  v2Filt = 0.854*v2Filt + 0.0728*v2 + 0.0728*v2Prev;
  v2Prev = v2;
}

/************以上，可爱的小姐**************/


/***********以下，编码器相关**********/
/*初始化编码器*/
void initEncoders()
{
  pinMode(ENCODER_LEFT_A_1_PIN,INPUT);
  pinMode(ENCODER_LEFT_B_1_PIN,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A_1_PIN), ISR_Encoder_LEFT_A_1_PIN, RISING);
}

/*读取编码器脉冲*/
void ISR_Encoder_LEFT_A_1_PIN()
{
  int increment = 0;
  if(digitalRead(ENCODER_LEFT_B_1_PIN)==HIGH)
    increment = 1;// If B is high, increment forward//如果 B 为高，则向前递增
  else
    increment = -1;// Otherwise, increment backward//否则，向后递增
  
  now_encoder += increment;

  //Compute velocity with method 2//用方法2计算速度
  currT = micros();
  deltaT = ((float) (currT - prevT_i))/1.0e6;
  velocity_i = increment/deltaT;
  prevT_i = currT;

  // // if(digitalRead(ENCODER_LEFT_A_1_PIN)==HIGH)
  // // {
  // //   if(digitalRead(ENCODER_LEFT_B_1_PIN)==LOW)
  // //     now_encoder++;
  // //   else
  // //     now_encoder--;
  // // }
  // // else 
  // // {
  // //   if(digitalRead(ENCODER_LEFT_B_1_PIN)==HIGH)
  // //     now_encoder++;
  // //   else
  // //     now_encoder--;
  // // }
}
/***********以上，编码器相关**********/



/**********以下，声明电机，运动相关**********/
/*初始化电机*/
void initMotors(){
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA_LEFT, OUTPUT);
}

/*pwm输出*/
void pwmOut(int dir,int pwr)
{
  if(dir<0)//原因是此处电机方向不对
  {
    forwardA(pwr);
  }
  else
  {
    reverseA(pwr);
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




