/***此为转弯时，两轮同向-根本转不了弯***/

/***
 * 白色-0-红灯亮
 * 黑色-1-红灯灭
 * 1-说明在线上
 * 0-说明脱线
****/

#define STOP      0 //停止
#define RUN       1 //前进
#define BACK      2 //后退
#define LEFT      3 //左转
#define RIGHT     4 //右转
int a1 = 6;//左电机1
int a2 = 7;//左电机2
int b1 = 8;//右电机1
int b2 = 9;//右电机2
int leftPWM = 10;//L298N使能端左
int rightPWM = 11;//L298N使能端右


int sensor1 = 2; //从车头方向的自左向右排序 探测器
int sensor2 = 3; 
int sensor3 = 4; 
int sensor4 = 5; 

#define full 100
#define small_pwm full*2.5
#define big_pwm   full*2.5
#define go        full*1
#define go_p      full*0.6

void car_work(int which_car, int pwm_left_value, int pwm_right_value);


void setup()
{
  Serial.begin(9600);//电机控制引脚初始化
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(sensor1, INPUT);//寻迹模块引脚初始化
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_hongwai();
}


void read_hongwai()
{
  int hw_in[4];
  hw_in[0] = digitalRead(sensor1);
  hw_in[1] = digitalRead(sensor2);
  hw_in[2] = digitalRead(sensor3);
  hw_in[3] = digitalRead(sensor4);
  Serial.print(hw_in[0]); Serial.print('\t');
   Serial.print(hw_in[1]); Serial.print('\t');
    Serial.print(hw_in[2]); Serial.print('\t');
     Serial.print(hw_in[3]); Serial.print('\t');
  Serial.print('\n');
  /*修正*/

  if(hw_in[1])                               /*左边有线，右偏-左转*/
  {
    Serial.println("小幅度-右偏");
    car_work(LEFT, go_p, small_pwm);
  }
  else if(hw_in[0])                               /*左边有线，右偏-左转*/
  {
    Serial.println("大幅度-右偏");
    car_work(LEFT, go_p, big_pwm);
  }
  
  else if(hw_in[2])                               /*右边有线，左偏-右转*/
  {
    Serial.println("小幅度-左偏");
    car_work(RIGHT, small_pwm, go_p);
  }
  else if(hw_in[3])                               /*右边有线，左偏-右转*/
  {
    Serial.println("大幅度-左偏");
    car_work(RIGHT, big_pwm, go_p);
  }

  else if(!hw_in[1] && !hw_in[2])
  {
    Serial.println("正轨");
    car_work(RUN, go, go);
  } 
}


void car_work(int which_car, int pwm_left_value, int pwm_right_value)
{
  
  analogWrite(leftPWM, pwm_left_value);     //PWM控制
  analogWrite(rightPWM, pwm_right_value);
  Serial.print("pwm_left_value:  ");    Serial.print(pwm_left_value);     Serial.print('\t');
  Serial.print("pwm_right_value:  ");    Serial.println(pwm_right_value);
  
  switch(which_car)                      //运动状态控制
  {
     case RUN:
      Serial.println("RUN"); //前进输出状态
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(b2, LOW);
      break;
     case BACK:
      Serial.println("BACK"); //后退输出状态
      digitalWrite(a1, LOW);
      digitalWrite(a2, HIGH);
      digitalWrite(b1, LOW);
      digitalWrite(b2, HIGH);
      break;

     case RIGHT:
      Serial.println("LEFT"); //左转输出状态
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(b2, LOW);
      break;
     case LEFT:
      Serial.println("RIGHT"); //右转弯输出状态
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
      digitalWrite(b1, HIGH);
      digitalWrite(b2, LOW);
      break;


     default:
      Serial.println("STOP"); //停止输出状态
      digitalWrite(a1, LOW);
      digitalWrite(a2, LOW);
      digitalWrite(b1, LOW);
      digitalWrite(b2, LOW);
      break;
  }
}
