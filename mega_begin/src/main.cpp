#include <Arduino.h>
#include <Servo.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/35, /* data=*/34, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display


#define myservo_x_pin 8
#define myservo_y_pin 9
#define rest 3//按键-舵机复位
#define  x_up   38
#define  x_down 41
#define  y_up   39
#define  y_down 42

#define test_now 40

Servo myservo_x;//x轴舵机  
Servo myservo_y; 



int fromPos; //建立变量，存储电机起始运动角度值
int moveStep = 1;  // 每一次按下手柄按键，舵机移动量（仅适用于手柄模式）   
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
void armDataCmd(char serialCmd,char servoData_small, int DSD_small);//Arduino根据串行指令执行相应操作/*以下，直接到位*/
void servoCmd(char servoName, int toPos, int servoDelay);//指挥电机运行
void armJoyCmd(char serialCmd);//Arduino根据手柄按键执行相应操作/*以下，一点一点移动*/
void button_fine_tuning();/*按键移动*/
void OLED_reportStatus();/*OELD获取当前舵机信息*/
/*以上，函数声明*/


/*以下，获取当前脉冲*/
void get_angle();/*获取当前脉冲*/
int get_angle_x;//当前脉冲
int get_angle_y;
/*以上，获取当前脉冲*/


/*以下，复位*/
void interruptFunction();
volatile bool flag = 0;
/*以上，复位*/


/*++++++++++++++++++++++++++++++以下，任务+++++++++++++++++++++++++++++*/
void Task_2();/*任务2*/
void Task_3();/*任务3*/
/*++++++++++++++++++++++++++++++以上，任务+++++++++++++++++++++++++++++*/

// int start_x=1410;
// int start_y=1330;
int start_x=1470;//原点位置
int start_y=1414;

/*以下，新添*/
void contal_pulse(char servoName,int last_pos,int toPos,int servoDelay);/*直接到位*/
void contrl_reading(char serialCmd,int servoDelay); /*一点一点动*/
void writeMicroseconds_button_fine_tuning();        /*以下，法2，按键移动调试*/
int contrl_reading_baseJoyPos_x=start_x;
int contrl_reading_baseJoyPos_y=start_y;
int contrl_reading_moveStep=3;
/*以上，新添*/


volatile bool flag_rest=0;




void setup() {
  // Serial.begin(9600);
  pinMode(3, INPUT_PULLUP); // 设置引脚3为输入模式
  pinMode(48,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), interruptFunction, FALLING);


  u8g2.begin();              // 初始化演示器
  u8g2.setColorIndex(1);

  pinMode(rest,INPUT_PULLUP);//舵机复位键上拉
  pinMode(x_up,INPUT_PULLUP);//x-上
  pinMode(x_down,INPUT_PULLUP);//x-下
  pinMode(y_up,INPUT_PULLUP);//y-上
  pinMode(y_down,INPUT_PULLUP);//y-下

  pinMode(test_now,INPUT_PULLUP);//y-下


  myservo_x.attach(myservo_x_pin,500,2500);//舵机连接位置/*此处更改了默认的180-2400*/
  myservo_y.attach(myservo_y_pin,500,2500);

  myservo_x.write(0);//舵机起始位置
  delay(10);
  myservo_y.write(0); 
  delay(10);
  delay(3000);



  myservo_x.write(87);//舵机起始位置
  delay(10);
  myservo_y.write(76); 
  delay(10);
  
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
  // do//自动回正
  // {
  //   for(int i=0;i<start_x;i+=10)
  //   {
  //     myservo_x.writeMicroseconds(i);
  //     delay(15);
  //   }
  //   for(int i=0;i<start_y;i+=10)
  //   {
  //     myservo_y.writeMicroseconds(i);
  //     delay(15);
  //   }
  //   delay(500);
  // } while (0);
  
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(20, 40, "+++GO+++");
        } while (u8g2.nextPage());
  // contal_pulse('x',1260,DSD);/*直接到位*/
  // contal_pulse('y',1470,DSD);/*直接到位*/
}


 



 

