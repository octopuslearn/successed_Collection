/*5.设置一个5ms间隔的定时器循环调用后台处理函数//这一步在void setup中
 * # MultiButton

## 简介
MultiButton 是一个小巧简单易用的事件驱动型按键驱动模块，可无限量扩展按键，按键事件的回调异步处理方式可以简化你的程序结构，去除冗余的按键处理硬编码，让你的按键业务逻辑更清晰。

## 使用方法
1.先申请一个按键结构

```c
struct Button button1;
```
2.初始化按键对象，绑定按键的GPIO电平读取接口**read_button_pin()** ，后一个参数设置有效触发电平

```c
button_init(&button1, read_button_pin, 0, 0);
```
3.注册按键事件

```c
button_attach(&button1, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
button_attach(&button1, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);
...
```
4.启动按键

```c
button_start(&button1);
```
5.设置一个5ms间隔的定时器循环调用后台处理函数//这一步在void setup中

```c
while(1) {//这一步在void setup中
    ...
    if(timer_ticks == 5) {
        timer_ticks = 0;

        button_ticks();
    }
}
 */



#include "key.h"

int key_to_u8g2 = 0;        //按键情况
struct Button button1;      //1.申请一个按键结构
void Callback_button1_Handler(void *btn);//回调函数
bool HAL_key0_ReadPin();

void MX_GPIO_key_Init()
{
  pinMode(key0_Pin, INPUT_PULLUP);
  button_init(&button1, HAL_key0_ReadPin, KEY0_DOWN);     //2.初始化按键对象
  /*3.注册按键事件*/                                                          
  button_attach(&button1, PRESS_UP,           Callback_button1_Handler);      //按键弹起，每次松开都触发  
  button_attach(&button1, PRESS_DOWN,         Callback_button1_Handler);      //按键按下，每次按下都触发
  button_attach(&button1, PRESS_REPEAT,       Callback_button1_Handler);      //重复按下触发，变量repeat计数连击次数
  button_attach(&button1, SINGLE_CLICK,       Callback_button1_Handler);      //单击按键事件
  button_attach(&button1, DOUBLE_CLICK,       Callback_button1_Handler);      //双击按键事件  
  button_attach(&button1, LONG_PRESS_START,   Callback_button1_Handler);      //达到长按时间阈值时触发一次
  button_attach(&button1, LONG_PRESS_HOLD,    Callback_button1_Handler);      //长按期间一直触发
  button_start(&button1);                               //4.启动按键
}


bool HAL_key0_ReadPin()
{
  bool HAL_key0_ReadPin_value = digitalRead(key0_Pin);
  return HAL_key0_ReadPin_value;
}

void Callback_button1_Handler(void* btn)
{
    switch(get_button_event((struct Button*)btn))
  {
    case PRESS_DOWN:
      key_to_u8g2 = 1;
      Serial.println("KEY0:PRESS_DOWN\n");
    break;

    case PRESS_UP:
      key_to_u8g2 = 2;
      Serial.println("KEY0:PRESS_UP\n");
    break;
    
    case PRESS_REPEAT:
      key_to_u8g2 = 3;
      Serial.println("KEY0:PRESS_REPEAT\n");
    break;
    
    case SINGLE_CLICK:
      key_to_u8g2 = 4;
      Serial.println("KEY0:SINGLE_CLICK\n");
    break;
    
    case DOUBLE_CLICK:
      key_to_u8g2 = 5;
      Serial.println("KEY0:DOUBLE_CLICK\n");
    break;
   
    case LONG_PRESS_START:
      key_to_u8g2 = 6;
      Serial.println("KEY0:LONG_PRESS_START\n");
    break;
    
    case LONG_PRESS_HOLD:
      key_to_u8g2 = 7;
      Serial.println("KEY0:LONG_PRESS_HOLD\n");
    break;
    
    default:break;
  }
}

