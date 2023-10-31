#include <TimerOne.h>

#include "key.h"
#include <U8g2lib.h>
#include <U8x8lib.h>

#include <Wire.h>


/*
* SCL 20
* SDA 21
*/
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
void setup(){
  Serial.begin(9600);
  u8g2.begin();//初始化演示器
  Timer1.initialize(5000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(timerIsr); // attach the service routine here
  // put your setup code here, to run once:  
   MX_GPIO_key_Init();
} 

void loop() {
  Serial.println("sssssssssssssssssssssssss");
  u8g2.clearBuffer();                //清除内部缓冲区
  u8g2.setFont(u8g2_font_8x13B_mr);//设置字体集
  u8g2.drawStr(40,20,"oc_learn! ");
  u8g2.drawStr(20,40,"belive you!!!");
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