void loop() {
  // armDataCmd('x', 87, DSD);
  // armDataCmd('y', 90, DSD);
  // while(digitalRead(test_now) != LOW);

  armDataCmd('x', 101, DSD);
  armDataCmd('y', 65, DSD);
  
  armDataCmd('x', 73, DSD);
  
  armDataCmd('y', 88, DSD);
  
  armDataCmd('x', 100, DSD);
  armDataCmd('y', 76, DSD);
  // button_fine_tuning();/*按键移动*/







  // get_angle();//获取当前脉冲
  // contal_pulse('x',1260,DSD);/*直接到位*/
  // contal_pulse('y',1470,DSD);/*直接到位*/
  // contal_pulse('x',1560,DSD);/*直接到位*/
  // contal_pulse('y',1193,DSD);/*直接到位*/
  // while(1);
  // Rest_origin();/*自动回中*/


  //Task_2();/*任务2*/
  // Task_3();/*任务3*/
  // writeMicroseconds_button_fine_tuning();/*以下，法2，按键移动调试*/

}




/*？？？？？？？？？？？？？？？？？？？以上，法1，读数控制？？？？？？？？？？？？？？？？？？？？？？？？*/
/*以下，法1，读数控制*/
void servoCmd(char servoName, int toPos, int servoDelay)//指挥电机运行
{
  Servo servo2go;

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
    servoCmd(serialCmd, servoData_small, DSD_small);
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
/*？？？？？？？？？？？？？？？？？？？以上，法1，读数控制？？？？？？？？？？？？？？？？？？？？？？？？*/

















/*##########################以下，法2，脉冲控制###########################*/
/*直接到位*/
void contal_pulse(char servoName,int last_pos,int toPos,int servoDelay)
{
    switch(servoName){
    case 'x':
      if(toPos >= 500 && toPos <= 2500){
        if(toPos>last_pos)
        {
          for(int i=last_pos;i<toPos;i+=5)
          {
            while(flag_rest);
            myservo_x.writeMicroseconds(i);
            delay(servoDelay-5);
          }
        }
        else if(toPos<last_pos)
        {
          for(int i=last_pos;i>toPos;i-=5)
          {
            while(flag_rest);
            myservo_x.writeMicroseconds(i);
            delay(servoDelay-5);
          }
        }


/*以下，显示*/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "contrl_pulse");
          u8g2.drawStr(0,25, "cp_x");
          u8g2.setCursor(sizeof("cp_x: ") * 6, 25);
          u8g2.print(toPos);
        } while (u8g2.nextPage());
/*以上，显示*/
        break;
      } else {
        Serial.println("+Warning: x_Base舵机角度超范围!");
        return;
      }

    case 'y':
      if(toPos >= 500 && toPos <= 2500){ 
        if(toPos>last_pos)
        {
          for(int i=last_pos;i<toPos;i+=5)
          {
            while(flag_rest);
            myservo_y.writeMicroseconds(i);
            delay(servoDelay-5);
          }
        }
        else if(toPos<last_pos)
        {
          for(int i=last_pos;i>toPos;i-=5)
          {
            while(flag_rest);
            myservo_y.writeMicroseconds(i);
            delay(servoDelay-5);
          }
        }   
/*以下，显示*/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "contrl_reading");
          u8g2.drawStr(0,25, "cr_y");
          u8g2.setCursor(sizeof("cr_y: ") * 6, 25);
          u8g2.print(toPos);
        } while (u8g2.nextPage());
/*以上，显示*/
        break;
      } else {
        Serial.println("+Warning: y_Claw舵机角度超范围!");
        return;        
      }
    }
}

