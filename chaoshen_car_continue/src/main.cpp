/**
 * 说明：白底上亮-1，黑线上灭-0
 * 规定：左转为车向左转，右转为车向右转
 */

#include <Arduino.h>
// #include <FlexiTimer2.h>
// #include <TimerThree.h>
/*以下，OLED相关文件*/
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Wire.h>

#include "avr8-stub.h"
#include "app_api.h"

/*OLED配置*/
//SCL-8 SDA-9
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 8, /* data=*/ 9, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display


/*以下，需要调试的各种参数*/
#define lj_60cm_go 1000*4.2 //60cm前进累计
#define lj_60cm_lr 500 //60cm左右累计

#define Advance_black_ms 100*6 //到黑线上前

#define AnitClockwise_ms 10*10   //左转时间
#define AnitClockwise_pwm_le 80 //左转pwm-左
#define AnitClockwise_pwm_ri 80 
#define Clockwise_ms 100*10   //右转时间
#define Clockwise_pwm_le 80 //右转pwm-左
#define Clockwise_pwm_ri 80 
/*以上，需要调试的各种参数*/

/*灰度接线*/
#define hd1   22    //灰度1-8通道
#define hd2   23    //
#define hd3   24    //
#define hd4   25    //
#define hd5   26    //
#define hd6   27    //
#define hd7   28    //
#define hd8   29    //

/*电机接线*/
#define left_motor_pwm    2 //左电机pwm
#define right_motor_pwm   3 //右电机pwm
#define ln1_left    30 //左电机1
#define ln2_left    31 //左电机2
#define ln3_right   32 //右电机1
#define ln4_right   33 //右电机2

/*车运动定义*/
#define STOP      'a'   //停止
#define RUN       'b'  //前进
#define BACK      'c'   //后退
#define LEFT      'd'   //左转
#define RIGHT     'e'   //右转


int hd_value[9];  //灰度传感器数值

int Aim = 15;  //目的地编号
int ZY = 5;   //openmv分左右


char Aim_in = ' ';

/*以下，函数声明*/
void AnitClockwise(char car_mode_a, int lr_a, int lr_b, int lr_a_ms);/*左转*/
void Clockwise(char car_mode_a, int lr_a, int lr_b, int lr_a_ms); /*右转*/
void leri_chooise(int target_1or2_choose);/*返回-5678左右60cm弯*/
void lj_60cm(unsigned int lj_60cm_ms);/*60cm前进累计*/
void Advance_black(int Advance_go);/*到黑线，上前*/
void white_Search();/*找到白色(终点)停下*/
void black_Search();/*前进找到黑色(终点)停下*/
void Search_black_back();/*后退找到黑线，停止*/
void track_Q();/*前进循迹*/
void track_Q_BACK();/*倒退循迹*/
void hd_read_value();/*读取灰度值*/
void motor_Exercise_status(char motro_state, int left_pwm, int right_pwm);/*电机运动状态*/
void OpenmvZY();/*k210确定左右，有无*/
void OpenmvRead();/*k210读取目标病房*/
/*以上，函数声明*/


void setup()
{
  // debug_init();
  Serial.begin(9600);//串口调试-因为RAM断点默认串口0,懒得改了，就用串口2读数据吧
  Serial1.begin(9600);//openmv
  /*灰度引脚模式*/
  pinMode(hd1,INPUT);pinMode(hd2,INPUT);
  pinMode(hd3,INPUT);pinMode(hd4,INPUT);
  pinMode(hd5,INPUT);pinMode(hd6,INPUT);
  pinMode(hd7,INPUT);pinMode(hd8,INPUT);

  /*电机引脚模式*/
  pinMode(ln1_left,OUTPUT);pinMode(ln2_left,OUTPUT);
  pinMode(ln3_right,OUTPUT);pinMode(ln4_right,OUTPUT);
  /*电机PWM引脚模式*/
  pinMode(left_motor_pwm,OUTPUT);pinMode(right_motor_pwm,OUTPUT);

  /*以下，OLED部分*/
  pinMode(40, INPUT_PULLUP); //mode
  pinMode(41, INPUT_PULLUP); //down
  pinMode(42, INPUT_PULLUP); //up
  pinMode(43, INPUT_PULLUP); //enter
  u8g2.begin();//初始化演示器
  u8g2.setColorIndex(1);   
  /*以上，OLED部分*/

}



