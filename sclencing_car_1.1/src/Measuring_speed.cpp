#include "Motor.h"
#include "PinChangeInt.h"


unsigned long Motor_encoder_count_right_a;//右电机脉冲
unsigned long Motor_encoder_count_left_a;//左电机脉冲

void EncoderCountRightA();//获取右电机脉冲
void EncoderCountLeftA();//获取左电机脉冲

void Motor_Encoder_init()//读取编码电机脉冲
{
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A_PIN), EncoderCountLeftA, CHANGE);
  attachPinChangeInterrupt(ENCODER_RIGHT_A_PIN, EncoderCountRightA, CHANGE);
}


//Getting Right Wheel Speed.
void EncoderCountRightA()//获取右电机脉冲
{
  Motor_encoder_count_right_a++;
}



//Getting Left Wheel Speed.
void EncoderCountLeftA()//获取左电机脉冲
{
  Motor_encoder_count_left_a++;
}
