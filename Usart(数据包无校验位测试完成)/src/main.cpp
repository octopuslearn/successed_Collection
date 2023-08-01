#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX




/**************以下，函数声明******************/
void sending_data(unsigned int First_data, unsigned int Second_data, unsigned int Third_data,unsigned int Four_data); //发送四个数据函数
void ReceiveDate(uint8_t num); //接收函数,每次调用只接收四个数据包里准确的数据刷新进全局data数组，接收正确就消亡了，要再接收必须重新调用。
/**************以上，函数声明******************/

unsigned int data[4];


void setup() {
  Serial.begin(115200); // 初始化串口通信，波特率为9600
  mySerial.begin(115200);    // 初始化虚拟串口
}

void loop() {
//  while(Serial.available()>0) char wrongCommand = Serial.read();  //清除串口缓存的错误指令
    sending_data(4,5,6,7);
    ReceiveDate(4);            //规定只接收四个数据
    Serial.println(data[2]);   //打印第三个数据 
  // for (int i = 0; i <3; i++) {
  //   Serial.print(data[i]); // 以十六进制格式输出数据包内容
  // }

}


void ReceiveDate(uint8_t num){
    uint8_t Flag =1;
    unsigned int RxState = 0;
    unsigned int RxCounter1 = 0;
    while (Flag)
    {
      while(Serial.available() > 0) {   
          uint16_t com_data = Serial.read();
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
        mySerial.write(data[i]);
    }
}


