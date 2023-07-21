#include "Motor.h"

extern void Motor_Encoder_init();//读取编码电机脉冲
extern void EncoderCountRightA();//获取右电机脉冲
extern void EncoderCountLeftA();//获取左电机脉冲
extern unsigned long Motor_ncoder_count_right_a;//右电机脉冲
extern unsigned long Motor_encoder_count_left_a;//左电机脉冲

void setup()
{
  Motor_Pin_init();
  Motor_Encoder_init();
  Serial.begin(9600);
}

void loop()
{   for(int i=0;i<100;i++)
    { 
      Motor_Forward(i);
      Serial.print(Motor_encoder_count_left_a); Serial.print("\t");Serial.println(Motor_ncoder_count_right_a);
      delay(100);
    }
    Motor_encoder_count_left_a=0;
    Motor_ncoder_count_right_a=0;
}