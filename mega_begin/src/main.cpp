#include <Arduino.h>
#include <Servo.h>

#define myservo_x_pin 8
#define myservo_y_pin 9
#define rest 43//按键-舵机复位

Servo myservo_x;//x轴舵机  
Servo myservo_y; 



int fromPos; //建立变量，存储电机起始运动角度值
int moveStep = 3;  // 每一次按下手柄按键，舵机移动量（仅适用于手柄模式）   
int baseJoyPos;
int rArmJoyPos;
int DSD = 15; //Default Servo Delay (默认电机运动延迟时间)
              //此变量用于控制电机运行速度.增大此变量数值将
              //降低电机运行速度从而控制机械臂动作速度。
//存储电机极限值(const指定该数值为常量,常量数值在程序运行中不能改变)
const int x_baseMin = 0;
const int x_baseMax = 180;
const int y_rArmMin = 45;
const int y_rArmMax = 180;


/*以下，函数声明*/
void servoCmd(char servoName, int toPos, int servoDelay);//指挥电机运行
void reportStatus();  //舵机状态信息
/*以上，函数声明*/




void setup() {
  Serial.begin(9600);

  pinMode(rest,INPUT_PULLUP);//舵机复位键上拉

  myservo_x.attach(myservo_x_pin);//舵机连接位置 
  myservo_y.attach(myservo_y_pin);

  myservo_x.write(90);//舵机起始位置
  delay(10);
  myservo_y.write(90); 
  delay(10);
  Serial.print("myservo_x: "); Serial.println(myservo_x.read());
  Serial.print("myservo_y: "); Serial.println(myservo_y.read());
  Serial.println("程序开始启动!!!");
}



void loop() {

char serialCmd='x';
char servoData=0;

  if(digitalRead(rest)==LOW)
  {
   
    Serial.println("1");
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    int flag_rest=1;
    if(digitalRead(rest)==LOW)
    {
      Serial.println("2");
      if(digitalRead(rest)==LOW)
      {
        Serial.println("3");
        myservo_x.write(150);//舵机起始位置
        delay(10);
        myservo_y.write(0); 
        delay(10);
      }
    while(flag_rest);
    } 
  }
  else
  {
    reportStatus();
    servoCmd(serialCmd, servoData, DSD);
  }
}




void servoCmd(char servoName, int toPos, int servoDelay)//指挥电机运行
{
  Servo servo2go;

  //串口监视器输出接收指令信息
  Serial.println("");
  Serial.print("舵机编号: ");
  Serial.print(servoName);
  Serial.print("目标值: ");
  Serial.print(toPos);
  Serial.print("等待舵机到达: ");
  Serial.println(servoDelay);


    switch(servoName){
    case 'x':
      if(toPos >= x_baseMin && toPos <= x_baseMax){
        servo2go = myservo_x;
        fromPos = myservo_x.read();  // 获取当前电机角度值用于“电机运动起始角度值”
        break;
      } else {
        Serial.println("+Warning: x_Base舵机角度超范围!");
        return;
      }

    case 'y':
      if(toPos >= y_rArmMin && toPos <= y_rArmMax){    
        servo2go = myservo_y;
        fromPos = myservo_y.read();  // 获取当前电机角度值用于“电机运动起始角度值”
        break;
      } else {
        Serial.println("+Warning: y_Claw舵机角度超范围!");
        return;        
      }
    }
 
  //指挥电机运行
  if (fromPos <= toPos){  //如果“起始角度值”小于“目标角度值”
    for (int i=fromPos; i<=toPos; i++){
      servo2go.write(i);
      delay (servoDelay);
    }
  }  else { //否则“起始角度值”大于“目标角度值”
    for (int i=fromPos; i>=toPos; i--){
      servo2go.write(i);
      delay (servoDelay);
    }
  }
}




void reportStatus(){  //舵机状态信息
  Serial.println("+ Robot-Arm Status Report +");
  Serial.print("myservo_x: "); Serial.println(myservo_x.read());
  Serial.print("myservo_y: "); Serial.println(myservo_y.read());
  Serial.println("++++++++++++++++++++++++++");
  Serial.println("");
}
 




/*以下，一点一点移动*/
// void armJoyCmd(char serialCmd)//Arduino根据手柄按键执行相应操作
// {
//    switch(serialCmd){
//     case 'a':  // x_Base向左
//       Serial.println("x_Base向左");                
//       baseJoyPos = myservo_x.read() - moveStep;
//       servoCmd('x', baseJoyPos, DSD);
//       break;  
      
//     case 'd':  // x_Base向右
//       Serial.println("x_Base向右");                
//       baseJoyPos = myservo_x.read() + moveStep;
//       servoCmd('x', baseJoyPos, DSD);
//       break;        
 
//     case 's':  // y_rArm向下
//     Serial.println("y_rArm向下");                
//       rArmJoyPos = myservo_y.read() + moveStep;
//       servoCmd('y', rArmJoyPos, DSD);
//       break;  
                 
//     case 'w':  // y_rArm向上
//       Serial.println("y_rArm向上");     
//       rArmJoyPos = myservo_y.read() - moveStep;
//       servoCmd('y', rArmJoyPos, DSD);
//       break;  
//   }
// } 
/*以上，一点一点移动*/