/*
*复位键-D3外部中断3#define rest 3//按键-舵机复位
*
*
*
*/
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <VarSpeedServo.h> 
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/35, /* data=*/34, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display

#define myservo_x_pin 8//舵机引脚
#define myservo_y_pin 9
#define rest 3//按键-舵机复位
#define  x_up   38
#define  x_down 41
#define  y_up   39
#define  y_down 42


VarSpeedServo myservo_x;//x轴舵机  
VarSpeedServo myservo_y; 


/*(((((((((((((((((((((((((((((((((((以下，设点原点位置)))))))))))))))))))))))))))))))))))*/
int start_x=1491;//原点位置
int start_y=1427;
/*(((((((((((((((((((((((((((((((((((以上，设点原点位置)))))))))))))))))))))))))))))))))))*/
int DSD = 15; //Default Servo Delay (默认电机运动延迟时间)
              //此变量用于控制电机运行速度.增大此变量数值将
              //降低电机运行速度从而控制机械臂动作速度。
//存储电机极限值(const指定该数值为常量,常量数值在程序运行中不能改变)





/*++++++++++++++++++++++++++++++以下，任务+++++++++++++++++++++++++++++*/
void Task_2();/*任务2*/
void Task_3();/*任务3*/
void read_Task_3(int x,int y,int w,int h);/*任务3-从k210获取*/
/*++++++++++++++++++++++++++++++以上，任务+++++++++++++++++++++++++++++*/





/*以下，获取当前脉冲*/
void get_angle();/*获取当前脉冲*/
/*以上，获取当前脉冲*/


/*以下，复位*/
void interruptFunction();
volatile bool flag = 0;
volatile bool flag_rest=0;
/*以上，复位*/


/*以下，任务3-K210新添*/
void contal_pulse(char servoName,int last_pos,int toPos,int servoDelay);/*直接到位*/
void contrl_reading(char serialCmd,int servoDelay); /*一点一点动*/
void writeMicroseconds_button_fine_tuning();        /*以下，法2，按键移动调试*/
int contrl_reading_baseJoyPos_x=start_x;
int contrl_reading_baseJoyPos_y=start_y;
int contrl_reading_moveStep=1;
/*以下，任务3-K210新添*/


/*以下，读取K210值和发送-任务3*/
void ReceiveDate(uint8_t num); //接收函数,每次调用只接收四个数据包里准确的数据刷新进全局data数组，接收正确就消亡了，要再接收必须重新调用。
unsigned int data[4];
void sending_data(unsigned int First_data, unsigned int Second_data, unsigned int Third_data,unsigned int Four_data);
/*以下，读取K210值和发送-任务3*/

int lr_data;
int fb_data;
int x_data_r;int x_data_l;
int x_data_f;int x_data_b;
/*以上，读取K210值-任务3*/









