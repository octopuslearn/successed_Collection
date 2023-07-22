#include "Motor.h"
#include "Balanced.h"

extern void Motor_Encoder_init();//读取编码电机脉冲
extern void EncoderCountRightA();//获取右电机脉冲
extern void EncoderCountLeftA();//获取左电机脉冲
extern unsigned long Motor_ncoder_count_right_a;//右电机脉冲
extern unsigned long Motor_encoder_count_left_a;//左电机脉冲

void setup()
{
  Serial.begin(9600);
  Motor_Pin_init();
  Motor_Encoder_init();
  Timer2_init(TIMERE);//初始化定时器2，每5ms
  Mpu6050_init();
}

void loop()
{   
  char direction_buf[]={FORWARD,BACK,LEFT,RIGHT,STOP};
  static unsigned long print_time;

  for(int i=0;i<5;)
  {
    if(millis() - print_time > 5000)
    { 
        print_time = millis();
        Balanced_Motion_Control(direction_buf[i++]);
    }
  }

}