void loop()
{
  Serial.println("程序开始： ");
  u8g2.firstPage();
  do {
  // u8g2.clearBuffer();                //清除内部缓冲区
          u8g2.setFont(u8g_font_7x14); //设置字体
          u8g2.drawStr(0,10,"hd_value: ");
          ////hd_read_value();//读取灰度值 /*在此不能调用此函数*/
          u8g2.setCursor((10*0), 10*2.5); u8g2.print(hd_value[1]);
          u8g2.setCursor((10*1), 10*2.5); u8g2.print(hd_value[2]);
          u8g2.setCursor((10*2), 10*2.5); u8g2.print(hd_value[3]);
          u8g2.setCursor((10*3), 10*2.5); u8g2.print(hd_value[4]);
          u8g2.setCursor((10*4), 10*2.5); u8g2.print(hd_value[5]);
          u8g2.setCursor((10*5), 10*2.5); u8g2.print(hd_value[6]);
          u8g2.setCursor((10*6), 10*2.5); u8g2.print(hd_value[7]);
          u8g2.setCursor((10*7), 10*2.5); u8g2.print(hd_value[8]);

        u8g2.drawStr(0,45,"Aim: ");
        u8g2.setCursor(sizeof("Aim: ")*8, 45);
        u8g2.print(Aim);


        u8g2.drawStr(0, 60, "ZY: ");
        u8g2.setCursor(sizeof("ZY: ")*8, 60);
        u8g2.print(ZY);
    } while ( u8g2.nextPage() );
    // u8g2.sendBuffer();

   OpenmvRead();//读取目的地编号 
/*以下，近端*/
  if(Aim==1)//目的地编号1  ////1号位置在左边
  {
    /*前进*/
    black_Search();                   //前进找到黑色(终点)停下
    Advance_black(Advance_black_ms);                //到黑线，上前
    AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
    white_Search();                   //找到白色(终点)停下
    /*返回*/
    Search_black_back();              //后退找到黑线停下
    Advance_black(Advance_black_ms);                //到黑线，上前
    AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
    white_Search();                   //找到白色(终点)停下

  }

  if(Aim==2)//目的地编号2  ////2号位置在右边
  {
    /*前进*/
    black_Search();                   //前进找到黑色(终点)停下
    Advance_black(Advance_black_ms);                //到黑线，上前
    Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms); //右转
    white_Search();                   //找到白色(终点)停下

    /*返回*/
    Search_black_back();              //后退找到黑线停下
    Advance_black(Advance_black_ms);                //到黑线，上前
    Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms); //右转
    white_Search();                   //找到白色(终点)停下
  }
/*以上，近端*/

/*中端测试*/
/*以下，中端和远端*/
  if(Aim>2)
  {
    /*调试*/Serial.println("进入中端");
    black_Search();                   //前进找到黑色(终点)停下  //到达近端十字路口
    lj_60cm(lj_60cm_go);              //60cm前进累计-向前
    /*调试*/motor_Exercise_status(BACK, 100, 100);
    /*调试*/delay(100);
    /*调试*/motor_Exercise_status(STOP, 0, 0); 
    OpenmvZY();                     //比对，分左右
    // /*调试*/while(1);
    /*开始中端*/
    switch(ZY)//中端找到了目的地编号
    {
      case 0://中端左转
      /*调试*/Serial.println("中端左转开始!!!");
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
        white_Search();                   //找到白色(终点)停下
        /*调试*/Serial.println("中端开始返回");
        Search_black_back();              //后退找到黑线停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
        white_Search();                   //找到白色(终点)停下
        /*调试*/Serial.println("中端左转结束###");
      break;

      case 1://中端右转
        /*前进*/
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms); //右转
        white_Search();                   //找到白色(终点)停下

        /*返回*/
        Search_black_back();              //后退找到黑线停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms); //右转
        white_Search();                   //找到白色(终点)停下
      break;

      default:break;
    }

