/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 * 说明：白底上亮-1，黑线上灭-0
 * 规定：左转为车向左转，右转为车向右转
 */

#include <Arduino.h>

#define hd1   22    //
#define hd2   23    //
#define hd3   24    //
#define hd4   25    //
#define hd5   26    //
#define hd6   27    //
#define hd7   28    //
#define hd8   29    //


#define left_motor_pwm    2 //左电机pwm
#define right_motor_pwm   3 //右电机pwm
#define ln1_left    30 //左电机1
#define ln2_left    31 //左电机2
#define ln3_right   32 //右电机1
#define ln4_right   33 //右电机2

#define STOP      'a'   //停止
#define RUN       'b'  //前进
#define BACK      'c'   //后退
#define LEFT      'd'   //左转
#define RIGHT     'e'   //右转

int hd_value[9];  //灰度传感器数值



void motor_Exercise_status(char motro_state, int left_pwm, int right_pwm);
void fline();





void setup()
{
  Serial.begin(9600);
}

void loop()
{
  fline();
}





/*循迹*/
void fline()
{
  hd_value[1] = digitalRead(hd1);
  hd_value[2] = digitalRead(hd2);
  hd_value[3] = digitalRead(hd3);
  hd_value[4] = digitalRead(hd4); //中左1
  hd_value[5] = digitalRead(hd5); //中右1
  hd_value[6] = digitalRead(hd6);
  hd_value[7] = digitalRead(hd7);
  hd_value[8] = digitalRead(hd8);

  /*调试*/
   Serial.print(hd_value[1]); Serial.print('\t');
    Serial.print(hd_value[2]); Serial.print('\t');
     Serial.print(hd_value[3]); Serial.print('\t');
      Serial.print(hd_value[4]); Serial.print('\t');
        Serial.print(hd_value[5]); Serial.print('\t');
         Serial.print(hd_value[6]); Serial.print('\t');
          Serial.print(hd_value[7]); Serial.print('\t');
            Serial.print(hd_value[8]); Serial.println('\t');
  

  // if(hd_value[4]==0 || hd_value[5]==0)
  // {
  //   if(hd_value[3]==1 && hd_value[6]==1)
  //   {
  //     motor_Exercise_status(RUN, 100, 100);
  //   }
  // }

  if(hd_value[3]==0 && hd_value[4]==0 && hd_value[5]==1 && hd_value[6]==1)  //右偏-小      0011
  {
    motor_Exercise_status(LEFT, 50, 50);
    /*调试*/Serial.println("右偏-小");
  }
  else if(hd_value[3]==0 && hd_value[4]==1 && hd_value[5]==1 && hd_value[6]==1)  //右偏-中 0111
  {
    motor_Exercise_status(LEFT, 80, 80);
    /*调试*/Serial.println("右偏-中");
  }

  else if(hd_value[3]==1 && hd_value[4]==1 && hd_value[5]==0 && hd_value[6]==0)  //左偏-小 1100
  {
    motor_Exercise_status(RIGHT, 50, 50);
    /*调试*/Serial.println("左偏-小");
  }
  else if(hd_value[3]==1 && hd_value[4]==1 && hd_value[5]==1 && hd_value[6]==0)  //左偏-中 1110
  {
    motor_Exercise_status(RIGHT, 80, 80);
    /*调试*/Serial.println("左偏-中");
  }
  
  motor_Exercise_status(RUN, 100, 100);
  /*调试*/Serial.println("正轨");
}


/*电机运动状态*/
void motor_Exercise_status(char motro_state, int left_pwm, int right_pwm)
{
  analogWrite(left_motor_pwm,  left_pwm);     //PWM控制
  analogWrite(right_motor_pwm, right_pwm);

  switch(motro_state)
  {
    case RUN:
      Serial.println("RUN"); //前进输出状态
      digitalWrite(ln1_left,  LOW);
      digitalWrite(ln2_left,  HIGH);
      digitalWrite(ln3_right, LOW);
      digitalWrite(ln4_right, HIGH);
      break;
      
     case BACK:
      Serial.println("BACK"); //后退输出状态
      digitalWrite(ln1_left,  HIGH);
      digitalWrite(ln2_left,  LOW);
      digitalWrite(ln3_right, HIGH);
      digitalWrite(ln4_right, LOW);
      break;

     case RIGHT:
      Serial.println("LEFT"); //左转输出状态
      digitalWrite(ln1_left,  HIGH);  //左轮开机
      digitalWrite(ln2_left,  LOW);
      digitalWrite(ln3_right, LOW);
      digitalWrite(ln4_right, HIGH);
      break;
  
     case LEFT:
      Serial.println("RIGHT"); //右转弯输出状态
      digitalWrite(ln1_left,  LOW);
      digitalWrite(ln2_left,  HIGH);
      digitalWrite(ln3_right, HIGH);  //右轮开机
      digitalWrite(ln4_right, LOW);
      break;

     default:
      Serial.println("STOP"); //停止输出状态
      digitalWrite(ln1_left,  LOW);
      digitalWrite(ln2_left,  LOW);
      digitalWrite(ln3_right, LOW);
      digitalWrite(ln4_right, LOW);
      break;
  }
}