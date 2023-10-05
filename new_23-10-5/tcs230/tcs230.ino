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

// Pin definitions
//引脚定义
#define  S2_OUT  12
#define  S3_OUT  13
#define  OE_OUT   8    // LOW = ENABLED //默认和GND连接在一起

//创建对象
MD_TCS230  CS(S2_OUT, S3_OUT, OE_OUT);






void setup() 
{
  Serial.begin(115200);
  Serial.println("[TCS230 Simple NON_BLOCKING Example]");
  Serial.println("\nMove the sensor to different color to see the RGB value");
  Serial.println("Note: These values are being read in without sensor calibration");
  Serial.println("and are likely to be far from reality");

  CS.begin(); //初始化对象
}



void loop() 
{
  readSensor();
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
      colorData  rgb; //结构体
      
      CS.getRGB(&rgb);  //获取最后的RGB格式的数据并放入rag结构体
      Serial.print("RGB [");
      Serial.print(rgb.value[TCS230_RGB_R]);  //使用rgb.value获取结构体中的rgb数据
      Serial.print(",");
      Serial.print(rgb.value[TCS230_RGB_G]);
      Serial.print(",");
      Serial.print(rgb.value[TCS230_RGB_B]);
      Serial.println("]");
      
      waiting = false;
    }
  }
}


