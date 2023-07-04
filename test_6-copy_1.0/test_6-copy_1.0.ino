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
#define small_pwm full*1.5
#define big_pwm   full*1.7
#define go        full*1
#define go_p      full*0.6

int hw_in[4];
int hw_xiuzheng[4];


#define XZ_LEFT       'm'
#define XZ_RIGHT      'n'
#define XZ_LEFT_BIG   'x'
#define XZ_RIGHT_BIG  'y'

int s = 0;

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
//  read_hongwai();
car_work(STOP,100,100);
}


void read_hongwai()
{
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

  if(hw_in[1]==1)                               /*左边有线，右偏-左转*/
  {
    Serial.println("小幅度-右偏");
    car_work(LEFT, go_p, small_pwm);
    xiuzheng(XZ_LEFT);
  }
  else if(hw_in[0]==1)                               /*左边有线，右偏-左转*/
  {
    Serial.println("大幅度-右偏");
//    car_work(LEFT, go_p, big_pwm);
    xiuzheng(XZ_LEFT_BIG);         // x
  }
  
  else if(hw_in[2]==1)                               /*右边有线，左偏-右转*/
  {
    Serial.println("小幅度-左偏");
    car_work(RIGHT, small_pwm, go_p);
    xiuzheng(XZ_RIGHT);            
  }
  else if(hw_in[3]==1)                               /*右边有线，左偏-右转*/
  {
    Serial.println("大幅度-左偏");
//    car_work(RIGHT, big_pwm, go_p);
    xiuzheng(XZ_RIGHT_BIG);          //y
  }

  else if(!hw_in[1] && !hw_in[2])
  {
    Serial.println("正轨");
    car_work(RUN, go, go);
  } 
}

void xiuzheng(char xiuzheng_mode)
{
  switch(xiuzheng_mode)
  {
  
    case 'm':
      do
      {
        Serial.println("小幅度-右偏");
        Serial.print("m开始修正：  ");
        s++;
        Serial.println(s);
        hw_xiuzheng[1] = digitalRead(sensor2);
        hw_xiuzheng[2] = digitalRead(sensor3);
        Serial.print("hw_xiuzheng[1]: ");Serial.print(hw_xiuzheng[1]);Serial.print("\t");
        Serial.print("hw_xiuzheng[2]: ");Serial.println(hw_xiuzheng[2]);
        car_work(LEFT, go_p, small_pwm);  
      }while(!(hw_xiuzheng[1]==0 && hw_xiuzheng[2]==0));
      s = 0;
      Serial.print("hw_xiuzheng[1]: ");Serial.print(hw_xiuzheng[1]);Serial.print("\t");
      Serial.print("hw_xiuzheng[2]: ");Serial.println(hw_xiuzheng[2]);
      Serial.println("m修正完成");
     break;


     case 'n':
        do
        {
          Serial.println("小幅度-左偏");
          Serial.print("n开始修正：  ");
          s++;
          Serial.println(s);
          hw_xiuzheng[1] = digitalRead(sensor2);
          hw_xiuzheng[2] = digitalRead(sensor3);
          Serial.print("hw_xiuzheng[1]: ");Serial.print(hw_xiuzheng[1]);Serial.print("\t");
          Serial.print("hw_xiuzheng[2]: ");Serial.println(hw_xiuzheng[2]);
          car_work(RIGHT, go_p, small_pwm);  
        }while(!(hw_xiuzheng[1]==0 && hw_xiuzheng[2]==0));
        s = 0;
        Serial.print("hw_xiuzheng[1]: ");Serial.print(hw_xiuzheng[1]);Serial.print("\t");
        Serial.print("hw_xiuzheng[2]: ");Serial.println(hw_xiuzheng[2]);
        Serial.println("n修正完成");
     break;


     case 'x':
        do
        {
          Serial.println("大幅度-右偏");
          Serial.print("x开始修正：  ");
          s++;
          Serial.println(s);
          hw_xiuzheng[0] = digitalRead(sensor1);
          hw_xiuzheng[1] = digitalRead(sensor2);
          Serial.print("hw_xiuzheng[0]: ");Serial.print(hw_xiuzheng[0]);Serial.print("\t");
          Serial.print("hw_xiuzheng[1]: ");Serial.println(hw_xiuzheng[1]);
          car_work(RIGHT, go_p, small_pwm);  
        }while(hw_xiuzheng[1]==0);
        s = 0;
        Serial.print("hw_xiuzheng[0]: ");Serial.print(hw_xiuzheng[0]);Serial.print("\t");
        Serial.print("hw_xiuzheng[1]: ");Serial.println(hw_xiuzheng[1]);
        Serial.println("x修正完成");
     break;


     case 'y':
        do
        {
          Serial.println("大幅度-左偏");
          Serial.print("y开始修正：  ");
          s++;
          Serial.println(s);
          hw_xiuzheng[2] = digitalRead(sensor3);
          hw_xiuzheng[3] = digitalRead(sensor4);
          Serial.print("hw_xiuzheng[2]: ");Serial.print(hw_xiuzheng[2]);Serial.print("\t");
          Serial.print("hw_xiuzheng[3]: ");Serial.println(hw_xiuzheng[3]);
          car_work(LEFT, go_p, small_pwm);  
        }while(hw_xiuzheng[2]==0);
        s = 0;
        Serial.print("hw_xiuzheng[2]: ");Serial.print(hw_xiuzheng[2]);Serial.print("\t");
        Serial.print("hw_xiuzheng[3]: ");Serial.println(hw_xiuzheng[3]);
        Serial.println("y修正完成");
     break;

      default:break;
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
      Serial.println("RIGHT"); //左转输出状态
      digitalWrite(a1, LOW);
      digitalWrite(a2, HIGH);
      digitalWrite(b1, HIGH);
      digitalWrite(b2, LOW);
      break;
     case LEFT:
      Serial.println("LEFT"); //右转弯输出状态
      digitalWrite(a1, HIGH);
      digitalWrite(a2, LOW);
      digitalWrite(b1, LOW);
      digitalWrite(b2, HIGH);
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
