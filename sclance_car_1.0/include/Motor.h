#ifndef _MOTOR_H
#define _MOTOR_H
#include <Arduino.h> 
         /*Motor pin*/
          #define AIN1 7
          #define AIN2 9
          #define PWMA_LEFT 5//NC
          #define BIN1 12
          #define BIN2 10
          #define PWMB_RIGHT 6//NC
          #define STBY_PIN 8//电机工作/待机
          
          /*Encoder measuring speed  pin*/
          #define ENCODER_LEFT_A_PIN 2//读取左电机脉冲
          #define ENCODER_RIGHT_A_PIN 4//读取右电机脉冲

          void Motor_Pin_init();
          void Motor_Stop();
          void Motor_Forward(int speed);
          void Motor_Back(int speed);
          void Motor_Left(int speed);
          void Motor_Right(int speed);
          void Motor_Control(int PIN,int PIN_value,int PWM_pin,int speed);


#endif
