#include <Arduino.h>

/*以下，编码电机相关*/
#define AIN1 3
#define AIN2 4
#define PWMA_LEFT 5//NC
#define ENCODER_LEFT_A_PIN 2 //编码器
int PPR = 11;              //一圈11个脉冲
volatile long encoderCount=0;//存储电机脉冲数
int rpm = 0; int setRPM=0;   //rpm: 实际测到的转速， setRPM:目标转速

/*以上，编码电机相关*/

/*以下，电位器*/
#define  pot A0//电位器
/*以上，电位器*/

unsigned int last_time;//上一次时间


/*以下，PID相关*/
float setPoint = 19;//滑轨中心与测距模块的距离

float error;          //当前误差
float previous_error; //上一次误差,用来计算D

float kp=10;
float ki=0.05;
float kd=50;

unsigned int dt=50;//每50ms进行一次计算

float P;
float I;
float D;
float PID;
/*以上，PID相关*/



/*以下，函数声明*/
void hc_sr04_read();
void PID_contrl();
void ISR_Encoder();
/*以上，函数声明*/




void setup() {
  Serial.begin(9600);


  /*以下，编码电机*/
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA_LEFT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A_PIN), ISR_Encoder, CHANGE);
  /*以上，编码电机*/


  last_time = millis();//获取上一次时间
  // put your setup code here, to run once:

}

void loop() {
  setRPM = map(analogRead(pot), 0, 1023, 2000, 10000);//设定转速
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2,LOW);   //电机方向
  if(millis() > last_time+dt)
  {
    last_time = millis();//更新上一次时间
    Serial.print("encoderCount: ");    Serial.print(encoderCount);
    rpm = (float)((encoderCount/PPR)*(1000/dt)*60);//实际转速
    Serial.print("起始Measured RPM: ");    Serial.print(rpm);   
    PID_contrl();
    analogWrite(PWMA_LEFT, PID); //调整马达转速

        // 串口监视器上显示目标，Arduino 输出的 PWM， 以及测量到的RPM
      Serial.print("Set RPM: ");         Serial.print(setRPM);
      Serial.print('\t');
      Serial.print("Measured RPM: ");    Serial.print(rpm);
      Serial.print('\t');
      Serial.print("PID: ");             Serial.println(PID);

      //encoderCount = 0;// 累计脉冲数量重置,为下一秒测量做准备  
  }
}





void PID_contrl(){
  error=rpm-setRPM;          //计算误差
  P=kp*error;                       //P项
  if(-4<error && error<4)
  {
    I += ki*error;                  //I项
  }
  else
  {
    I = 0;
  }
  D = kd*((error-previous_error)/dt);//D项

  PID=P+I+D;                         //PID

  if(PID>200) {PID=200;}             //限幅
  if(PID<-200){PID=-200;}

  previous_error = error;//更新上一刻误差
}


void ISR_Encoder(){
  encoderCount++;
}