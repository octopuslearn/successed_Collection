#include "FastLED.h"            // 此示例程序需要使用FastLED库

#define LED 3
#define microphone A0
#define threshold 580 //声控用阈值

int irSensorPin = 4 ;            // 连接红外传感器引脚
bool irSensorOutput;            // 红外传感器输出信号


#define NUM_LEDS 120             // LED灯珠数量
#define DATA_PIN 3             // Arduino输出控制信号引脚
#define LED_TYPE WS2812         // LED灯带型号
#define COLOR_ORDER GRB         // RGB灯珠中红色、绿色、蓝色LED的排列顺序
uint8_t max_bright = 128;       // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高
 
CRGB leds[NUM_LEDS];            // 建立光带leds
 
void setup() { 
  pinMode(irSensorPin, INPUT);
//  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带 
  FastLED.setBrightness(max_bright);                            // 设置光带亮度
}
 
void loop() { 
  int intput=analogRead(microphone);
  char output_led;
  
  irSensorOutput = digitalRead(irSensorPin);  // 读取红外传感器输出

  Serial.print("intput: "); Serial.print(intput); 

  if ((irSensorOutput == HIGH)||(intput>=threshold)) {  // 如果红外传感器输出高电平
      Serial.println("IR Motion Sensor OUTPUT: HIGH.");
      Serial.print("aaaa");
      output_led=1;
  } 
  else {
      Serial.println("IR Motion Sensor OUTPUT: LOW.");
      Serial.print("zzzz");
      output_led=0;
  }
  switch(output_led)
  {
    case 0: fill_solid(leds,120,CRGB::Black); FastLED.show();  break;
    case 1: fill_solid(leds+3,120,CRGB::White); FastLED.show();  break;
  }
  
  FastLED.show();               // 更新LED色彩
  delay(500);                     // 等待500毫秒
}
