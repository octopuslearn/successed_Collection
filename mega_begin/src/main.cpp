#include <Arduino.h>
#include <Servo.h>

#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/35, /* data=*/34, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display




#define myservo_x_pin 8
#define myservo_y_pin 9
#define rest 43//按键-舵机复位
#define  x_up   38
#define  x_down 41
#define  y_up   39
#define  y_down 42


Servo myservo_x;//x轴舵机  
Servo myservo_y; 



int fromPos; //建立变量，存储电机起始运动角度值
int moveStep = 6;  // 每一次按下手柄按键，舵机移动量（仅适用于手柄模式）   
int baseJoyPos;
int rArmJoyPos;
int DSD = 15; //Default Servo Delay (默认电机运动延迟时间)
              //此变量用于控制电机运行速度.增大此变量数值将
              //降低电机运行速度从而控制机械臂动作速度。
//存储电机极限值(const指定该数值为常量,常量数值在程序运行中不能改变)
const int x_baseMin = 0;
const int x_baseMax = 180;
const int y_rArmMin = 0;
const int y_rArmMax = 180;





/*以下，函数声明*/
void reportStatus();  //舵机状态信息
void armDataCmd(char serialCmd,char servoData_small, int DSD_small);//Arduino根据串行指令执行相应操作/*以下，直接到位*/
void servoCmd(char servoName, int toPos, int servoDelay);//指挥电机运行
void armJoyCmd(char serialCmd);//Arduino根据手柄按键执行相应操作/*以下，一点一点移动*/
void button_fine_tuning();/*按键移动*/
void OLED_reportStatus();/*OELD获取当前舵机信息*/
/*以上，函数声明*/




void setup() {
  Serial.begin(9600);
  
  u8g2.begin();              // 初始化演示器
  u8g2.setColorIndex(1);

  pinMode(rest,INPUT_PULLUP);//舵机复位键上拉
  pinMode(x_up,INPUT_PULLUP);//x-上
  pinMode(x_down,INPUT_PULLUP);//x-下
  pinMode(y_up,INPUT_PULLUP);//y-上
  pinMode(y_down,INPUT_PULLUP);//y-下


  myservo_x.attach(myservo_x_pin);//舵机连接位置 
  myservo_y.attach(myservo_y_pin);

  myservo_x.write(90);//舵机起始位置
  delay(10);
  myservo_y.write(90); 
  delay(10);
  
  // Serial.print("myservo_x: "); Serial.println(myservo_x.read());
  // Serial.print("myservo_y: "); Serial.println(myservo_y.read());
  // Serial.println("程序开始启动!!!");
  int myservo_x_star=myservo_x.read();
  int myservo_y_star=myservo_y.read();

        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "###START###");

          u8g2.drawStr(20, 35, "x_sta: ");
          u8g2.setCursor(sizeof("x_sta: ") * 8, 35);
          u8g2.print(myservo_x_star);

          u8g2.drawStr(20, 55, "y_sta: ");
          u8g2.setCursor(sizeof("y_sta: ") * 8, 55);
          u8g2.print(myservo_y_star);


        } while (u8g2.nextPage());
}




void loop() {
  //armDataCmd('x', 30, DSD);
  button_fine_tuning();
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
  if (fromPos <= toPos)
  {  //如果“起始角度值”小于“目标角度值”
    for (int i=fromPos; i<=toPos; i++){
      servo2go.write(i);
      delay (servoDelay);
    }
  }  
  else 
  { //否则“起始角度值”大于“目标角度值”
    for (int i=fromPos; i>=toPos; i--){
      servo2go.write(i);
      delay (servoDelay);
    }
  }
  int end_x=myservo_x.read();
  int end_y=myservo_y.read();
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "!!!end!!!");

          u8g2.drawStr(20, 35, "end_x: ");
          u8g2.setCursor(sizeof("end_x: ") * 8, 35);
          u8g2.print(end_x);

          u8g2.drawStr(20, 55, "end_y: ");
          u8g2.setCursor(sizeof("end_y: ") * 8, 55);
          u8g2.print(end_y);


        } while (u8g2.nextPage());
}