/*远端测试*/
    /*开始远端*/
    black_Search();                   //到达中端十字路口上
    lj_60cm(lj_60cm_go);              //60cm前进累计
    OpenmvZY();                     //比对，分左右

    switch(ZY)//远端找到了目的地编号
    {
      case 0://远端左转
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
        lj_60cm(lj_60cm_lr);              //60cm左右累计
        OpenmvZY();                     //比对，分左右
        /*远端分支*/
        if(ZY==0)//远端左分支-左转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(Advance_black_ms);                //到黑线，上前
          AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
          white_Search();                   //找到白色(终点)停下

          /*远端返回*/
          leri_chooise(1);                  //远端分支-左
          leri_chooise(4);                  //远端-左(实际右转)
          white_Search();                   //找到白色(终点)停下
        }
        else if(ZY==1)//远端左分支-右转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(Advance_black_ms);                //到黑线，上前
          Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms);  //右转
          white_Search();                   //找到白色(终点)停下

          /*远端返回*/
          leri_chooise(2);                  //远端分支-右
          leri_chooise(4);                  //远端-右(实际右转)
          white_Search();                   //找到白色(终点)停下
        }
      break;



      case 1://远端右转
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms);  //右转
        lj_60cm(lj_60cm_lr);              //60cm左右累计
        OpenmvZY();                     //比对，分左右
        /*远端分支*/
        if(ZY==0)//远端右分支-左转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(Advance_black_ms);                //到黑线，上前
          AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
          white_Search();                   //找到白色(终点)停下

          /*远端返回*/
          leri_chooise(1);                  //远端分支-左
          leri_chooise(3);                  //远端-左(实际左转)
          white_Search();                   //找到白色(终点)停下
        }
        else if(ZY==1)//右转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(Advance_black_ms);                //到黑线，上前
          Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms);  //右转
          white_Search();                   //找到白色(终点)停下

          /*远端返回*/
          leri_chooise(2);                  //远端分支-右
          leri_chooise(3);                  //远端-右(实际右转)
          white_Search();                   //找到白色(终点)停下
        }
      break;

      default:break;
    }    
  }
  /*以上，中端和远端*/

}


/*以下，所有函数*/


/*左转*/
void AnitClockwise(char car_mode_a, int lr_a, int lr_b, int lr_a_ms)
{ 
  motor_Exercise_status(car_mode_a, lr_a, lr_a_ms);//左转 //LEFT
  delay(lr_a_ms);  
  motor_Exercise_status(BACK, 100, 100);//停车
  delay(100);  
  motor_Exercise_status(STOP, 0, 0);//停车
  delay(100);
  /*调试*/Serial.println("左转完成");
}


/*右转*/
void Clockwise(char car_mode_a, int lr_a, int lr_b, int lr_a_ms)
{ 
  motor_Exercise_status(car_mode_a, lr_a, lr_b);//右转
  delay(lr_a_ms);    
  motor_Exercise_status(BACK, 100, 100);//停车
  delay(100);
  motor_Exercise_status(STOP, 0, 0);//停车
  delay(100);
  /*调试*/Serial.println("右转完成");
}


/*返回-5678左右60cm弯*/
void leri_chooise(int target_1or2_choose)
{
  switch(target_1or2_choose)
  {
    /*以下，后退左右转*/
    case 1: //后退左转
    {
        int x=1;
        while(x==1)
        {
          track_Q_BACK();//倒退循迹
          hd_read_value();
          if(hd_value[1]==0 && hd_value[2]==0 && hd_value[3]==0 && hd_value[4]==0)
          {
            x = 0;
            break;
          }
        }
    
        Advance_black(Advance_black_ms);                //到黑线，上前
        AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
    }
    break;

    case 2: //后退右转
    {
        int x=1;  
        while(x==1)
        {
          track_Q_BACK();//倒退循迹
          hd_read_value();
          if(hd_value[5]==0 && hd_value[6]==0 && hd_value[7]==0 && hd_value[8]==0)
          {
            x = 0;
            break;
          }
        }
        Advance_black(Advance_black_ms);                //到黑线，上前
        Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms);  //右转
    }
    break;
    /*以上，后退左右转*/


    /*以下，前进左右转*/
    case 3: //前进左转
    {
        int x=1;
        while(x==1)
        {
          track_Q();//前进循迹
          hd_read_value();
          if(hd_value[1]==0 && hd_value[2]==0 && hd_value[3]==0 && hd_value[4]==0)
          {
            x = 0;
            break;
          }
        }
        Advance_black(Advance_black_ms);                //到黑线，上前
        AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
    }
    break;

    case 4: //前进右转
        int x_3=1;  
        while(x_3==1)
        {
          track_Q();//前进循迹
          hd_read_value();
          if(hd_value[5]==0 && hd_value[6]==0 && hd_value[7]==0 && hd_value[8]==0)
          {
            x_3 = 0;
            break;
          }
        }
        Advance_black(Advance_black_ms);                //到黑线，上前
        Clockwise(RIGHT, Clockwise_pwm_le, Clockwise_pwm_ri, Clockwise_ms);  //右转
    break;
    /*以上，前进左右转*/    

  }
}


