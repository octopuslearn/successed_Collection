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
// const unsigned char bitmap_icon_wushousign [] PROGMEM = 
// {
// 0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x40,0x02,0x20,0x04,0x0C,0x30,0x48,0x12,0x08,0x10,0x80,0x01,0x48,0x12,0x20,0x04,0x24,0x24,0x22,0x44,0x12,0x48,0x00,0x00,/*"C:\Users\zy\Desktop\摊手图标.bmp",0*/
// /* (16 X 16 )*/

// };


// const unsigned char bitmap_item_sel_outline[] PROGMEM = //所选项中框 
// {
// 0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
// 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
// 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
// 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,
// 0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"C:\Users\zy\Desktop\中框 (1).bmp",0*/
// /* (128 X 21 )*/

// };


// const unsigned char bitmap_scrollbar_background[] PROGMEM = //滚动列表背景
// {
// 0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,/*"C:\Users\zy\Desktop\滚动列表线.bmp",0*/
// /* (2 X 64 )*/

// };


// int item_sel_previous; // previous item - used in the menu screen to draw the item before the selected one  //上一项 - 在菜单屏幕中用于在所选项目之前绘制项目
// int item_sel_next; // next item - used in the menu screen to draw next item after the selected one  //下一项 - 在菜单屏幕中用于在所选项目之后绘制下一项
// int item_selected = 0;//选择了菜单中的那个项目，即但前按键项目

// int current_screen = 0; //屏幕，如截屏，二维码

// int demo_mode_delay = 0;//屏幕切换速度，即屏幕延时时间
// int demo_mode_state = 0;//屏幕数  //即直接模式下，界面的个数



// int demo_mode = 0;//屏幕模式选择



// int button_up_clicked = 0;//向上，单击按键，等到再次按下，即只按一次
// int button_down_clicked = 0;//向下，单击按键，等到再次按下，即只按一次
// int button_select_clicked = 0;//确定


// const unsigned char* bitmap_icons[3]={
//   bitmap_icon_wushousign,       //握手标志
//   bitmap_item_sel_outline,      //所选项中框
//   bitmap_scrollbar_background,  //滚动列表背景
// };


// const int NUM_ITEMS = 4;      //列表中的项目数，即行数
// const int MAX_ITEM_LENGTH = 20;//项目名称的最大数组，即menu_items列数

// char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {  // array with item names
//   { "hd_value" }, 
//   { "Aim" }, 
//   { "ZY" }, 
//   { "wring!" }, 
//  };
// /*以上，OLED相关文件*/








/*以下，需要调试的各种参数*/
#define lj_60cm_go 1000 //60cm前进累计
#define lj_60cm_lr 500 //60cm左右累计

#define Advance_black_ms 500 //到黑线上前

#define AnitClockwise_ms 500   //左转时间
#define AnitClockwise_pwm_le 80 //左转pwm-左
#define AnitClockwise_pwm_ri 80 
#define Clockwise_ms 500   //右转时间
#define Clockwise_pwm_le 80 //右转pwm-左
#define Clockwise_pwm_ri 80 
/*以上，需要调试的各种参数*/



/*以下，调试用外设*/
#define led1 49
/*以上，调试用外设*/



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
void lj_60cm(int lj_60cm_ms);/*60cm前进累计*/
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
void OLED_show();/*OELD*/
void OLED_button_read();
/*以上，函数声明*/




