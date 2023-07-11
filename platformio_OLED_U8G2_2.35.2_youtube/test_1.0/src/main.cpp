/*去掉，Serial.begin(9600);-可以显示0_L_HOD，打开就不能显示了
双击时，效果不明显，好像一个dowm,两个up*/
//另外，如何将参数从key.cpp传递到main.cpp来？？？

#include <TimerOne.h>

#include "key.h"
#include <U8g2lib.h>
#include <U8x8lib.h>

#include <Wire.h>
void timerIsr();

/*
* SCL 20
* SDA 21
*/
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
void setup(){
//   Serial.begin(9600);
  u8g2.begin();//初始化演示器
  Timer1.initialize(5000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(timerIsr); // attach the service routine here
  // put your setup code here, to run once:  
   MX_GPIO_key_Init();
} 

void loop() {
  u8g2.clearBuffer();                //清除内部缓冲区
  u8g2.setFont(u8g2_font_8x13B_mr);//设置字体集
  u8g2.drawStr(0,10,"key:");
  u8g2.setCursor(sizeof("key:")*10,10);
  switch (key_to_u8g2)
  {
    case 1:
            u8g2.print("0_DOWN");
        break;
    case 2:
            u8g2.print("0_UP");
        break;
    case 3:
            u8g2.print("0_REPEAT");
        break;
    case 4:
            u8g2.print("0_CLICK");
        break; 
    case 5:
            u8g2.print("0_UP");
        break;
    case 6:
            u8g2.print("0_L_STR");
        break;
    case 7:
            u8g2.print("0_L_HOD");
        break; 

    default:
        break;
  }
  key_to_u8g2 = 0;  //按键模式清零
  u8g2.sendBuffer();                 //绘制缓冲区的内容
  // put your main code here, to run repeatedly:
  delay(100);
}

void timerIsr()
{
  /*调试*/
//  Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  button_ticks();
}