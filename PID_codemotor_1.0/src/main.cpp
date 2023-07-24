#include <Arduino.h>
#include <Servo.h>


/*以下，超声波相关*/
#define trig 8//控制
#define echo 9//信号

float distance;//胶带位置
/*以上，超声波相关*/



/*以下，舵机相关*/
#define Servo1 10//舵机

Servo Servo_1;//创建舵机对象

int angle;//舵机角度
/*以上，舵机相关*/



unsigned int last_time;//上一次时间


/*以下，PID相关*/
float setPoint = 19;//滑轨中心与测距模块的距离

float error;          //当前误差
float previous_error; //上一次误差,用来计算D

float kp=10;
float ki=0.05;
float kd=50;

unsigned int dt=50;//没50ms进行一次计算

float P;
float I;
float D;
float PID;
/*以上，PID相关*/



/*以下，函数声明*/
void hc_sr04_read();
void PID_contrl();
/*以上，函数声明*/




void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo,INPUT);
  pinMode(Servo1,OUTPUT);
  Servo_1.attach(10);
  last_time = millis();//获取上一次时间
  // put your setup code here, to run once:

}

void loop() {
  if(millis() > last_time+dt)
  {
    last_time = millis();//更新上一次时间
    hc_sr04_read();//获取超声波参数
    PID_contrl();  //PID控制

    /*调试*/

    Serial.print("distance:  ");Serial.print(distance);     
    Serial.print("Error: ");   Serial.print(error);    
    Serial.print("   P: ");     Serial.print(P); 
    Serial.print("   D: ");     Serial.print(D);
    Serial.print("   I: ");     Serial.print(I);
    Serial.print("   PID: ");   Serial.print(PID);
    
    angle = map(PID,-200,200,150,20);
    Serial.print("   angle: ");   Serial.println(angle);
    // put your main code here, to run repeatedly:
    Servo_1.write(angle);
  }
}




void hc_sr04_read(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(40);
  
  digitalWrite(trig, LOW);

  distance=pulseIn(echo, HIGH)/58;//获取高电平-距离

}

void PID_contrl(){
  error=distance-setPoint;          //计算误差
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
