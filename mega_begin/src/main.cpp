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




/*以下，法1函数声明*/
const int x_baseMin = 0;
const int x_baseMax = 180;
const int y_rArmMin = 0;
const int y_rArmMax = 180;
void armDataCmd(char serialCmd,char servoData_small, int DSD_small);/*以下，法1，直接到位*/
void armJoyCmd(char serialCmd);                                     /*以下，法1，一点一点移动*/
void button_fine_tuning();                                          /*以下，法1，按键移动调试*/
void servoCmd(char servoName, int toPos, int servoDelay);           /*以下，法1，指挥电机运行*/
void OLED_reportStatus();                                           /*以下，法1，OELD获取当前舵机信息*/
/*以上，法1函数声明*/




/*以下，法2函数声明*/
int writeMic_fromPos;         //建立变量，存储电机起始运动角度值-输入
int writeMic_fromPos_change;  //建立变量，存储电机起始运动脉冲值-转换输出
int writeMic_moveStep=10;     //调整程度
int writeMic_baseJoyPos;      //调整输出
void writeMicroseconds_armDataCmd(char serialCmd,int servoData_small, int DSD_small);/*以下，法2，直接到位*/
void writeMicroseconds_armJoyCmd(int serialCmd);                                      /*以下，法2，一点点移动*/
void writeMicroseconds_button_fine_tuning();                                          /*以下，法2，按键移动调试*/
void writeMic_servoCmd(char servoName, int toPos, int servoDelay);                    /*以下，法2，指挥舵机运行*/
void writeMic_OLED_reportStatus();                                                    /*以下，法2，OELD获取当前舵机信息*/
/*以上，法2函数声明*/









/*#################以下，主程序开始#################*/
void setup() {
  //Serial.begin(9600);
  
  u8g2.begin();              // 初始化演示器
  u8g2.setColorIndex(1);

  pinMode(rest,INPUT_PULLUP);//舵机复位键上拉
  pinMode(x_up,INPUT_PULLUP);//x-上
  pinMode(x_down,INPUT_PULLUP);//x-下
  pinMode(y_up,INPUT_PULLUP);//y-上
  pinMode(y_down,INPUT_PULLUP);//y-下

  pinMode(test_now,INPUT_PULLUP);


  myservo_x.attach(myservo_x_pin,500,2500);//舵机连接位置/*此处更改了默认的180-2400*/
  myservo_y.attach(myservo_y_pin,500,2500);

  myservo_x.write(0);//舵机起始位置
  delay(10);
  myservo_y.write(0); 
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
  armDataCmd('x', 89, DSD);
  armDataCmd('y', 90, DSD);

  ///*调试*/  Serial.println("程序初始化完成");
}


void loop() {
/*((((((((((((((((((((((((以下，法1调试))))))))))))))))))))))))*/
  // armDataCmd('x', 87, DSD);
  // armDataCmd('y', 90, DSD);
  //  while(digitalRead(test_now) != LOW);

  // armDataCmd('x', 105, DSD);
  // armDataCmd('y', 106, DSD);
  // armDataCmd('x', 75, DSD);
  // armDataCmd('y', 76, DSD);
  //  armDataCmd('x', 87+7-1, DSD);/*A4纸*/
  //  armDataCmd('y', 90+9-1, DSD);/*A4纸*/
  // while(1);
  //button_fine_tuning();/*按键移动*/
/*((((((((((((((((((((((((以上，法1调试))))))))))))))))))))))))*/

/*{{{{{{{{{{{{{{{{{{{{{{{{以下，法2调试}}}}}}}}}}}}}}}}}}}}}}}}*/

   while(digitalRead(test_now) != LOW){;}

    writeMicroseconds_armDataCmd('x', 1000, DSD);/*A4纸*/

    writeMicroseconds_armDataCmd('y', 2000, DSD);/*A4纸*/

/*{{{{{{{{{{{{{{{{{{{{{{{{以上，法2调试}}}}}}}}}}}}}}}}}}}}}}}}*/
}
/*#################以上，主程序开始#################*/