/*60cm前进累计*/
void lj_60cm(unsigned int lj_60cm_ms)
{
  Serial.println("中端前进累计开始!!!");
  unsigned long last_time = millis();
  while((millis()-last_time)<lj_60cm_ms)
  {
    track_Q();
  }
  Serial.println("中端前进累计结束###");
}

/*到黑线，上前*/
void Advance_black(int Advance_go)
{
  /*调试*/  Serial.println("找黑线，上前");
  motor_Exercise_status(RUN, 100, 100); 
  delay(Advance_go);
  motor_Exercise_status(BACK, 100, 100); 
  delay(100);
  motor_Exercise_status(STOP, 0, 0); 
  delay(100);
}


/*找到白色(终点)停下*/
void white_Search()
{
  int x = 1;
  while(x==1)
  {
    track_Q();//循迹-前进
    hd_read_value();
    if(hd_value[1]==1 && hd_value[2]==1 && hd_value[3]==1 && hd_value[4]==1 && hd_value[5]==1 && hd_value[6]==1 && hd_value[7]==1 && hd_value[8]==1)
    {
      x = 0;
      break;
    }
  }  
   motor_Exercise_status(BACK, 100, 100); //停车
   delay(100);
   motor_Exercise_status(STOP, 0, 0); //停车
   delay(100);
}

/*前进找到黑色(终点)停下*/
void black_Search()
{
  int x = 1;
  while(x==1)
  { 
    track_Q();//前进循迹
    hd_read_value();
    if(hd_value[1]==0 && hd_value[2]==0 && hd_value[3]==0 && hd_value[4]==0 && hd_value[5]==0 && hd_value[6]==0 && hd_value[7]==0 && hd_value[8]==0)
    {
      x = 0;
      break;
    }
  }
   motor_Exercise_status(BACK, 100, 100); //停车
   delay(100);
   motor_Exercise_status(STOP, 0, 0); //停车
   delay(100);
}

/*后退找到黑线，停止*/
void Search_black_back()
{
  int x = 1;
  while(x)
  {   
    track_Q_BACK();
    if(hd_value[1]==0 && hd_value[2]==0 && hd_value[3]==0 && hd_value[4]==0 && hd_value[5]==0 && hd_value[6]==0 && hd_value[7]==0 && hd_value[8]==0)
    {
      x = 0;
      break;
    }
  }
  motor_Exercise_status(RUN, 100, 100); //先刹车，后停车
  delay(100);
  motor_Exercise_status(STOP, 0, 0); //停车
  delay(100);
}



/*前进循迹*/
void track_Q()
{
    hd_read_value();//读取灰度值

    if(hd_value[3]==0 && hd_value[4]==0 && hd_value[5]==1 && hd_value[6]==1)  //右偏-小      0011 //左转
    {
      motor_Exercise_status(LEFT, 50, 50);
      /*调试*/Serial.println("右偏-小");
    }
    else if(hd_value[3]==0 && hd_value[4]==1 && hd_value[5]==1 && hd_value[6]==1)  //右偏-中 0111 
    {
      motor_Exercise_status(LEFT, 80, 80);
      /*调试*/Serial.println("右偏-中");
    }

    else if(hd_value[3]==1 && hd_value[4]==1 && hd_value[5]==0 && hd_value[6]==0)  //左偏-小 1100 //右转
    {
      motor_Exercise_status(RIGHT, 50, 50);
      /*调试*/Serial.println("左偏-小");
    }
    else if(hd_value[3]==1 && hd_value[4]==1 && hd_value[5]==1 && hd_value[6]==0)  //左偏-中 1110
    {
      motor_Exercise_status(RIGHT, 80, 80);
      /*调试*/Serial.println("左偏-中");
    }
    
    motor_Exercise_status(RUN, 100, 100); //前进
}

