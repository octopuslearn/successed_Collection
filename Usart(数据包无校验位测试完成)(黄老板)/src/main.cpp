#include <Arduino.h>






/**************以下，函数声明******************/
void ReceiveDate(uint8_t num); //接收函数,每次调用只接收四个数据包里准确的数据刷新进全局data数组，接收正确就消亡了，要再接收必须重新调用。
/**************以上，函数声明******************/

unsigned int data[4];


void setup() {
  Serial.begin(115200); // 初始化串口通信，波特率为9600
  Serial1.begin(115200);    // 初始化虚拟串口
}

void loop() {
    ReceiveDate(4);            //规定只接收四个数据
    Serial.print(data[0]);   Serial.print('\t');//打印第三个数据 Serial.print(data[0]);
    Serial.print(data[1]);   Serial.print('\t');//打印第三个数据 
    Serial.print(data[2]);   Serial.print('\t');//打印第三个数据 
    Serial.println(data[3]);   //打印第三个数据 
}


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