/*!!!!!!!!!!!!!!!!!!!!!!以下，法1汇总!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*以下，法1，指挥电机运行*/
void servoCmd(char servoName, int toPos, int servoDelay)
{
  Servo servo2go;


    switch(servoName){
    case 'x':
      if(toPos >= x_baseMin && toPos <= x_baseMax){
        servo2go = myservo_x;
        fromPos = myservo_x.read();  // 获取当前电机角度值用于“电机运动起始角度值”
        break;
      } else {
        //Serial.println("+Warning: x_Base舵机角度超范围!");
        return;
      }

    case 'y':
      if(toPos >= y_rArmMin && toPos <= y_rArmMax){    
        servo2go = myservo_y;
        fromPos = myservo_y.read();  // 获取当前电机角度值用于“电机运动起始角度值”
        break;
      } else {
        //Serial.println("+Warning: y_Claw舵机角度超范围!");
        return;        
      }
    }


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
/*以上，法1，指挥电机运行*/



/*以下，法1，一点一点移动*/
void armJoyCmd(char serialCmd)//Arduino根据手柄按键执行相应操作
{
   switch(serialCmd){
    case 'a':  // x_Base向左
      //Serial.println("x_Base向左");                
      baseJoyPos = myservo_x.read() - moveStep;
      servoCmd('x', baseJoyPos, DSD);
      break;  
      
    case 'b':  // x_Base向右
      //Serial.println("x_Base向右");                
      baseJoyPos = myservo_x.read() + moveStep;
      servoCmd('x', baseJoyPos, DSD);
      break;        
 
    case 's':  // y_rArm向下
    //Serial.println("y_rArm向下");                
      rArmJoyPos = myservo_y.read() + moveStep;
      servoCmd('y', rArmJoyPos, DSD);
      break;  
                 
    case 'w':  // y_rArm向上
      //Serial.println("y_rArm向上");     
      rArmJoyPos = myservo_y.read() - moveStep;
      servoCmd('y', rArmJoyPos, DSD);
      break;  
  }
  OLED_reportStatus();
} 
/*以下，法1，一点一点移动*/



/*以下，法1，直接到位*/
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
    servoCmd(serialCmd, servoData_small, DSD_small);
  }
}                                 
/*以上，法1，直接到位*/



/*以下，法1，按键移动调试*/
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
/*以上，法1，按键移动调试*/



/*以下，法1，OELD获取当前舵机信息*/
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
/*以上，法1，OELD获取当前舵机信息*/
/*!!!!!!!!!!!!!!!!!!!!!!以上，法1汇总!!!!!!!!!!!!!!!!!!!!!!!!!*/












/*@@@@@@@@@@@@@@@@@@@@@@@以下，法2汇总@@@@@@@@@@@@@@@@@@@@@@@*/
/*以下，法2，指挥舵机运行*/
void writeMic_servoCmd(char servoName, int toPos, int servoDelay)//指挥电机运行
{
  Servo servo2go;

    ///*调试*/  Serial.println("6");
    switch(servoName){
    case 'x':
    ///*调试*/  Serial.println("7");
      if(toPos >= 500 && toPos <= 2500){
         ///*调试*/  Serial.println("8");
        servo2go = myservo_x;
        writeMic_fromPos = myservo_x.read();  // 获取当前电机角度值用于“电机运动起始角度值”
        writeMic_fromPos_change=map(writeMic_fromPos,0,180,500,2500);
        break;
      } else {
        ///*调试*/Serial.println("+Warning: x_Base舵机角度超范围!");
        return;
      }

    case 'y':
      if(toPos >= 500 && toPos <= 2500){    
        servo2go = myservo_y;
        writeMic_fromPos = myservo_y.read();  // 获取当前电机角度值用于“电机运动起始角度值”
        writeMic_fromPos_change=map(writeMic_fromPos,0,180,500,2500);
        break;
      } else {
        //Serial.println("+Warning: y_Claw舵机角度超范围!");
        return;        
      }
    }

  //指挥电机运行
  if (fromPos <= toPos)
  {  //如果“起始角度值”小于“目标角度值”
    for (int i=fromPos; i<=toPos; i+=10){
      servo2go.writeMicroseconds(i);
      delay (servoDelay);
    }
  }  
  else 
  { //否则“起始角度值”大于“目标角度值”
    for (int i=fromPos; i>=toPos; i-=10){
      servo2go.writeMicroseconds(i);
      delay (servoDelay);
    }
  }

  /*以下，显示*/
  int endx=myservo_x.read();
  int endy=myservo_y.read();

  int end_x=map(endx,0,180,500,2500);
  int end_y=map(endy,0,180,500,2500);
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
        /*以上，显示*/
}
/*以上，法2，指挥舵机运行*/