/*倒退循迹*/
void track_Q_BACK()
{
    hd_read_value();
    if(hd_value[3]==0 && hd_value[4]==0 && hd_value[5]==1 && hd_value[6]==1)  //右偏-小      0011 //左转
    {
      motor_Exercise_status(LEFT, 50, 50);
      /*调试*/Serial.println("右偏-小");
    }
    else if(hd_value[3]==0 && hd_value[4]==1 && hd_value[5]==1 && hd_value[6]==1)  //右偏-中 0111 
    {
      motor_Exercise_status(LEFT, 80, 80);
      /*调试*/Serial.println("右偏-中");
    }

    else if(hd_value[3]==1 && hd_value[4]==1 && hd_value[5]==0 && hd_value[6]==0)  //左偏-小 1100 //右转
    {
      motor_Exercise_status(RIGHT, 50, 50);
      /*调试*/Serial.println("左偏-小");
    }
    else if(hd_value[3]==1 && hd_value[4]==1 && hd_value[5]==1 && hd_value[6]==0)  //左偏-中 1110
    {
      motor_Exercise_status(RIGHT, 80, 80);
      /*调试*/Serial.println("左偏-中");
    }
    
    motor_Exercise_status(BACK, 100, 100); //前进
    /*调试*/Serial.println("正轨");
}



/*读取灰度值*/
void hd_read_value()
{
  hd_value[1] = digitalRead(hd1);
  hd_value[2] = digitalRead(hd2);
  hd_value[3] = digitalRead(hd3);
  hd_value[4] = digitalRead(hd4); //中左1
  hd_value[5] = digitalRead(hd5); //中右1
  hd_value[6] = digitalRead(hd6);
  hd_value[7] = digitalRead(hd7);
  hd_value[8] = digitalRead(hd8);

// /*以下，调试*/
//    Serial.print(hd_value[1]); Serial.print('\t');
//     Serial.print(hd_value[2]); Serial.print('\t');
//      Serial.print(hd_value[3]); Serial.print('\t');
//       Serial.print(hd_value[4]); Serial.print('\t');
//         Serial.print(hd_value[5]); Serial.print('\t');
//          Serial.print(hd_value[6]); Serial.print('\t');
//           Serial.print(hd_value[7]); Serial.print('\t');
//             Serial.print(hd_value[8]); Serial.println('\t');
// /*以上，调试*/

  // u8g2.firstPage();//不可使用否则，打印出错？？？
  // do {
  //         u8g2.setFont(u8g_font_7x14);
  //         u8g2.drawStr(0,10,"hd_value: ");
  //         hd_read_value();//读取灰度值
  //         u8g2.setCursor((10*0), 10*3); u8g2.print(hd_value[1]);
  //         u8g2.setCursor((10*1), 10*3); u8g2.print(hd_value[2]);
  //         u8g2.setCursor((10*2), 10*3); u8g2.print(hd_value[3]);
  //         u8g2.setCursor((10*3), 10*3); u8g2.print(hd_value[4]);
  //         u8g2.setCursor((10*4), 10*3); u8g2.print(hd_value[5]);
  //         u8g2.setCursor((10*5), 10*3); u8g2.print(hd_value[6]);
  //         u8g2.setCursor((10*6), 10*3); u8g2.print(hd_value[7]);
  //         u8g2.setCursor((10*7), 10*3); u8g2.print(hd_value[8]);
  //     } while ( u8g2.nextPage() );
}


