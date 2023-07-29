#include <PID_v1.h>
#include <Encoder.h>

/*以下，电机相关*/
#define AIN1 22
#define AIN2 23
#define PWMA_LEFT  4//NC
#define ENCODER_LEFT_A_1_PIN 2 //编码器
#define ENCODER_LEFT_B_1_PIN 3 //编码器
/*以上，电机相关*/


/*以下，编码器相关*/
Encoder leftEncoder(ENCODER_LEFT_A_1_PIN,ENCODER_LEFT_B_1_PIN);     // 左轮编码器引脚 ， 需要检查 
long leftPosition;
long readEncoder(int i);      /*获取编码器值*/
void resetEncoder();          /*获取编码电机当前值*/
/*以上，编码器相关*/


/*以下，声明电机，运动相关*/
void initMotors();/*初始化电机*/
void pwmOut(int out);/*pwm输出*/
void forwardA(int pwm);/*前进*/
void reverseA(int pwm);/*后退*/
void brakeA();/*停车*/
/*以上，声明电机，运动相关*/















void setup()
{
  Serial.begin(9600);

}

void loop()
{ 
  resetEncoder();
  Serial.print("当前值");Serial.print("\t");Serial.println(leftPosition);
}







/*以下，encoder库-获取编码电机相关*/
/*获取编码器值*/
long readEncoder()
{
    return leftEncoder.read()-leftPosition;
}

/*获取编码电机当前值*/
void resetEncoder()
{
    leftPosition=leftEncoder.read();    
}
/*以下，encoder库-获取编码电机相关*/







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