void setup()
{
  debug_init();
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


  /*以下，调试用外设*/
  pinMode(led1,OUTPUT);
  /*以上，调试用外设*/


  // Serial.begin(9600); //调试-串口0
  // Serial1.begin(9600); //k210-串口1

  /*以下，OLED部分*/
  pinMode(40, INPUT_PULLUP); //mode
  pinMode(41, INPUT_PULLUP); //down
  pinMode(42, INPUT_PULLUP); //up
  pinMode(43, INPUT_PULLUP); //enter
  u8g2.begin();//初始化演示器
  u8g2.setColorIndex(1);   
  /*以上，OLED部分*/

  /*以下，初始化定时器2*/
  // FlexiTimer2::set(50, 1.0/1000, OLED_button_read); // call every 500 1ms "ticks"
  // // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  // FlexiTimer2::start();
  // /*以上，初始化定时器2*/

  // /*以下，初始化定时器2*/
  // Timer3.initialize(800000);//1500000-1000000-800000
  // Timer3.attachInterrupt(OLED_show);
  /*以上，初始化定时器2*/
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
    /*调试*/Serial.println("目标1-/*前进*/");
    black_Search();                   //前进找到黑色(终点)停下
    /*调试*/  //Serial.println("前进找到黑色(终点)停下");
    Advance_black(Advance_black_ms);                //到黑线，上前
    /*调试*/  //Serial.println("到黑线，上前");
    AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
    /*调试*/  //Serial.println("左转");
    white_Search();                   //找到白色(终点)停下
    /*调试*/  //Serial.println("找到白色(终点)停下");
    /*调试*///Serial.println("目标1-/*返回*/");
    /*返回*/
    Search_black_back();              //后退找到黑线停下
    Advance_black(Advance_black_ms);                //到黑线，上前
    AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
    white_Search();                   //找到白色(终点)停下
    /*调试*///Serial.println("目标1-/*完成*/");
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


/*以下，中端和远端*/
  if(Aim>2)
  {
    black_Search();                   //前进找到黑色(终点)停下  //到达近端十字路口
    lj_60cm(lj_60cm_go);              //60cm前进累计-向前
    /*调试*///Serial.println("中端开始读取：  ");
    OpenmvZY();                     //比对，分左右
    /*调试*///Serial.println("中端读取完成###");
    /*开始中端*/
    switch(ZY)//中端找到了目的地编号
    {
      // Serial.println("中端左转LLL");
      case 0://中端左转
      /*调试*///Serial.println("中端左转LLL");
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
        white_Search();                   //找到白色(终点)停下

        Search_black_back();              //后退找到黑线停下
        Advance_black(Advance_black_ms);                //到黑线，上前
        AnitClockwise(LEFT, AnitClockwise_pwm_le, AnitClockwise_pwm_ri, AnitClockwise_ms);  //左转
        white_Search();                   //找到白色(终点)停下
      break;

      case 1://中端右转
      /*调试*/Serial.println("中端右转RRR");
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
  motor_Exercise_status(car_mode_a, lr_a, lr_a_ms);//右转
  delay(lr_a_ms);    
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
void lj_60cm(int lj_60cm_ms)
{
  while(lj_60cm_ms--)
  {track_Q();}
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
  /*调试*/  //Serial.println("前进找黑线");
  int x = 1;
  while(x==1)
  {
    /*调试*/  //Serial.println("//前进循迹");
    track_Q();//前进循迹
    /*调试*/  //Serial.println("//前进循迹---完成");
    hd_read_value();
    /*调试*/  //Serial.println("222222222222222222");
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
    // motor_Exercise_status(BACK, 100, 100); //停车
    
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
    /*调试*/  //Serial.println("track_Q()---开始");
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
    /*调试*/ // Serial.println("track_Q()---完成");    
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

  // u8g2.firstPage();
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
  // Serial.println("k210读取目标病房");
  while(Serial1.read()>=0); //清空缓冲区
  while(Serial1.available()==0);//等待信号传输
  if(Serial1.available())     //读取k210目标病房
  {
    /*绝对不可以在此打印，否则只能识别1*/// Serial.println("inininin!!!");
    Aim_in = Serial1.read();
    if(Aim_in=='1') {Aim=1;Serial.println("k210_1");}
    if(Aim_in=='2') {Aim=2;Serial.println("k210_2");}  
    if(Aim_in=='3') {Aim=3;Serial.println("k210_3");}
    if(Aim_in=='4') {Aim=4;Serial.println("k210_4");}
    if(Aim_in=='5') {Aim=5;Serial.println("k210_5");}  
    if(Aim_in=='6') {Aim=6;Serial.println("k210_6");}
    if(Aim_in=='7') {Aim=7;Serial.println("k210_7");}
    if(Aim_in=='8') {Aim=8;Serial.println("k210_8");}
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

//this 'while' clause does not guard... [-Wmisleading-indentation]
// src\main.cpp: In function 'void OpenmvRead()':
// src\main.cpp:761:3: warning: this 'while' clause does not guard... [-Wmisleading-indentation]
//    while(Serial1.read()>=0); //清空缓冲区
//    ^~~~~
// src\main.cpp:762:5: note: ...this statement, but the latter is misleadingly indented as if it were guarded by the 'while'
//      u8g2.firstPage();

/*k210确定左右，有无*/
void OpenmvZY()
{
  //Serial.println("进入成功开始读取");
  while(Serial1.read()>=0); //清空缓冲区
  while(Serial1.available()==0);//等待信号传输
  if(Serial1.available()>0)     //读取k210确定左右，有无
  {
    char Aim_choose_in = Serial1.read();
    Serial.print("********Aim_choose_in*******"); Serial.println(Aim_choose_in); Serial.print("********Aim_choose_in*******"); 
    if(Aim_choose_in=='0')     
    {
      ZY=0;
      /*调试*/Serial.print("ZY:   "); Serial.println(ZY);
    }
    if(Aim_choose_in=='1') 
    {
      ZY=1;
      /*调试*/Serial.print("ZY:   "); Serial.println(ZY);
    }  
    if(Aim_choose_in=='2')
    {
      ZY=2;
      /*调试*/Serial.print("ZY:   "); Serial.println(ZY);
    }
    Serial.println("进入成功-读取");
    // OLED_show();
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



// /*OELD显示*/
// void OLED_show()
// {
//   // /*调试*/Serial.println("进入OLED刷新");
//   // /*调试*/Serial.println("进入OLED刷新1");
//   // /*调试*/Serial.println("进入OLED刷新2");
//   // /*调试*/Serial.println("进入OLED刷新3");
//   /*分页模式*/
//   u8g2.firstPage();
//   do {
//       if(current_screen == 0) //菜单屏幕
//       {
//         u8g2.drawXBMP(0, 22, 128, 21, bitmap_item_sel_outline);  //绘制所选项中框

//         /*将上一项设定为淡色*/
//         u8g2.setFont(u8g_font_7x14); //设置字体
//         u8g2.drawStr(25, 15, menu_items[item_sel_previous]);  //绘制选项中框中的选项文字 //二维数组为啥这样使用？？？  //drawStr绘制字符
//         u8g2.drawXBMP(4, 2, 16, 16, bitmap_icons[0]);  //绘制握手图标
//         // /*调试*/Serial.println("上一项前面++");
//         // /*调试*/ Serial.print("上一项，图标："); /*调试*/Serial.println("上一项后面--");Serial.println(item_sel_previous);

//         /*将当前项设定为深色*/
//         u8g2.setFont(u8g_font_7x14B); //设置字体 //B代表粗体
//         u8g2.drawStr(25, 15+20+2, menu_items[item_selected]);  //绘制选项中框中的选项文字 //二维数组为啥这样使用？？？  //drawStr绘制字符
//         u8g2.drawXBMP(4, 24, 16, 16, bitmap_icons[0]);  //绘制握手图标
//         // /*调试*/  Serial.print("当前项，图标："); Serial.println(item_selected);
//         /*将下一项设定为浅色*/
//         u8g2.setFont(u8g_font_7x14); //设置字体 //B代表粗体
//         u8g2.drawStr(25, 15+20+20+2+2, menu_items[item_sel_next]);  //绘制选项中框中的选项文字 //二维数组为啥这样使用？？？  //drawStr绘制字符
//         u8g2.drawXBMP(4, 46, 16, 16, bitmap_icons[0]);  //绘制握手图标
//         // /*调试*/  Serial.print("下一项，图标："); Serial.println(item_sel_next);

//         u8g2.drawXBMP(128-2, 0, 2, 64, bitmap_scrollbar_background);  //绘制滚动列表背景
//         u8g2.drawBox(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS);  //绘制滚动条 //drawBox是绘制啥的，为啥绘制滚动条的时候要用？？？
//       }
//       if(current_screen == 1)  //直接跳转到hd_value
//       {
//         // /*调试*/Serial.println("hd_value: ");
//           u8g2.drawStr(0,10,"hd_value: ");
//           hd_read_value();//读取灰度值
//           u8g2.setCursor((10*0), 10*3); u8g2.print(hd_value[1]);
//           u8g2.setCursor((10*1), 10*3); u8g2.print(hd_value[2]);
//           u8g2.setCursor((10*2), 10*3); u8g2.print(hd_value[3]);
//           u8g2.setCursor((10*3), 10*3); u8g2.print(hd_value[4]);
//           u8g2.setCursor((10*4), 10*3); u8g2.print(hd_value[5]);
//           u8g2.setCursor((10*5), 10*3); u8g2.print(hd_value[6]);
//           u8g2.setCursor((10*6), 10*3); u8g2.print(hd_value[7]);
//           u8g2.setCursor((10*7), 10*3); u8g2.print(hd_value[8]);
//       }
//       else if(current_screen == 2)  //直接跳转到t_lr90
//       {
//         // /*调试*/Serial.println("Aim: ");
//         u8g2.drawStr(0,10,"Aim: ");
//         u8g2.setCursor(sizeof("Aim: ")*8, 20);
//         u8g2.print(Aim);
//       }
//       else if(current_screen == 3)  //直接跳转到car_O/C
//       {
//         // /*调试*/Serial.println("ZY: ");
//         u8g2.drawStr(0, 10, "ZY: ");
//         u8g2.setCursor(sizeof("ZY: ")*8, 20);
//         u8g2.print(ZY);
//       }
//       else if(current_screen == 4)  //直接跳转到wring!
//       {
//         // /*调试*/Serial.println("wring!");
//           u8g2.drawStr(0,10,"hd_value: ");
//           hd_read_value();//读取灰度值
//           u8g2.setCursor((10*0), 10*2.5); u8g2.print(hd_value[1]);
//           u8g2.setCursor((10*1), 10*2.5); u8g2.print(hd_value[2]);
//           u8g2.setCursor((10*2), 10*2.5); u8g2.print(hd_value[3]);
//           u8g2.setCursor((10*3), 10*2.5); u8g2.print(hd_value[4]);
//           u8g2.setCursor((10*4), 10*2.5); u8g2.print(hd_value[5]);
//           u8g2.setCursor((10*5), 10*2.5); u8g2.print(hd_value[6]);
//           u8g2.setCursor((10*6), 10*2.5); u8g2.print(hd_value[7]);
//           u8g2.setCursor((10*7), 10*2.5); u8g2.print(hd_value[8]);

//         u8g2.drawStr(0,45,"Aim: ");
//         u8g2.setCursor(sizeof("Aim: ")*8, 45);
//         u8g2.print(Aim);


//         u8g2.drawStr(0, 60, "ZY: ");
//         u8g2.setCursor(sizeof("ZY: ")*8, 60);
//         u8g2.print(ZY);
//       }

//   } while ( u8g2.nextPage() );
//    ///*调试*/Serial.println("结束");
// }



// void OLED_button_read()
// {
//     if((digitalRead(41)==0) && (button_down_clicked==0))//down按下&&？  单击向下按钮 // && (button_down_clicked==0)-我认为是避免误触的方法
//     {
//       delay(50);
//       if((digitalRead(41)==0) && (button_down_clicked==0))//down按下&&？  单击向下按钮 // && (button_down_clicked==0)-我认为是避免误触的方法
//       {
//         button_down_clicked = 1;
//         item_selected++;//当前按键项目++
//         /*调试*/
//         //Serial.print("down按下: "); Serial.println(item_selected);
//         if(item_selected>=NUM_ITEMS)  //划到低了
//         {
//           item_selected = 0;
//         }
//       }
//     }
//     else if((digitalRead(42)==0) && (button_up_clicked==0))//up按下&&？  单击向下按钮
//     {
//       delay(50);
//       if((digitalRead(42)==0) && (button_up_clicked==0))//up按下&&？  单击向下按钮
//       {
//         button_up_clicked = 1;
//         item_selected--;//当前按键项目++
//         /*调试*/
//         //Serial.print("up按下: "); Serial.println(item_selected);
//         if(item_selected < 0)  //划到顶了 //为啥不是<=1???
//         {
//           item_selected = NUM_ITEMS-1;  //我认为是跳转到最后一项 NUM_ITEMS-1  //引用下标
//         }
//       }
//     }
    
//     if((digitalRead(42)==HIGH) && (button_up_clicked==1))   {button_up_clicked = 0;};  //取消点击  //up

//     if((digitalRead(41)==HIGH) && (button_down_clicked==1)) {button_down_clicked = 0;};  //down


//   if((digitalRead(43)==LOW) && (button_select_clicked==0)) //enter
//   {
//     delay(50);
//     if((digitalRead(43)==LOW) && (button_select_clicked==0)) //enter
//     {
//       button_select_clicked = 1;
//       if(current_screen==0) 
//       {       
//         if(item_selected == 0)
//         {
//           current_screen = 1;///*调试*/ Serial.println("1级菜单hd_value");
//         }
//         if(item_selected == 1)
//         {
//           current_screen = 2;///*调试*/ Serial.println("1级菜单t_lr90");
//         }
//         if(item_selected == 2)
//         {
//           current_screen = 3;///*调试*/ Serial.println("1级菜单car_O/C");
//         }
//         if(item_selected == 3)
//         {
//           current_screen = 4;///*调试*/ Serial.println("1级菜单wring!");
//         }
//       }
//     else 
//       {
//         current_screen=0;//Serial.println("主菜单"); /*就缺了个这！！！*/
//       }
//     }
//   }
//     if((digitalRead(43)==HIGH) && (button_select_clicked==1))
//     {
//       button_select_clicked = 0;//取消点击
//     }

//     /*确定上一项，下一项的值*/
//     item_sel_previous = item_selected-1;//上一项=当前项-1
//     if(item_sel_previous < 0)          {item_sel_previous = NUM_ITEMS-1;}
//     item_sel_next = item_selected+1;
//     if(item_sel_next >= NUM_ITEMS) {item_sel_next = 0;}

// }

// HardwareSerial0.cpp.o (symbol from plugin): In function `Serial':
// (.text+0x0): multiple definition of `__vector_25'
// avr8-stub.c.o (symbol from plugin):(.text+0x0): first defined here
// collect2.exe: error: ld returned 1 exit status
// *** [.pio\build\megaatmega2560\firmware.elf] Error 1