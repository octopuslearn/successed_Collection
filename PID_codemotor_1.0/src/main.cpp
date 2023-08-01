#include <Arduino.h>
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
volatile int now_encoder=0; //当前编码器数值
/*以上，编码电机硬件参数*/
/*******************以下，编码电机硬件相关**********************/



/*******************以下，PID调参相关**********************/
/*以下，速度环调参*/                                     
// double kp_speed=5,ki_speed=0.2,kd_speed=0.8;                  //速度环-kp,ki,kd-参数
// double kp_speed=1.1,ki_speed=0.05,kd_speed=0.4; 
double kp_speed=10,ki_speed=0,kd_speed=0.001; 
float output_speed=0,setpoint_speed=0;
int pos = 0;//当前脉冲数 
int e;//误差
float dedt;//积分项-用于计算积分的误差
float pwr;//pwm输出
float u;//pid解算结果


float eprev=0;    //上一次误差
float eintergal=0;//微分项


long currT;   //当前时间
float deltaT; //迭代时间
long prevT=0; //更新上一次的时间


int spped_stop=50;
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

/*以上，声明-位置环*/



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
/************************以下，添加********************/
  setpoint_speed=1000;
  pid_get_timer();

  pos=0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
  {
    pos = now_encoder;
  }
  pid_calculate();
/************************以上，添加********************/
}
/*######################以上，主程序######################*/




/************以下，可爱的小姐**************/
/*pid解算&限幅*/
void pid_calculate()
{ 
  e = pos-setpoint_speed;//速度环-误差

  if((e>(-spped_stop)) && (e<(spped_stop)))
  {
    e=0;
    eprev=0;
    dedt=0;
  }





  // e = pos-setpoint_speed;//速度环-误差

  dedt=(e-eprev)/(deltaT);//积分项-用于计算积分的误差

  eintergal+=e*deltaT;//微分项

  u=kp_speed*e+kd_speed*dedt+ki_speed*eintergal;//pid解算结果

  pwr=u;//pwm输出

  if(pwr>255)//pwm限幅
    pwr=255;
  else if(pwr<-255)
    pwr=-255;
  pwmOut(pwr);

  eprev=e;//更新上一次误差
  Serial.print("setpoint_speed: "); Serial.print(setpoint_speed);
  Serial.print(" ");
  Serial.print("pos:  ");  Serial.print(pos);
  Serial.println();
}

/*获取编码器时间*/
void pid_get_timer()
{
  currT=micros();//获取当前时间
  deltaT=((float)(currT-prevT)/(1.0e6));//迭代时间
  prevT=currT;//更新上一次的时间
}
/************以上，可爱的小姐**************/





/***********以下，编码器相关**********/
/*初始化编码器*/
void initEncoders()
{
  pinMode(ENCODER_LEFT_A_1_PIN,INPUT);
  pinMode(ENCODER_LEFT_B_1_PIN,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A_1_PIN), ISR_Encoder_LEFT_A_1_PIN, RISING);
  
  
  // pinMode(ENCODER_RIGHT_A_1_PIN,INPUT);
  // pinMode(ENCODER_RIGHT_A_1_PIN,INPUT);
  // attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A_1_PIN), ISR_Encoder_RIGHT_A_1_PIN, RISING);
  // attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A_1_PIN), ISR_Encoder_RIGHT_B_1_PIN, RISING);
}

/*读取编码器脉冲*/
void ISR_Encoder_LEFT_A_1_PIN()
{
  if(digitalRead(ENCODER_LEFT_B_1_PIN)==HIGH)
    now_encoder++;
  else
    now_encoder--;
  // if(digitalRead(ENCODER_LEFT_A_1_PIN)==HIGH)
  // {
  //   if(digitalRead(ENCODER_LEFT_B_1_PIN)==LOW)
  //     now_encoder++;
  //   else
  //     now_encoder--;
  // }
  // else 
  // {
  //   if(digitalRead(ENCODER_LEFT_B_1_PIN)==HIGH)
  //     now_encoder++;
  //   else
  //     now_encoder--;
  // }
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




