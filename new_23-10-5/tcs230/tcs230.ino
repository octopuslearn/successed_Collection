// TCS230 sensor reading example
//
// This is just to show basic functionality without calibration.
// Utilises the non-blocking version of the reading function.
// Output uses the Serial console.
// 这只是为了显示基本功能，无需校准。
// 使用非阻塞版本的读取函数。
// 输出使用串行控制台。


#include <MD_TCS230.h>
#include <FreqCount.h>

/*ws2812*/
#include <FastLED.h>
#define NUM_LEDS 64 
#define DATA_PIN 2



// Pin definitions
//引脚定义
#define  S2_OUT  12
#define  S3_OUT  13
#define  OE_OUT   8    // LOW = ENABLED //默认和GND连接在一起

unsigned int tcs230_rgb[3]={};



//创建对象
MD_TCS230  CS(S2_OUT, S3_OUT, OE_OUT);

/*ws2812*/
//创建对象
CRGB leds[NUM_LEDS];


void setup() 
{
  Serial.begin(115200);

  FastLED.addLeds<WS2812,DATA_PIN,  GRB>(leds,NUM_LEDS);
  CS.begin(); //初始化对象
}



void loop() 
{
  readSensor();
  ledshow(tcs230_rgb[0], tcs230_rgb[1], tcs230_rgb[2]);
  /*测试颜色顺序*/
  // ledshow(255, 0, 0);
}





void readSensor()
{
  static  bool  waiting;  //自定义waiting=0
  
  if (!waiting) //waiting=0 !0=1 执行下面的程序
  {
    CS.read();  //从传感器开始读周期
    waiting = true;
  }
  else
  {
    if (CS.available()) //确定传感器读取完成，完成-true
    {
      colorData  rgb; //存储tcs230rgb数据-结构体
      
      CS.getRGB(&rgb);  //获取最后的RGB格式的数据并放入rag结构体
      tcs230_rgb[0] = rgb.value[TCS230_RGB_R];
      tcs230_rgb[1] = rgb.value[TCS230_RGB_G];
      tcs230_rgb[2] = rgb.value[TCS230_RGB_B];
      Serial.print("RGB [");
      Serial.print(tcs230_rgb[0]);  //使用rgb.value获取结构体中的rgb数据
      Serial.print(",");
      Serial.print(tcs230_rgb[1]);
      Serial.print(",");
      Serial.print(tcs230_rgb[2]);
      Serial.println("]");
      
      waiting = false;
    }
  }
}


void ledshow(unsigned int r, unsigned int g, unsigned int b)
{
      for(int i = 0; i < NUM_LEDS; i++) {
        // Set the i'th led to red 
        leds[i] = CRGB(r, g, b);
        // Show the leds
        FastLED.show(); 
        // now that we've shown the leds, reset the i'th led to black
        // leds[i] = CRGB::Black;
        // delay(10);
    }
}


