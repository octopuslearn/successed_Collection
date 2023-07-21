#include "Motor.h"
#include <Arduino.h>

void Motor_Pin_init()
{
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA_LEFT, OUTPUT);
  pinMode(PWMB_RIGHT, OUTPUT);
  pinMode(STBY_PIN, OUTPUT);
  digitalWrite(STBY_PIN, HIGH);//电机开机
}


void Motor_Stop()
{
  digitalWrite(STBY_PIN,0);//待机即可
//  analogWrite(PWMA_LEFT, 0);
//  analogWrite(PWMB_RIGHT, 0);
}

void Motor_Forward(int speed)
{
  digitalWrite(AIN1, 0);
  digitalWrite(AIN2, 1);
  digitalWrite(BIN1, 0);
  digitalWrite(BIN2, 1);
  analogWrite(PWMA_LEFT, speed);
  analogWrite(PWMB_RIGHT, speed);
}

void Motor_Back(int speed)
{
  digitalWrite(AIN1, 1);
  digitalWrite(AIN2, 0);
  digitalWrite(BIN1, 1);
  digitalWrite(BIN2, 0);
  analogWrite(PWMA_LEFT, speed);
  analogWrite(PWMB_RIGHT, speed);
}


void Motor_Left(int speed)
{
  digitalWrite(AIN1, 0);
  digitalWrite(AIN2, 1);
  digitalWrite(BIN1, 1);
  digitalWrite(BIN2, 0);;
  analogWrite(PWMA_LEFT, speed);
  analogWrite(PWMB_RIGHT, 0);
}

void Motor_Right(int speed)
{
  digitalWrite(AIN1, 1);
  digitalWrite(AIN2, 0);
  digitalWrite(BIN1, 0);
  digitalWrite(BIN2, 1);
  analogWrite(PWMA_LEFT,0);
  analogWrite(PWMB_RIGHT,speed);
}

void Motor_Control(int PIN,int PIN_value,int PWM_pin,int speed)
{
  digitalWrite(PIN, PIN_value);
  analogWrite(PWM_pin,speed);
}
