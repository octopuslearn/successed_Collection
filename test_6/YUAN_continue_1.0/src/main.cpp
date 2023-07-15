/**
 * 
 * 说明：白底上亮-1，黑线上灭-0
 * 规定：左转为车向左转，右转为车向右转
 */

#include <Arduino.h>

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




/*调试LED*/
#define led1 49

/*车运动定义*/
#define STOP      'a'   //停止
#define RUN       'b'  //前进
#define BACK      'c'   //后退
#define LEFT      'd'   //左转
#define RIGHT     'e'   //右转



int hd_value[9];  //灰度传感器数值

int Aim = 0;  //目的地编号
int ZY = 0;   //openmv分左右



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




void setup()
{
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

  /*调试LED*/
  pinMode(led1,OUTPUT);


  Serial.begin(9600); //串口调试
}



void loop()
{
  // OpenmvRead();//目的地编号

/*以下，近端*/
  if(Aim==1)//目的地编号1  ////1号位置在左边
  {
    /*前进*/
    black_Search();                   //前进找到黑色(终点)停下
    Advance_black(10);                //到黑线，上前
    AnitClockwise(LEFT, 50, 50, 50);  //左转
    white_Search();                   //找到白色(终点)停下

    /*返回*/
    Search_black_back();              //后退找到黑线停下
    Advance_black(10);                //到黑线，上前
    AnitClockwise(LEFT, 50, 50, 50);  //左转
    white_Search();                   //找到白色(终点)停下

    digitalWrite(led1,LOW);//亮红灯
  }

  if(Aim==2)//目的地编号2  ////2号位置在右边
  {
    /*前进*/
    black_Search();                   //前进找到黑色(终点)停下
    Advance_black(10);                //到黑线，上前
    AnitClockwise(RIGHT, 50, 50, 50); //右转
    white_Search();                   //找到白色(终点)停下

    /*返回*/
    Search_black_back();              //后退找到黑线停下
    Advance_black(10);                //到黑线，上前
    AnitClockwise(RIGHT, 50, 50, 50); //右转
    white_Search();                   //找到白色(终点)停下
  }
/*以上，近端*/


/*以下，中端和远端*/
  if(Aim>2)
  {
    black_Search();                   //前进找到黑色(终点)停下  //到达近端十字路口
    lj_60cm(1500);                    //60cm前进累计
    //OpenmvZY();                     //比对，分左右

    /*开始中端*/
    switch(ZY)//中端找到了目的地编号
    {
      case 0://中端左转
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(10);                //到黑线，上前
        AnitClockwise(LEFT, 50, 50, 50);  //左转
        white_Search();                   //找到白色(终点)停下

        Search_black_back();              //后退找到黑线停下
        Advance_black(10);                //到黑线，上前
        AnitClockwise(LEFT, 50, 50, 50);  //左转
        white_Search();                   //找到白色(终点)停下
      break;

      case 1://中端右转
        /*前进*/
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(10);                //到黑线，上前
        AnitClockwise(RIGHT, 50, 50, 50); //右转
        white_Search();                   //找到白色(终点)停下

        /*返回*/
        Search_black_back();              //后退找到黑线停下
        Advance_black(10);                //到黑线，上前
        AnitClockwise(RIGHT, 50, 50, 50); //右转
        white_Search();                   //找到白色(终点)停下
      break;

      default:break;
    }

    /*开始远端*/
    black_Search();                   //到达中端十字路口上
    lj_60cm(1500);                    //60cm前进累计
    //OpenmvZY();                     //比对，分左右

    switch(ZY)//远端找到了目的地编号
    {
      case 0://远端左转
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(10);                //到黑线，上前
        AnitClockwise(LEFT, 50, 50, 50);  //左转
        lj_60cm(1500);                    //60cm前进累计
        //OpenmvZY();                     //比对，分左右
        /*远端分支*/
        if(ZY==0)//远端左分支-左转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(10);                //到黑线，上前
          AnitClockwise(LEFT, 50, 50, 50);  //左转
          white_Search();                   //找到白色(终点)停下

          /*远端返回*/
          leri_chooise(1);                  //远端分支-左
          leri_chooise(4);                  //远端-左(实际右转)
          white_Search();                   //找到白色(终点)停下
        }
        else if(ZY==1)//远端左分支-右转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(10);                //到黑线，上前
          AnitClockwise(RIGHT, 50, 50, 50);  //右转
          white_Search();                   //找到白色(终点)停下

          /*远端返回*/
          leri_chooise(2);                  //远端分支-右
          leri_chooise(4);                  //远端-右(实际右转)
          white_Search();                   //找到白色(终点)停下
        }
      break;



      case 1://远端右转
        black_Search();                   //前进找到黑色(终点)停下
        Advance_black(10);                //到黑线，上前
        AnitClockwise(RIGHT, 50, 50, 50);  //左转
        lj_60cm(1500);                    //60cm前进累计
        //OpenmvZY();                     //比对，分左右
        /*远端分支*/
        if(ZY==0)//远端右分支-左转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(10);                //到黑线，上前
          AnitClockwise(LEFT, 50, 50, 50);  //左转
          white_Search();                   //找到白色(终点)停下

          /*远端返回*/
          leri_chooise(1);                  //远端分支-左
          leri_chooise(3);                  //远端-左(实际左转)
          white_Search();                   //找到白色(终点)停下
        }
        else if(ZY==1)//右转
        {
          black_Search();                   //前进找到黑色(终点)停下
          Advance_black(10);                //到黑线，上前
          AnitClockwise(RIGHT, 50, 50, 50);  //右转
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
  motor_Exercise_status(STOP, 0, 0);//停车
  delay(100);
  /*调试*/Serial.println("左转完成");
}


/*右转*/
void Clockwise(char car_mode_a, int lr_a, int lr_b, int lr_a_ms)
{ 
  motor_Exercise_status(RIGHT, 50, 50);//右转
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
    
        Advance_black(10);                //到黑线，上前
        AnitClockwise(LEFT, 50, 50, 50);  //左转
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
        Advance_black(10);                //到黑线，上前
        AnitClockwise(RIGHT, 50, 50, 50);  //右转
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
        Advance_black(10);                //到黑线，上前
        AnitClockwise(LEFT, 50, 50, 50);  //左转
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
        Advance_black(10);                //到黑线，上前
        AnitClockwise(RIGHT, 50, 50, 50);  //右转
    break;
    /*以上，前进左右转*/    

  }
}


/*60cm前进累计*/
void lj_60cm(int lj_60cm_ms)
{ 
  track_Q();
  delay(lj_60cm_ms);
}

/*到黑线，上前*/
void Advance_black(int Advance_go)
{
  motor_Exercise_status(RUN, 100, 100); 
  delay(Advance_go);
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
    /*调试*/Serial.println("正轨");

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

/*以下，调试*/
   Serial.print(hd_value[1]); Serial.print('\t');
    Serial.print(hd_value[2]); Serial.print('\t');
     Serial.print(hd_value[3]); Serial.print('\t');
      Serial.print(hd_value[4]); Serial.print('\t');
        Serial.print(hd_value[5]); Serial.print('\t');
         Serial.print(hd_value[6]); Serial.print('\t');
          Serial.print(hd_value[7]); Serial.print('\t');
            Serial.print(hd_value[8]); Serial.println('\t');
/*以上，调试*/
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