/*一点一点动*/
void contrl_reading(char serialCmd,int servoDelay)
{
  
   switch(serialCmd){
    case 'a':  // x_Base向左
        Serial.println("x_Base向左");                
      contrl_reading_baseJoyPos_x += contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_x>2500)  contrl_reading_baseJoyPos_x=2500;
            if(contrl_reading_baseJoyPos_x<500)  contrl_reading_baseJoyPos_x=500;
      myservo_x.writeMicroseconds(contrl_reading_baseJoyPos_x);
      delay(servoDelay);
            Serial.print("x_Base向左: ");Serial.println(contrl_reading_baseJoyPos_x); 
            Serial.println("x_Base向左###");
/*以下，显示*/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "contrl_reading");
          u8g2.drawStr(0,25, "cr_x");
          u8g2.setCursor(sizeof("cr_x: ") * 6, 25);
          u8g2.print(contrl_reading_baseJoyPos_x);
        } while (u8g2.nextPage());
/*以上，显示*/
      break;  
      
    case 'b':  // x_Base向右
        Serial.println("x_Base向右");                
      contrl_reading_baseJoyPos_x -= contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_x>2500)  contrl_reading_baseJoyPos_x=2500;
            if(contrl_reading_baseJoyPos_x<500)  contrl_reading_baseJoyPos_x=500;
      myservo_x.writeMicroseconds(contrl_reading_baseJoyPos_x);
      delay(servoDelay);
            Serial.print("x_Base向右: ");Serial.println(contrl_reading_baseJoyPos_x); 
            Serial.println("x_Base向右###"); 
/*以下，显示*/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "contrl_reading");
          u8g2.drawStr(0,25, "cr_x");
          u8g2.setCursor(sizeof("cr_x: ") * 6, 25);
          u8g2.print(contrl_reading_baseJoyPos_x);
        } while (u8g2.nextPage());
/*以上，显示*/
      break;        
 
    case 's':  // y_rArm向下
        Serial.println("y_rArm向下");                
      contrl_reading_baseJoyPos_y += contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_y>2500)  contrl_reading_baseJoyPos_y=2500;
            if(contrl_reading_baseJoyPos_y<500)  contrl_reading_baseJoyPos_y=500;
      myservo_y.writeMicroseconds(contrl_reading_baseJoyPos_y);
      delay(servoDelay);
            Serial.print("y_rArm向下: ");Serial.println(contrl_reading_baseJoyPos_y);
            Serial.println("y_rArm向下###");
/*以下，显示*/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "contrl_reading");
          u8g2.drawStr(0,40, "cr_y");
          u8g2.setCursor(sizeof("cr_y: ") * 6, 40);
          u8g2.print(contrl_reading_baseJoyPos_y);
        } while (u8g2.nextPage());
/*以上，显示*/
      break;  
                 
    case 'w':  // y_rArm向上
        Serial.println("y_rArm向上");     
      contrl_reading_baseJoyPos_y -= contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_y>2500)  contrl_reading_baseJoyPos_y=2500;
            if(contrl_reading_baseJoyPos_y<500)  contrl_reading_baseJoyPos_y=500;
      myservo_y.writeMicroseconds(contrl_reading_baseJoyPos_y);
      delay(servoDelay);
            Serial.print("y_rArm向上: ");Serial.println(contrl_reading_baseJoyPos_y);
            Serial.println("y_rArm向上###"); 
/*以下，显示*/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "contrl_reading");
          u8g2.drawStr(0,40, "cr_y");
          u8g2.setCursor(sizeof("cr_y: ") * 6, 40);
          u8g2.print(contrl_reading_baseJoyPos_y);
        } while (u8g2.nextPage());
/*以上，显示*/
      break;  
  }
}