/*电机运动状态*/
void motor_Exercise_status(char motro_state, int left_pwm, int right_pwm)
{
  analogWrite(left_motor_pwm,  left_pwm);     //PWM控制
  analogWrite(right_motor_pwm, right_pwm);

  switch(motro_state)
  {
    case RUN:
      Serial.println("RUN"); //前进输出状态
      digitalWrite(ln1_left,  LOW);
      digitalWrite(ln2_left,  HIGH);
      digitalWrite(ln3_right, LOW);
      digitalWrite(ln4_right, HIGH);
      break;
      
     case BACK:
      Serial.println("BACK"); //后退输出状态
      digitalWrite(ln1_left,  HIGH);
      digitalWrite(ln2_left,  LOW);
      digitalWrite(ln3_right, HIGH);
      digitalWrite(ln4_right, LOW);
      break;

     case RIGHT:
      Serial.println("LEFT"); //左转输出状态
      digitalWrite(ln1_left,  HIGH);  //左轮开机
      digitalWrite(ln2_left,  LOW);
      digitalWrite(ln3_right, LOW);
      digitalWrite(ln4_right, HIGH);
      break;
  
     case LEFT:
      Serial.println("RIGHT"); //右转弯输出状态
      digitalWrite(ln1_left,  LOW);
      digitalWrite(ln2_left,  HIGH);
      digitalWrite(ln3_right, HIGH);  //右轮开机
      digitalWrite(ln4_right, LOW);
      break;

     default:
      Serial.println("STOP"); //停止输出状态
      digitalWrite(ln1_left,  LOW);
      digitalWrite(ln2_left,  LOW);
      digitalWrite(ln3_right, LOW);
      digitalWrite(ln4_right, LOW);
      break;
  }
}

/*k210读取目标病房*/
void OpenmvRead()
{
  while(Serial1.read()>=0); //清空缓冲区
  while(Serial1.available()==0);//等待信号传输
  if(Serial1.available())     //读取k210目标病房
  {
    /*绝对不可以在此打印，否则只能识别1*/// Serial.println("inininin!!!");
    Aim_in = Serial1.read();
    if(Aim_in=='1') {Aim=1;}
    if(Aim_in=='2') {Aim=2;}  
    if(Aim_in=='3') {Aim=3;}
    if(Aim_in=='4') {Aim=4;}
    if(Aim_in=='5') {Aim=5;}  
    if(Aim_in=='6') {Aim=6;}
    if(Aim_in=='7') {Aim=7;}
    if(Aim_in=='8') {Aim=8;}
    /*绝对不可以在此打印，否则只能识别1*/// Serial.println("outoutout!!!");
  }
  while(Serial1.read()>=0); //清空缓冲区
  // while(Serial1.read()>=0){;} //清空缓冲区
  u8g2.firstPage();
  do {
    // u8g2.clearBuffer();
        u8g2.setFont(u8g_font_7x14); //设置字体
        u8g2.drawStr(0,45,"Aim: ");
        u8g2.setCursor(sizeof("Aim: ")*8, 45);
        u8g2.print(Aim);
      } while ( u8g2.nextPage() );
    // u8g2.sendBuffer();
}

/*k210确定左右，有无*/
void OpenmvZY()
{
  while(Serial1.read()>=0); //清空缓冲区
  while(Serial1.available()==0);//等待信号传输
  if(Serial1.available()>0)     //读取k210确定左右，有无
  {
    char Aim_choose_in = Serial1.read();
    Serial.print("********Aim_choose_in*******"); Serial.println(Aim_choose_in); Serial.print("********Aim_choose_in*******"); 
    if(Aim_choose_in=='0')     
    {
      ZY=0;
    }
    if(Aim_choose_in=='1') 
    {
      ZY=1;
    }  
    if(Aim_choose_in=='2')
    {
      ZY=2;
    }
    Serial.println("进入成功-读取");
  }
  while(Serial1.read()>=0);//清空缓冲区
  // while(Serial1.read()>=0){;}//清空缓冲区
  u8g2.firstPage();
  do {
    // u8g2.clearBuffer(); 
        u8g2.setFont(u8g_font_7x14);
        u8g2.drawStr(0, 60, "ZY: ");
        u8g2.setCursor(sizeof("ZY: ")*8, 60);
        u8g2.print(ZY);
      } while ( u8g2.nextPage() );
    // u8g2.sendBuffer();
}