/*以下，法2，一点点移动*/
void writeMicroseconds_armJoyCmd(int serialCmd)
{
   int writeMic_baseJoyPos_int_x=myservo_x.read();//获取当前角度
   int writeMic_baseJoyPos_change_x=map(writeMic_baseJoyPos_int_x,0,180,500,2500);//角度转换为脉冲

   int writeMic_baseJoyPos_int_y=myservo_y.read();//获取当前角度
   int writeMic_baseJoyPos_change_y=map(writeMic_baseJoyPos_int_y,0,180,500,2500);//角度转换为脉冲

   switch(serialCmd){
      case 'a':  // x_Base向左
        //Serial.println("x_Base向左");                
        writeMic_baseJoyPos = writeMic_baseJoyPos_change_x - writeMic_moveStep;//调整输出
        writeMic_servoCmd('x', writeMic_baseJoyPos, DSD);//开始调整
      break; 

      case 'b':  // x_Base向右
        //Serial.println("x_Base向右");                
        writeMic_baseJoyPos = writeMic_baseJoyPos_change_x + writeMic_moveStep;
        writeMic_servoCmd('x', writeMic_baseJoyPos, DSD);
      break;        
 
      case 's':  // y_rArm向下
        //Serial.println("y_rArm向下");                
        writeMic_baseJoyPos = writeMic_baseJoyPos_change_y + moveStep;
        writeMic_servoCmd('y', writeMic_baseJoyPos, DSD);
      break;  
                 
      case 'w':  // y_rArm向上
        //Serial.println("y_rArm向上");     
        writeMic_baseJoyPos = writeMic_baseJoyPos_change_y - moveStep;
        writeMic_servoCmd('y', writeMic_baseJoyPos, DSD);
      break;  
  }
  OLED_reportStatus(); 
}
/*以上，法2，一点点移动*/




/*以下，法2，按键移动调试*/
void writeMicroseconds_button_fine_tuning()
{
  
  if(digitalRead(x_up)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(x_up)==LOW)
    {   
      writeMicroseconds_armJoyCmd('b');
    }
  }
  if(digitalRead(x_down)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(x_down)==LOW)
    {   
      writeMicroseconds_armJoyCmd('a');
    }
  }



  if(digitalRead(y_up)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(y_up)==LOW)
    {   
      writeMicroseconds_armJoyCmd('s');
    }
  }
  if(digitalRead(y_down)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(y_down)==LOW)
    {   
      writeMicroseconds_armJoyCmd('w');
    }
  }
}
/*以上，法2，按键移动调试*/



/*以下，法2，直接到位*/
void writeMicroseconds_armDataCmd(char serialCmd,int servoData_small, int DSD_small)//Arduino根据串行指令执行相应操作
{                              //指令示例：b45 底盘转到45度角度位置
                               //          o 输出机械臂舵机状态信息 
  ///*调试*/  Serial.println("4");
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
        int myservo_xvlue=myservo_x.read();
        int myservo_yvlue=myservo_y.read();
        int myservo_x_vlue=map(myservo_xvlue,0,180,500,2500);
        int myservo_y_vlue=map(myservo_yvlue,0,180,500,2500);
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
    ///*调试*/  Serial.println("5");
    writeMic_servoCmd(serialCmd, servoData_small, DSD_small);
  }
}                                 
/*以上，法2，直接到位*/



/*以下，法2，OELD获取当前舵机信息*/
 void writeMic_OLED_reportStatus()
 {
  int showx=myservo_x.read();
  int showy=myservo_y.read();
  int show_x=map(showx,0,180,500,2500);
  int show_y=map(showy,0,180,500,2500);
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
 /*以下，法2，OELD获取当前舵机信息*/
/*@@@@@@@@@@@@@@@@@@@@@@@以上，法2汇总@@@@@@@@@@@@@@@@@@@@@@@*/