/*以下，法2，按键移动调试*/
void writeMicroseconds_button_fine_tuning()
{
  
  if(digitalRead(x_up)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(x_up)==LOW)
    {   
      // /*调试*/ Serial.println("(x_up)==LOW");
      contrl_reading('a',15);
    }
  }
  if(digitalRead(x_down)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(x_down)==LOW)
    {   
      contrl_reading('b',15);
    }
  }



  if(digitalRead(y_up)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(y_up)==LOW)
    {   
      contrl_reading('s',15);
    }
  }
  if(digitalRead(y_down)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(y_down)==LOW)
    {   
      contrl_reading('w',15);
    }
  }
}
/*以上，法2，按键移动调试*/
/*##########################以上，法2，脉冲控制###########################*/






/*任务2*/
void Task_2()
{
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 40, "---Task_2_STAR---");
        } while (u8g2.nextPage());
  // /*调试*/Serial.println("1");
  while(flag_rest);
  contal_pulse('x',start_x,1300,DSD);/*直接到位*//*调试*/Serial.println("2");contal_pulse('y',start_y,1275,DSD);/*直接到位*/
// /*调试*/Serial.println("3");
  while(flag_rest);
  contal_pulse('x',1300,1602,DSD);/*直接到位*/
// /*调试*/Serial.println("4");
  while(flag_rest);
  contal_pulse('y',1275,1542,DSD);/*直接到位*/
// /*调试*/Serial.println("5");
  while(flag_rest);
  contal_pulse('x',1602,1300,DSD);/*直接到位*/
// /*调试*/Serial.println("6");
  while(flag_rest);
  contal_pulse('y',1542,1402,DSD);/*直接到位*/
  while(flag_rest){;}
// /*调试*/Serial.println("7");
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 40, "+++Task_2_END+++");
        } while (u8g2.nextPage());
}
/*任务3*/
void Task_3()
{
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 40, "-33Task_3_STAR33-");
        } while (u8g2.nextPage());



  // /*调试*/Serial.println("1");
  while(flag_rest);
  contal_pulse('x',start_x,1360,DSD);/*直接到位*//*调试*/contal_pulse('y',start_y,1360,DSD);/*直接到位*/
  
// /*调试*/Serial.println("3");
  while(flag_rest);
  contal_pulse('x',1360,1545,DSD);/*直接到位*/
 
// /*调试*/Serial.println("4");
  while(flag_rest);
  contal_pulse('y',1360,1459,DSD);/*直接到位*/


// /*调试*/Serial.println("5");
  while(flag_rest);
  contal_pulse('x',1545,1360,DSD);/*直接到位*/
   

// /*调试*/Serial.println("6");
  while(flag_rest);
  contal_pulse('y',1459,1402,DSD);/*直接到位*/
  while(flag_rest){;}

  while(1){;}
// /*调试*/Serial.println("7");



        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 40, "-33Task_3_END33-");
        } while (u8g2.nextPage());

}


void interruptFunction()
{
  delay(500);
  if(digitalRead(rest)==LOW)
  {
    delay(500);
    if(digitalRead(rest)==LOW)
    {
      digitalWrite(48,digitalRead(48));
      myservo_x.writeMicroseconds(start_x);
      myservo_y.writeMicroseconds(start_y);
  /*以下，显示*/
          u8g2.firstPage();
          do
          {
            u8g2.setFont(u8g_font_7x14); // 设置字体
            u8g2.drawStr(20, 40, "@@@REST@@@");
          } while (u8g2.nextPage());
  /*以上，显示*/
      flag_rest=1;
    }
  }
}




/*获取当前脉冲*/
void get_angle()
{
  get_angle_x=map(myservo_x.read(),0,180,500,2500);
  get_angle_y=map(myservo_y.read(),0,180,500,2500);
/*以下，显示*/
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 10, "get_angle");
          u8g2.drawStr(0,25, "ga_x");
          u8g2.setCursor(sizeof("ga_x: ") * 6, 25);
          u8g2.print(get_angle_x);

          u8g2.drawStr(0,40, "ga_y");
          u8g2.setCursor(sizeof("ga_y: ") * 6, 40);
          u8g2.print(get_angle_y);

        } while (u8g2.nextPage());
/*以上，显示*/
}