void reportStatus(){  //舵机状态信息
  Serial.println("+ Robot-Arm Status Report +");
  Serial.print("myservo_x: "); Serial.println(myservo_x.read());
  Serial.print("myservo_y: "); Serial.println(myservo_y.read());
  Serial.println("++++++++++++++++++++++++++");
  Serial.println("");
}

/*OELD获取当前舵机信息*/
void OLED_reportStatus()
{
  int show_x=myservo_x.read();
  int show_y=myservo_y.read();
  /**以下，OLED显示**/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "show_x: ");
          u8g2.setCursor(sizeof("show_x: ") * 8, 10);
          u8g2.print(show_x);
          u8g2.drawStr(0, 40, "show_y: ");
          u8g2.setCursor(sizeof("show_y: ") * 8, 40);
          u8g2.print(show_y);
        } while (u8g2.nextPage());
/**以上，OLED显示**/
}
 




/*以下，一点一点移动*/
void armJoyCmd(char serialCmd)//Arduino根据手柄按键执行相应操作
{
   switch(serialCmd){
    case 'a':  // x_Base向左
      Serial.println("x_Base向左");                
      baseJoyPos = myservo_x.read() - moveStep;
      servoCmd('x', baseJoyPos, DSD);
      break;  
      
    case 'b':  // x_Base向右
      Serial.println("x_Base向右");                
      baseJoyPos = myservo_x.read() + moveStep;
      servoCmd('x', baseJoyPos, DSD);
      break;        
 
    case 's':  // y_rArm向下
    Serial.println("y_rArm向下");                
      rArmJoyPos = myservo_y.read() + moveStep;
      servoCmd('y', rArmJoyPos, DSD);
      break;  
                 
    case 'w':  // y_rArm向上
      Serial.println("y_rArm向上");     
      rArmJoyPos = myservo_y.read() - moveStep;
      servoCmd('y', rArmJoyPos, DSD);
      break;  
  }
  OLED_reportStatus();
} 
/*以上，一点一点移动*/



/*以下，直接到位*/
void armDataCmd(char serialCmd,char servoData_small, int DSD_small)//Arduino根据串行指令执行相应操作
{                              //指令示例：b45 底盘转到45度角度位置
                               //          o 输出机械臂舵机状态信息 
  if(digitalRead(rest)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    int flag_rest=1;
    if(digitalRead(rest)==LOW)
    {   
        myservo_x.write(150);//舵机起始位置
        delay(10);
        myservo_y.write(0); 
        delay(10);
        int myservo_x_vlue=myservo_x.read();
        int myservo_y_vlue=myservo_y.read();


/**以下，OLED显示**/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "x: ");
          u8g2.setCursor(sizeof("x: ") * 8, 10);
          u8g2.print(myservo_x_vlue);
          u8g2.drawStr(0, 40, "y: ");
          u8g2.setCursor(sizeof("y: ") * 8, 40);
          u8g2.print(myservo_y_vlue);
        } while (u8g2.nextPage());
/**以上，OLED显示**/
    while(flag_rest);
    } 
  }
  else
  {
    // reportStatus();
    servoCmd(serialCmd, servoData_small, DSD_small);
  }
}                                 
/*以上，直接到位*/



/*以下，按键移动*/
void button_fine_tuning()
{
  
  if(digitalRead(x_up)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(x_up)==LOW)
    {   
      armJoyCmd('b');
    }
  }
  if(digitalRead(x_down)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(x_down)==LOW)
    {   
      armJoyCmd('a');
    }
  }



  if(digitalRead(y_up)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(y_up)==LOW)
    {   
      armJoyCmd('s');
    }
  }
  if(digitalRead(y_down)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(y_down)==LOW)
    {   
      armJoyCmd('w');
    }
  }
}
/*以上，按键移动*/