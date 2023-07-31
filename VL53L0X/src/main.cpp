#include <Arduino.h>
/*****插上一会儿，灯灭了，但是不影响使用*****/
/*****只能正对着用*****/
/*
GY56-----------MINI
VCC ---------- 5V
GND  --------- GND
Pin 5 -------- RC
Pin 4 -------- TD
*/
#include "Wire.h"

/******************以下，激光测距参数************************/
//The Arduino Wire library uses the 7-bit version of the address, so the code example uses 0x70 instead of the 8-bit 0xE0//Arduino Wire 库使用 7 位版本的地址，因此代码示例使用 0x70 而不是 8 位 0xE0
#define SensorAddress byte(0x70)
//The sensors ranging command has a value of 0x51//传感器测距命令的值为 0x51
//传感器测距命令的值为 0x51
#define RangeCommand byte(0x51)
//These are the two commands that need to be sent in sequence to change the sensor address//这是需要按顺序发送的两个命令来更改传感器地址
//这是需要按顺序发送的两个命令来更改传感器地址
#define ChangeAddressCommand1 byte(0xAA)
#define ChangeAddressCommand2 byte(0xA5)
/******************以上，激光测距参数************************/



/*******************以下，函数声明*********************/
word requestRange();//Returns the last range that the sensor determined in its last ranging cycle in centimeters. Returns 0 if there is no communication.//返回传感器在其上一个测距周期中确定的最后一个范围（以厘米为单位）。如果没有通信则返回 0。
void takeRangeReading();//Commands the sensor to take a range reading//命令传感器读取范围读数
/*******************以上，函数声明*********************/


/********************以下，自己新添了部分**********************/
word get_jiguang_value(unsigned int refrush);
/********************以上，自己新添了部分**********************/


void setup() 
{
    Serial.begin(9600); //Open serial connection at 9600 baud
    Wire.begin(); 
    // changeAddress(SensorAddress,0x40,0);//改变地址(传感器地址,0x40,0);
}
void loop()
{
    get_jiguang_value(10);
}




/********************以下，自己新添了部分**********************/
word get_jiguang_value(unsigned int refrush)
{
    unsigned long last_time=millis();
    takeRangeReading(); //Tell the sensor to perform a ranging cycle//告诉传感器执行一个测距周期
    while(millis()-last_time<refrush);//告诉传感器执行一个测距周期
    // delay(10); //Wait for sensor to finish//告诉传感器执行一个测距周期
    word range = requestRange(); //Get the range from the sensor//获取传感器的距离
    Serial.print("Range: "); Serial.println(range); //Print to the user//打印给用户
}
/********************以上，自己新添了部分**********************/



/****************************以下，激光测距模块函数***************************/
//Commands the sensor to take a range reading//命令传感器读取范围读数
void takeRangeReading()
{
  Wire.beginTransmission(SensorAddress); //Start addressing//开始寻址
  Wire.write(RangeCommand); //send range command//发送范围命令
  Wire.endTransmission(); //Stop and do something else now//现在停下来做点别的事
}


//Returns the last range that the sensor determined in its last ranging cycle in centimeters. Returns 0 if there is no communication.//返回传感器在其上一个测距周期中确定的最后一个范围（以厘米为单位）。如果没有通信则返回 0。
word requestRange()
{
    Wire.requestFrom(SensorAddress, byte(2));
    if(Wire.available() >= 2){ //Sensor responded with the two bytes//传感器响应两个字节
    byte HighByte = Wire.read(); //Read the high byte back//读回高字节
    byte LowByte = Wire.read(); //Read the low byte back//读回低字节
    word range = word(HighByte, LowByte); //Make a 16-bit word out of the two bytes for the range//用两个字节组成一个 16 位字作为范围
    return range;
    }
    else {
    return word(0); //Else nothing was received, return 0//否则什么也没收到，返回0
    }
}
/* Commands a sensor at oldAddress to change its address to newAddress
oldAddress must be the 7-bit form of the address that is used by Wire
7BitHuh determines whether newAddress is given as the new 7 bit version or the 8 bit version of the address
If true, if is the 7 bit version, if false, it is the 8 bit version
*/
/*命令 oldAddress 处的传感器将其地址更改为 newAddress
oldAddress 必须是 Wire 使用的地址的 7 位形式
7BitHuh 确定 newAddress 是作为地址的新 7 位版本还是 8 位版本给出
如果为 true，则为 7 位版本，如果为 false，则为 8 位版本
*/
void changeAddress(byte oldAddress, byte newAddress, boolean SevenBitHuh)
{
    Wire.beginTransmission(oldAddress); //Begin addressing//开始寻址
    Wire.write(ChangeAddressCommand1); //Send first change address command//发送第一个更改地址命令
    Wire.write(ChangeAddressCommand2); //Send second change address command//发送第二个改变地址命令
    byte temp;
    if(SevenBitHuh){ temp = newAddress << 1; } //The new address must be written to the sensor//新地址必须写入传感器
    else { temp = newAddress; } //in the 8bit form, so this handles automatic shifting//以8位形式，所以这处理自动移位
    Wire.write(temp); //Send the new address to change to//发送要更改的新地址
    Wire.endTransmission();
}
/****************************以上，激光测距模块函数***************************/