/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&主程序开始&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void setup() {
  // Serial.begin(9600);
  Serial1.begin(115200);    // 初始化虚拟串口
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
  do//自动回正
  {
    for(int i=0;i<start_x;i+=10)
    {
      myservo_x.writeMicroseconds(i);
      delay(15);
    }
    for(int i=0;i<start_y;i+=10)
    {
      myservo_y.writeMicroseconds(i);
      delay(15);
    }
    delay(500);
  } while (0);
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
        // u8g2.firstPage();
        // do
        // {
        //   u8g2.setFont(u8g_font_7x14); // 设置字体
        //   u8g2.drawStr(0, 10, ".....loop.....");
        // } while (u8g2.nextPage());
//     ReceiveDate(4);            //规定只接收四个数据
//     Serial.print(data[0]);   Serial.print('\t');//打印第三个数据 Serial.print(data[0]);
//     Serial.print(data[1]);   Serial.print('\t');//打印第三个数据 
//     Serial.print(data[2]);   Serial.print('\t');//打印第三个数据 
//     Serial.println(data[3]);   //打印第三个数据 


// lr_data=data[2]/2/1.6*6.4;
// fb_data=data[3]/2/1.7*6.4;
// x_data_r=1512-lr_data;  x_data_l=1512+lr_data;
// x_data_f=1378-fb_data;  x_data_b=1378+fb_data;
// Serial.print("dlr_data: ");Serial.print(lr_data);//x
// Serial.print("fb_data: ");Serial.print(fb_data);//y
// Serial.print("x_data_r: ");Serial.print(x_data_r);Serial.print("x_data_l: ");Serial.print(x_data_l);
// Serial.print("x_data_f: ");Serial.print(x_data_f);Serial.print("x_data_b: ");Serial.println(x_data_b);


        // u8g2.firstPage();
        // do
        // {
        //   u8g2.setFont(u8g_font_7x14); // 设置字体
        //   u8g2.drawStr(0, 10, "data_out[i]:");
        //   u8g2.setCursor(0, 30);  u8g2.print(data_out[0]);
        //   u8g2.setCursor(30, 30);  u8g2.print(data_out[1]);
        //   u8g2.setCursor(50, 30);  u8g2.print(data_out[2]);
        //   u8g2.setCursor(70, 30);  u8g2.print(data_out[3]);
        // } while (u8g2.nextPage());
    
  //  read_Task_3(x_data_r,x_data_l,x_data_f,x_data_b);



  // get_angle();//获取当前脉冲
  // contal_pulse('x',1260,DSD);/*直接到位*/
  // contal_pulse('y',1470,DSD);/*直接到位*/
  // contal_pulse('x',1560,DSD);/*直接到位*/
  // contal_pulse('y',1193,DSD);/*直接到位*/
  // while(1);
  // Rest_origin();/*自动回中*/


  // Task_2();/*任务2*/
  // Task_3();/*任务3*/





  writeMicroseconds_button_fine_tuning();/*以下，法2，按键移动调试*/

}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&主程序开始&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/












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
      contrl_reading_baseJoyPos_x += contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_x>2500)  contrl_reading_baseJoyPos_x=2500;
            if(contrl_reading_baseJoyPos_x<500)  contrl_reading_baseJoyPos_x=500;
      myservo_x.writeMicroseconds(contrl_reading_baseJoyPos_x);
      delay(servoDelay);
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
      contrl_reading_baseJoyPos_x -= contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_x>2500)  contrl_reading_baseJoyPos_x=2500;
            if(contrl_reading_baseJoyPos_x<500)  contrl_reading_baseJoyPos_x=500;
      myservo_x.writeMicroseconds(contrl_reading_baseJoyPos_x);
      delay(servoDelay);
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
      contrl_reading_baseJoyPos_y += contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_y>2500)  contrl_reading_baseJoyPos_y=2500;
            if(contrl_reading_baseJoyPos_y<500)  contrl_reading_baseJoyPos_y=500;
      myservo_y.writeMicroseconds(contrl_reading_baseJoyPos_y);
      delay(servoDelay);
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
      contrl_reading_baseJoyPos_y -= contrl_reading_moveStep;
            if(contrl_reading_baseJoyPos_y>2500)  contrl_reading_baseJoyPos_y=2500;
            if(contrl_reading_baseJoyPos_y<500)  contrl_reading_baseJoyPos_y=500;
      myservo_y.writeMicroseconds(contrl_reading_baseJoyPos_y);
      delay(servoDelay);
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
      contrl_reading('w',15);
    }
  }
  if(digitalRead(y_down)==LOW)
  {
    long last_button_time=millis();
    while((millis()-last_button_time)<50);
    if(digitalRead(y_down)==LOW)
    {   
      contrl_reading('s',15);
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
/*任务3-从k210获取*/
void read_Task_3(int x_data_r,int x_data_l,int x_data_f,int x_data_b)
{
  do
  {
    sending_data(0x01,0x00,0x00,0x00);
  } while (0);
  
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 40, "-read_Task_3START-");
        } while (u8g2.nextPage());



  // /*调试*/Serial.println("1");
  while(flag_rest);
  contal_pulse('x',1512,x_data_r,DSD);/*直接到位*//*调试*/contal_pulse('y',1378,x_data_b,DSD);/*直接到位*/

// /*调试*/Serial.println("3");
  while(flag_rest);
  contal_pulse('x',x_data_r,x_data_l,DSD);/*直接到位*/

// /*调试*/Serial.println("4");
  while(flag_rest);
  contal_pulse('y',x_data_b,x_data_f,DSD);/*直接到位*/
  

// /*调试*/Serial.println("5");
  while(flag_rest);
  contal_pulse('x',x_data_l,x_data_r,DSD);/*直接到位*/


// /*调试*/Serial.println("6");
  while(flag_rest);
  contal_pulse('y',x_data_f,x_data_b,DSD);/*直接到位*/
  while(flag_rest){;}

  // while(1){;}
// /*调试*/Serial.println("7");



        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g_font_7x14); // 设置字体
          u8g2.drawStr(0, 40, "-read_Task_3END-");
        } while (u8g2.nextPage());

}

/*以下，任意时刻中断复位*/
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
/*以上，任意时刻中断复位*/

/*获取当前脉冲*/
void get_angle()
{
/*以下，显示*/
        // u8g2.firstPage();
        // do
        // {
        //   u8g2.setFont(u8g_font_7x14); // 设置字体
        //   u8g2.drawStr(0, 10, "get_angle");
        //   u8g2.drawStr(0,25, "ga_x");
        //   u8g2.setCursor(sizeof("ga_x: ") * 6, 25);
        //   u8g2.print(get_angle_x);

        //   u8g2.drawStr(0,40, "ga_y");
        //   u8g2.setCursor(sizeof("ga_y: ") * 6, 40);
        //   u8g2.print(get_angle_y);

        // } while (u8g2.nextPage());
/*以上，显示*/
}

/*以下，串口收发*/
void ReceiveDate(uint8_t num){
    uint8_t Flag =1;
    unsigned int RxState = 0;
    unsigned int RxCounter1 = 0;
    while (Flag)
    {
      while(Serial1.available() > 0) {   
          uint16_t com_data = Serial1.read();
          if (RxState == 0)
              {
                  if (com_data == 0xFF)
                  {
                      RxState = 1;
                      RxCounter1 = 0;
                      
                  }
              }
          else if (RxState == 1)
          {
              data[RxCounter1] = com_data;
              RxCounter1++;
              
              if (RxCounter1 >= num) //数据校验数据有多少就写多少
              {
                  RxState = 2;
              }
          }
          else if (RxState == 2)
          {
              if (com_data == 0xFE)
              {
                  RxState = 0;
                  Flag = 0;
              }
          }
      }

  }
  
}

void sending_data(unsigned int First_data, unsigned int Second_data, unsigned int Third_data,unsigned int Four_data) {
    unsigned char data[] = {0xFF, First_data,Second_data,Third_data,Four_data,0xFE};
    for (int i = 0; i < sizeof(data); i++) {
        Serial1.write(data[i]);
    }
}
/*以上，串口收发*/