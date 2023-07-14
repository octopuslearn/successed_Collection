// ////据猜测1-黑色  --->黑色为1
// ////根据识别到的数字，执行相应的动作，是写死的
// ////digitalWrite(AdirPin,HIGH);//A电机顺时针为向后 ////据分析--->A-左轮
// ////AnitClockwisePX右转循迹函数
// ////ClockwisePX(10,3000);//左转循迹函数
// ////  Advance(Line,2000); ////过线后移动的距离 --->Advance-向前
// ////  AnitClockwise(ZJ,2000); ////转90读
// /*以下，结论错误*/
// ////  digitalWrite(AdirPin,LOW);  --->AdirPin,LOW-左轮向前
// ////  digitalWrite(BdirPin,HIGH); --->BdirPin,HIGH-右轮向前
// /*以上，结论错误*/
// ////void AnitClockwise(int a,int t)  //a为相对逆时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
// ////{   
// ////   digitalWrite(AdirPin,LOW);//A电机逆时针为向前  ////此处仅A-向前 --->左轮停机-左转（右轮转）xxx错误 视频上是左轮不动右轮动--->-ALOW,右轮前进
// ////正确结论 --->AdirPin,LOW-右轮向前
// ////正确结论 --->BdirPin,HIGH-左轮向前

// ////AstepPin-给DIR脉冲控制，一个高一个低，走一步
// /*AstepPin-给DIR脉冲控制，一个高一个低，走一步
//    for(int i=0;i<a;i++) ////给DIR脉冲控制，一个高一个低，走一步
//    {
//      digitalWrite(AstepPin,HIGH); 
//      digitalWrite(BstepPin,HIGH);
//      delayMicroseconds(t/2);
//      digitalWrite(AstepPin,LOW); 
//      digitalWrite(BstepPin,LOW);
//      delayMicroseconds(t/2);
//    }
// }
// */

// //分析得出 循迹-左右反向同速

// #include <U8g2lib.h>

// #define R 10 //红灯
// #define Y 9 //黄灯
// #define G 8 //绿灯
// #define BEEP 13 //蜂鸣器40

// #define AdirPin 3     //// --->AdirPin,LOW-左轮向前   A方向控制引脚 DIR-控制方向的 HIGH-正转 LOW-反转  --->左轮方向控制
// #define AstepPin 4    //// A步进控制引脚 PUL-脉冲控制的                    --->左轮步进控制
// #define BdirPin 5     //// --->BdirPin,HIGH-左轮向前  B方向控制引脚
// #define BstepPin 6    //// B步进控制引脚

// #define Q1 23 ////灰度传感器
// #define Q2 31 ////灰度传感器
// #define Q3 25 ////左  /*使用了Q3-Q6*/
// #define Q4 33 ////中左  /*使用了Q3-Q6*/
// #define Q5 27 ////中右  /*使用了Q3-Q6*/
// #define Q6 35 ////右  /*使用了Q3-Q6*/
// #define Q7 29 ////灰度传感器
// #define Q8 37 ////灰度传感器

// #define Line 250 //过线后移动距离
// #define Line2 230 //过线后移动距离
// #define ZJ 1000 //旋转90度
// #define box 12 //药仓检测 ////pinMode(box,INPUT);

// int Aim=0;//目的地编号
// int ZY=3;
// int count=0;//过线数
// int Mid=2;
// int Far=2;
// int Long=2;
// int place=0;//绝对位置

// String Data=" "; 
// char inData=' ';
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,21,20,U8X8_PIN_NONE);
// void OLED_Show(void);

// void setup() {
//   Serial.begin(9600);
//   Serial1.begin(9600);//连接蓝牙
//   Serial2.begin(9600);//连接openmv
//   Serial3.begin(9600);
//   pinMode(2,OUTPUT);
//   digitalWrite(2,HIGH);//步进电机电平比较端
//   pinMode(AdirPin,OUTPUT);
//   pinMode(AstepPin,OUTPUT);
//   pinMode(BdirPin,OUTPUT);
//   pinMode(BstepPin,OUTPUT);
//   pinMode(R,OUTPUT);digitalWrite(R,HIGH);
//   pinMode(Y,OUTPUT);digitalWrite(Y,HIGH);
//   pinMode(G,OUTPUT);digitalWrite(G,HIGH);
//   pinMode(box,INPUT);
//   pinMode(Q1,INPUT);pinMode(Q2,INPUT);
//   pinMode(Q3,INPUT);pinMode(Q4,INPUT);
//   pinMode(Q5,INPUT);pinMode(Q6,INPUT);
//   pinMode(Q7,INPUT);pinMode(Q8,INPUT);
//   pinMode(BEEP,OUTPUT);digitalWrite(BEEP,LOW);
//   u8g2.begin();
//   u8g2.clearBuffer();  
//   u8g2.setFont(u8g2_font_logisoso28_tf);
//   u8g2.drawStr(0,48,"-------");
//   u8g2.sendBuffer();  
// }

// void OpenmvRead(void)
// {
//   while(Serial2.read()>=0);//清除缓存
//   while(Serial2.available()==0);//等待信号传输
//   if(Serial2.available())//接收到openmv发来的病房号数字
//   {
//     inData=Serial2.read();//读取接收的字符
//     if(inData=='1')Aim=1;
//     if(inData=='2')Aim=2;
//     if(inData=='3')Aim=3;
//     if(inData=='4')Aim=4;
//     if(inData=='5')Aim=5;
//     if(inData=='6')Aim=6;
//     if(inData=='7')Aim=7;
//     if(inData=='8')Aim=8;
//     inData=' ';
//     while(Serial2.read()>=0);//清除缓存区数据
//   }
// }

// void OpenmvZY(void)
// {
//   while(Serial2.read()>=0);
//   Serial2.write(0x31);//发送开启指令
//   while(Serial2.available()==0);//等待信号传输
//   if(Serial2.available())
//   {
//     inData=Serial2.read();//读取第一个接收的字符
//     if(inData=='1')
//     {
//       ZY=0;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---Z---");
//       u8g2.sendBuffer();
//     }
//     if(inData=='2')
//     {
//       ZY=1;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---Y---");
//       u8g2.sendBuffer();
//     }
//     if(inData=='0')
//     {
//       ZY=2;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---N---");
//       u8g2.sendBuffer();
//     }
//     inData=' ';
//     while(Serial2.read()>=0);//清除缓存区数据
//   }
// }

// void BTpalce(void)//蓝牙发送地址信息（绝对）
// {
//   if(Far==2||Long==2)//如果没有到达远端，只在中端结束，则发送结束目的位置
//   {
//     if(Mid==0)
//     {
//       Serial1.print('3');
//       place=3;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---3---");
//       u8g2.sendBuffer(); 
//     }
//     if(Mid==1)
//     {
//       Serial1.print('4');
//       place=4;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---4---");
//       u8g2.sendBuffer(); 
//     }
//   }else{
//     if(Far==0&&Long==0)
//     {
//       Serial1.print('5');
//       place=5;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---5---");
//       u8g2.sendBuffer(); 
//     }
//     if(Far==0&&Long==1)
//     {
//       Serial1.print('7');
//       place=7;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---7---");
//       u8g2.sendBuffer(); 
//     }
//     if(Far==1&&Long==0)
//     {
//       Serial1.print('6');
//       place=6;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---6---");
//       u8g2.sendBuffer(); 
//     }
//     if(Far==1&&Long==1)
//     {
//       Serial1.print('8');
//       place=8;
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---8---");
//       u8g2.sendBuffer(); 
//     }
//   }
// }


// // /*步进，给脉冲的*/
// /*前进*/
// void Advance(int a,int t)  
// //a为前进距离，x=800时为轮子转一圈，距离约为23.56cm。
// //t为一次脉冲的周期时间（us）。频率f=1/t。
// {
//    digitalWrite(AdirPin,LOW);//A电机逆时针为向前  ////LOW-向前  //??既然LOW-向前，那么HIGH-向后or停止？--->HIGH向前，LOW向后
//    digitalWrite(BdirPin,HIGH);//B电机顺时针为向前 ////HIGH-向前
//    for(int i=0;i<a;i++)
//    {
//      digitalWrite(AstepPin,HIGH); 
//      digitalWrite(BstepPin,HIGH); 
//      delayMicroseconds(t/2);
//      digitalWrite(AstepPin,LOW);  
//      digitalWrite(BstepPin,LOW);  
//      delayMicroseconds(t/2);   
//    }
// }

// void Back(int a,int t)  //a为后退距离，x=800时为轮子转一圈，距离约为23.56cm。 t为一次脉冲的周期时间。频率f=1/t。
// {
//    digitalWrite(AdirPin,HIGH);//A电机顺时针为向后
//    digitalWrite(BdirPin,LOW);//B电机逆时针为向后
//    for(int i=0;i<a;i++)
//    {
//      digitalWrite(AstepPin,HIGH); 
//      digitalWrite(BstepPin,HIGH);
//      delayMicroseconds(t/2);
//      digitalWrite(AstepPin,LOW); 
//      digitalWrite(BstepPin,LOW);
//      delayMicroseconds(t/2);
//    }
// }

// void Clockwise(int a,int t)  //a为相对顺时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
// {
//    digitalWrite(BdirPin,HIGH);//B电机顺时针为向前
//    for(int i=0;i<a;i++)
//    {
//      digitalWrite(BstepPin,HIGH);
//      delayMicroseconds(t/2);
//      digitalWrite(BstepPin,LOW);
//      delayMicroseconds(t/2);
//    }
//    while(!digitalRead(Q4)&&!digitalRead(Q5))
//    {
//      for(int i=0;i<10;i++)
//      {
//        digitalWrite(BstepPin,HIGH);
//        delayMicroseconds(t/2);
//        digitalWrite(BstepPin,LOW);
//        delayMicroseconds(t/2);
//      }
//    }
//    for(int i=0;i<10;i++)
//    {
//      digitalWrite(BstepPin,HIGH);
//      delayMicroseconds(t/2);
//      digitalWrite(BstepPin,LOW);
//      delayMicroseconds(t/2);
//    }
// }

// /**/
// void AnitClockwise(int a,int t)  //a为相对逆时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
// {   
//    digitalWrite(AdirPin,LOW);//A电机逆时针为向前  ////此处仅A-向前 --->左轮停机-左转（右轮转）xxx错误 视频上是左轮不动右轮动--->-ALOW,右轮前进
//    for(int i=0;i<a;i++)
//    {
//      digitalWrite(AstepPin,HIGH); 
//      delayMicroseconds(t/2);
//      digitalWrite(AstepPin,LOW);  
//      delayMicroseconds(t/2);
//    }
//    while(!digitalRead(Q4)&&!digitalRead(Q5))  ////白底上时左转
//    {
//      for(int i=0;i<10;i++)
//      {
//        digitalWrite(AstepPin,HIGH); 
//        delayMicroseconds(t/2);
//        digitalWrite(AstepPin,LOW); 
//        delayMicroseconds(t/2);
//      }
//    }
//    for(int i=0;i<10;i++)  ////?
//    {
//      digitalWrite(AstepPin,HIGH); 
//      delayMicroseconds(t/2);
//      digitalWrite(AstepPin,LOW); 
//      delayMicroseconds(t/2);
//    }
// }


// ///*据注释顺时针，但是函数英文名为反顺时针-逆时针*/
// /*右转循迹函数*/
// void AnitClockwisePX(int a,int t)  //a为相对顺时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
// {
//    digitalWrite(AdirPin,HIGH);//A电机顺时针为向后 ////--->A-左轮-正转
//    digitalWrite(BdirPin,HIGH);//B电机顺时针为向前 ////B？
//    for(int i=0;i<a;i++) ////给DIR脉冲控制，一个高一个低，走一步
//    {
//      digitalWrite(AstepPin,HIGH); 
//      digitalWrite(BstepPin,HIGH);
//      delayMicroseconds(t/2);
//      digitalWrite(AstepPin,LOW); 
//      digitalWrite(BstepPin,LOW);
//      delayMicroseconds(t/2);
//    }
// }

// /*左转循迹函数*/
// void ClockwisePX(int a,int t)  //a为相对逆时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
// {
//    digitalWrite(AdirPin,LOW);//A电机逆时针为向前
//    digitalWrite(BdirPin,LOW);//B电机逆时针为向后
//    for(int i=0;i<a;i++)
//    {
//      digitalWrite(AstepPin,HIGH); 
//      digitalWrite(BstepPin,HIGH);
//      delayMicroseconds(t/2);
//      digitalWrite(AstepPin,LOW); 
//      digitalWrite(BstepPin,LOW);
//      delayMicroseconds(t/2);
//    }
// }

// void track_Q()    ////循迹
// {
//   int D1=digitalRead(Q3);
//   int D2=digitalRead(Q4);
//   int D3=digitalRead(Q5);
//   int D4=digitalRead(Q6);
//   if((D1==0)&&(D2==0)&&(D3==0)&&(D4==1))  ////假设D4（最右边）-左偏=右转-左轮向前--->左偏
//   {
//      AnitClockwisePX(10, 3000); ////右转循迹
//   }else if((D1==0)&&(D2==0)&&(D3==1)&&(D4==0))
//   {
//     AnitClockwisePX(10,3000);
//   }else if((D1==0)&&(D2==0)&&(D3==1)&&(D4==1))
//   {
//     AnitClockwisePX(10,3000);
//   }else if((D1==1)&&(D2==0)&&(D3==0)&&(D4==0))  ////右偏
//   {
//     ClockwisePX(10,3000); ////顺时针            ////左转循迹
//   }else if((D1==0)&&(D2==1)&&(D3==0)&&(D4==0))
//   {
//     ClockwisePX(10,3000);
//   }else if((D1==1)&&(D2==1)&&(D3==0)&&(D4==0))
//   {
//     ClockwisePX(10,3000);
//   }
//   Advance(50,1800); ////否则前进
// }

// void loop() {
//   //调试
// //  if(Serial1.available())//蓝牙接收器
// //  {
// //    Data=Serial1.readString();//读取第一个接收的字符
// //    if(Data=="ON")
// //    {
//       //主代码------------------------------------
//       OpenmvRead();//获取任务码
//       OLED_Show();//显示目的地任务
//       while(digitalRead(box)==0);//等待药物放下
//       delay(500);
//       if(Aim==1)  //目的地编号  ////1号位置在左边
//       {
//         int X=1;
//         while(X)
//         {
//           track_Q();  ////循迹的时候就前进了
//           if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))  //到达第一个T字路口
//           {
//             X=0;
//             break;
//           }
//         }
//         Advance(Line,2000); ////过线后移动的距离
//         AnitClockwise(ZJ,2000); ////左转90度-视频上看，左轮不动，右轮动
// //        while(!digitalRead(Q4)||!digitalRead(Q5)){AnitClockwise(ZJ,2000);}
//         X=1;
//         while(X)
//         {
//           track_Q();//灰度传感器状态全白即到达位置------------------------------------------------ ////如此来看1-黑
//           if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//           {
//             Advance(30,2000);
//             if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//             {
//               X=0;
//               break;
//             }
//           }
//         }
//         //已到达收货区1
//         digitalWrite(R,LOW);//亮红灯
//         digitalWrite(BEEP,HIGH);
//         delay(500);
//         digitalWrite(BEEP,LOW);
//         BTpalce();
//         while(digitalRead(box)==1); ////--->药品放上面时是高电平
//         delay(500);
//         digitalWrite(R,HIGH);//灭红灯
//         //开始返回
//         Back(100,2000);
//         X=1;
//         while(X)
//         {
//           Back(30,2000);
//           if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))
//           {
//             X=0;
//             break;
//           }
//         }
//         Advance(Line,2000);
//         AnitClockwise(ZJ,2000);
//         X=1;
//         while(X)
//         {
//           track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//           if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//           {
//             Advance(30,2000);
//             if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//             {
//               X=0;
//               break;
//             }
//           }
//         }
//         digitalWrite(G,LOW);//亮绿灯
//         digitalWrite(BEEP,HIGH);
//         delay(500);
//         digitalWrite(BEEP,LOW);
//         while(1);
//       }
//       if(Aim==2)
//       {
//         int X=1;
//         while(X)
//         {
//           track_Q();
//           if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//           {
//             X=0;
//             break;
//           }
//         }
//         Advance(Line,2000);
//         Clockwise(ZJ,2000);
// //        while(!digitalRead(Q4)||!digitalRead(Q5)){Clockwise(ZJ,2000);}
//         X=1;
//         while(X)
//         {
//           track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//           if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//           {
//             Advance(30,2000);
//             if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//             {
//               X=0;
//               break;
//             }
//           }
//         }
//         //已到达收货区2
//         digitalWrite(R,LOW);//亮红灯
//         digitalWrite(BEEP,HIGH);
//         delay(500);
//         digitalWrite(BEEP,LOW);
//         BTpalce();
//         while(digitalRead(box)==1);
//         delay(500);
//         digitalWrite(R,HIGH);//灭红灯
//         //开始返回
//         Back(100,2000);
//         X=1;
//         while(X)
//         {
//           Back(30,2000);
//           if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))
//           {
//             X=0;
//             break;
//           }
//         }
//         Advance(Line,2000);
//         Clockwise(ZJ,2000);
//         X=1;
//         while(X)
//         {
//           track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//           if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//           {
//             Advance(30,2000);
//             if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//             {
//               X=0;
//               break;
//             }
//           }
//         }
//         digitalWrite(G,LOW);//亮绿灯
//         digitalWrite(BEEP,HIGH);
//         delay(500);
//         digitalWrite(BEEP,LOW);
//         while(1);
//       }
//       if(Aim>2)//中部或远端病房
//       {
//         int X=1;
//         while(X)
//         {
//           track_Q();
//           if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//           {
//             X=0;
//             break;
//           }
//         }
//         int i=53;//近端到中端---------------------------------------------------------
//         while(i--){track_Q();}       
//         OpenmvZY();//读取左右
//         switch(ZY)
//         {
//           case 0:{//在左边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             AnitClockwise(ZJ,2000);//逆时针左转
//             Mid=0;
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             //已到达收货区3
//             digitalWrite(R,LOW);//亮红灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             BTpalce();
//             while(digitalRead(box)==1);
//             delay(500);
//             Serial1.print('O');
//             digitalWrite(R,HIGH);//灭红灯
//             //开始返回
//             Back(100,2000);
//             X=1;
//             while(X)
//             {
//               Back(30,2000);
//               if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             AnitClockwise(ZJ,2000);
//             Serial1.print('K');
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             digitalWrite(G,LOW);//亮绿灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             while(1);
//             break;
//           }
//           case 1:{//在右边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             Clockwise(ZJ,2000);//顺时针左转
//             Mid=1;
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             //已到达收货区4
//             digitalWrite(R,LOW);//亮红灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             BTpalce();
//             while(digitalRead(box)==1);
//             delay(500);
//             Serial1.print('O');
//             digitalWrite(R,HIGH);//灭红灯
//             //开始返回
//             Back(100,2000);
//             X=1;
//             while(X)
//             {
//               Back(30,2000);
//               if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             Clockwise(ZJ,2000);
//             Serial1.print('K');
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             digitalWrite(G,LOW);//亮绿灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             while(1);
//             break;
//           }
//           case 2:break;
//         }


        
// //基础任务3=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        
        
        
//         X=1;
//         while(X)
//         {
//           track_Q();
//           if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//           {
//             X=0;
//             break;
//           }
//         }
//         i=53;//中端到远端-----------------------------------------------------
//         while(i--){track_Q();}
//         //到达4个数字前
//         OpenmvZY();//读取左右
//         switch(ZY)
//         {
//           case 0:{//在左边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             AnitClockwise(ZJ,2000);//逆时针左转
//             Far=0;//记录左转
//             break;
//           }
//           case 1:{//在右边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             Clockwise(ZJ,2000);//顺时针左转
//             Far=1;//记录右转
//             break;
//           }
//           case 2:{//随机转一个方向：右
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             Clockwise(ZJ,2000);//顺时针
//             Far=1;//记录右转
//             break;
//           }
//         }
        
//    //==============================================================================
        
//         i=34;//远端到分支左、右
//         while(i--){track_Q();}
//         OpenmvZY();//读取左右
//         switch(ZY)
//         {
//           case 0:{//在左边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             AnitClockwise(ZJ,2000);//逆时针左转
//             Long=0;
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             //已到达收货区5、8
//             digitalWrite(R,LOW);//亮红灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             BTpalce();
//             while(digitalRead(box)==1);
//             delay(500);
//             digitalWrite(R,HIGH);//灭红灯
//             //开始返回（特殊T形）
//             Back(100,2000);
//             X=1;
//             while(X)
//             {
//               Back(30,2000);
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             AnitClockwise(ZJ,2000);
//             //经过第二个T形
//             if(Far==0)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               Clockwise(ZJ,2000);
//             }
//             if(Far==1)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               AnitClockwise(ZJ,2000);
//             }
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 Serial1.print('K');//全黑是发送指令
//               }
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             digitalWrite(G,LOW);//亮绿灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             while(1);
//             break;
//           }
//           case 1:{//在右边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             Clockwise(ZJ,2000);//顺时针右转
//             Long=1;
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             //已到达收货区6、7
//             digitalWrite(R,LOW);//亮红灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             BTpalce();
//             while(digitalRead(box)==1);
//             delay(500);
//             digitalWrite(R,HIGH);//灭红灯
//             //开始返回（特殊T形）
//             Back(100,2000);
//             X=1;
//             while(X)
//             {
//               Back(30,2000);
//               if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             Clockwise(ZJ,2000);
//             //经过第二个T形
//             if(Far==0)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               Clockwise(ZJ,2000);
//             }
//             if(Far==1)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               AnitClockwise(ZJ,2000);
//             }
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 Serial1.print('K');//全黑是发送指令
//               }
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             digitalWrite(G,LOW);//亮绿灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             while(1);
//             break;
//           }
//           case 2:{//走错了
//             Far=0;//记录右转
//             X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             AnitClockwise(ZJ,2000);
//             X=1;
//             while(X)
//             {
//               Back(30,2000);
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             AnitClockwise(ZJ,2000);
//             X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//               {
//                 X=0;
//                 break;
//               }
//             }
// //走错了============================================================================
//             i=51;
//             while(i--){track_Q();}
//             OpenmvZY();//读取左右
//             while(ZY==3)
//             {
//               track_Q();
//               delay(100);
//               OpenmvZY();
//             }
//         switch(ZY)
//             {
//               case 0:{//在左边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             AnitClockwise(ZJ,2000);//逆时针左转
//             Long=0;
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             //已到达收货区5、8
//             digitalWrite(R,LOW);//亮红灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             BTpalce();
//             while(digitalRead(box)==1);
//             delay(500);
//             digitalWrite(R,HIGH);//灭红灯
//             //开始返回（特殊T形）
//             Back(100,2000);
//             X=1;
//             while(X)
//             {
//               Back(30,2000);
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             AnitClockwise(ZJ,2000);
//             //经过第二个T形
//             if(Far==0)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               Clockwise(ZJ,2000);
//             }
//             if(Far==1)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               AnitClockwise(ZJ,2000);
//             }
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 Serial1.print('K');//全黑是发送指令
//               }
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             digitalWrite(G,LOW);//亮绿灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             while(1);
//             break;
//           }  
//           case 1:{//在右边
//             int X=1;
//             while(X)
//             {
//               track_Q();
//               if(digitalRead(Q1)&&digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line2,2000);
//             Clockwise(ZJ,2000);//顺时针右转
//             Long=1;
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             //已到达收货区6、7
//             digitalWrite(R,LOW);//亮红灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             BTpalce();
//             while(digitalRead(box)==1);
//             delay(500);
//             digitalWrite(R,HIGH);//灭红灯
//             //开始返回（特殊T形）
//             Back(100,2000);
//             X=1;
//             while(X)
//             {
//               Back(30,2000);
//               if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 X=0;
//                 break;
//               }
//             }
//             Advance(Line,2000);
//             Clockwise(ZJ,2000);
//             //经过第二个T形
//             if(Far==0)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               Clockwise(ZJ,2000);
//             }
//             if(Far==1)
//             {
//               X=1;
//               while(X)
//               {
//                 track_Q();
//                 if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//               Advance(Line,2000);
//               AnitClockwise(ZJ,2000);
//             }
//             X=1;
//             while(X)
//             {
//               track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
//               if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
//               {
//                 Serial1.print('K');//全黑是发送指令
//               }
//               if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//               {
//                 Advance(30,2000);
//                 if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
//                 {
//                   X=0;
//                   break;
//                 }
//               }
//             }
//             digitalWrite(G,LOW);//亮绿灯
//             digitalWrite(BEEP,HIGH);
//             delay(500);
//             digitalWrite(BEEP,LOW);
//             while(1);
//             break;
//           }
//           }
//           }
//         }
//         while(1);
//       }
// //    }//结束
// //    if(Data=="A"){OpenmvRead();OLED_Show();}
// //    if(Data=="B"){OpenmvZY();Serial1.println(ZY);}
// //    if(Data=="C"){OpenmvZY4();Serial1.println(ZY);}
// //    if(Data=="TQ"){while(1){track_Q();}}
// //    if(Data=="place")BTpalce();
// //    if(Data=="Qian")Advance(200,2000);
// //    if(Data=="Hou")Back(200,2000);
// //    if(Data=="QianShort")Advance(100,2000);
// //    if(Data=="HouShort")Back(100,2000);
// //    if(Data=="QianLong")Advance(400,1000);
// //    if(Data=="HouLong")Back(400,1000);
// //    if(Data=="Shun")AnitClockwisePX(1120,1000);
// //    if(Data=="Ni")ClockwisePX(1120,1000);
// //    if(Data=="readQ")
// //    {
// //      Serial1.print(digitalRead(Q1));Serial1.print(digitalRead(Q2));
// //      Serial1.print(digitalRead(Q3));Serial1.print(digitalRead(Q4));
// //      Serial1.print(digitalRead(Q5));Serial1.print(digitalRead(Q6));
// //      Serial1.print(digitalRead(Q7));Serial1.println(digitalRead(Q8));
// //    }
// //    Data=" ";
// //  }
// }

// /*调试，OLED显示目标位置*/
// void OLED_Show(void)
// {
//   switch(Aim)
//   {
//     case 1:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---1---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//     case 2:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---2---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//     case 3:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---3---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//     case 4:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---4---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//     case 5:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---5---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//     case 6:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---6---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//     case 7:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---7---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//     case 8:{
//       u8g2.clearBuffer();  
//       u8g2.setFont(u8g2_font_logisoso28_tf);
//       u8g2.drawStr(0,48,"---8---");
//       u8g2.sendBuffer(); 
//       break;
//     }
//   }
// }



////据猜测1-黑色  --->黑色为1
////根据识别到的数字，执行相应的动作，是写死的
////digitalWrite(AdirPin,HIGH);//A电机顺时针为向后 ////据分析--->A-左轮
////AnitClockwisePX右转循迹函数
////ClockwisePX(10,3000);//左转循迹函数
////  Advance(Line,2000); ////过线后移动的距离 --->Advance-向前
////  AnitClockwise(ZJ,2000); ////转90读
/*以下，结论错误*/
////  digitalWrite(AdirPin,LOW);  --->AdirPin,LOW-左轮向前
////  digitalWrite(BdirPin,HIGH); --->BdirPin,HIGH-右轮向前
/*以上，结论错误*/
////void AnitClockwise(int a,int t)  //a为相对逆时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
////{   
////   digitalWrite(AdirPin,LOW);//A电机逆时针为向前  ////此处仅A-向前 --->左轮停机-左转（右轮转）xxx错误 视频上是左轮不动右轮动--->-ALOW,右轮前进
////正确结论 --->AdirPin,LOW-右轮向前
////正确结论 --->BdirPin,HIGH-左轮向前
////while(digitalRead(box)==1); ////--->药品放上面时是高电平


////返回转弯策略：和前进时的转弯策略相同
/*以下，详情返回转弯策略：和前进时的转弯策略相同*/
        //开始返回
        // Back(100,2000); ////后退一定时间
        // X=1;
        // while(X)
        // {
        //   Back(30,2000);
        //   if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))  ////后退直到线上
        //   {
        //     X=0;
        //     break;
        //   }
        // }
        // Advance(Line,2000); ////前进一段时间
        // AnitClockwise(ZJ,2000);
/*以上，详情返回转弯策略：和前进时的转弯策略相同*/


////ZY-0-左转
////ZY-1-右转
////ZY-2-N
/*依据
      ZY=2;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---N---");
      u8g2.sendBuffer();
*/


////int i=53;//近端到中端---------------------------------------------------------  ////从近端开始累计-到达中端是累计循迹,到达中端的十字路口前开始识别
////        while(i--){track_Q();}       

////AstepPin-给DIR脉冲控制，一个高一个低，走一步
/*AstepPin-给DIR脉冲控制，一个高一个低，走一步
   for(int i=0;i<a;i++) ////给DIR脉冲控制，一个高一个低，走一步
   {
     digitalWrite(AstepPin,HIGH); 
     digitalWrite(BstepPin,HIGH);
     delayMicroseconds(t/2);
     digitalWrite(AstepPin,LOW); 
     digitalWrite(BstepPin,LOW);
     delayMicroseconds(t/2);
   }
}
*/

//分析得出 循迹-左右反向同速

#include <U8g2lib.h>

#define R 10 //红灯
#define Y 9 //黄灯
#define G 8 //绿灯
#define BEEP 13 //蜂鸣器40

#define AdirPin 3     //// --->AdirPin,LOW-左轮向前   A方向控制引脚 DIR-控制方向的 HIGH-正转 LOW-反转  --->左轮方向控制
#define AstepPin 4    //// A步进控制引脚 PUL-脉冲控制的                    --->左轮步进控制
#define BdirPin 5     //// --->BdirPin,HIGH-左轮向前  B方向控制引脚
#define BstepPin 6    //// B步进控制引脚

#define Q1 23 ////灰度传感器
#define Q2 31 ////灰度传感器
#define Q3 25 ////左  /*使用了Q3-Q6*/
#define Q4 33 ////中左  /*使用了Q3-Q6*/
#define Q5 27 ////中右  /*使用了Q3-Q6*/
#define Q6 35 ////右  /*使用了Q3-Q6*/
#define Q7 29 ////灰度传感器
#define Q8 37 ////灰度传感器

#define Line 250 //过线后移动距离
#define Line2 230 //过线后移动距离
#define ZJ 1000 //旋转90度
#define box 12 //药仓检测 ////pinMode(box,INPUT);

int Aim=0;//目的地编号
int ZY=3;
int count=0;//过线数
int Mid=2;
int Far=2;
int Long=2;
int place=0;//绝对位置

String Data=" "; 
char inData=' ';
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,21,20,U8X8_PIN_NONE);
void OLED_Show(void);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);//连接蓝牙
  Serial2.begin(9600);//连接openmv
  Serial3.begin(9600);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);//步进电机电平比较端
  pinMode(AdirPin,OUTPUT);
  pinMode(AstepPin,OUTPUT);
  pinMode(BdirPin,OUTPUT);
  pinMode(BstepPin,OUTPUT);
  pinMode(R,OUTPUT);digitalWrite(R,HIGH);
  pinMode(Y,OUTPUT);digitalWrite(Y,HIGH);
  pinMode(G,OUTPUT);digitalWrite(G,HIGH);
  pinMode(box,INPUT);
  pinMode(Q1,INPUT);pinMode(Q2,INPUT);
  pinMode(Q3,INPUT);pinMode(Q4,INPUT);
  pinMode(Q5,INPUT);pinMode(Q6,INPUT);
  pinMode(Q7,INPUT);pinMode(Q8,INPUT);
  pinMode(BEEP,OUTPUT);digitalWrite(BEEP,LOW);
  u8g2.begin();
  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_logisoso28_tf);
  u8g2.drawStr(0,48,"-------");
  u8g2.sendBuffer();  
}

void OpenmvRead(void)
{
  while(Serial2.read()>=0);//清除缓存
  while(Serial2.available()==0);//等待信号传输
  if(Serial2.available())//接收到openmv发来的病房号数字
  {
    inData=Serial2.read();//读取接收的字符
    if(inData=='1')Aim=1;
    if(inData=='2')Aim=2;
    if(inData=='3')Aim=3;
    if(inData=='4')Aim=4;
    if(inData=='5')Aim=5;
    if(inData=='6')Aim=6;
    if(inData=='7')Aim=7;
    if(inData=='8')Aim=8;
    inData=' ';
    while(Serial2.read()>=0);//清除缓存区数据
  }
}

/*到达数字，向open发送开始识别命令&获取openmv比对情况*/
void OpenmvZY(void)
{
  while(Serial2.read()>=0);
  Serial2.write(0x31);//发送开启指令
  while(Serial2.available()==0);//等待信号传输
  if(Serial2.available())
  {
    inData=Serial2.read();//读取第一个接收的字符
    if(inData=='1')
    {
      ZY=0; ////ZY-0-左转
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---Z---");
      u8g2.sendBuffer();
    }
    if(inData=='2')
    {
      ZY=1; ////ZY-1-右转
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---Y---");
      u8g2.sendBuffer();
    }
    if(inData=='0')
    {
      ZY=2;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---N---");
      u8g2.sendBuffer();
    }
    inData=' ';
    while(Serial2.read()>=0);//清除缓存区数据
  }
}

void BTpalce(void)//蓝牙发送地址信息（绝对）
{
  if(Far==2||Long==2)//如果没有到达远端，只在中端结束，则发送结束目的位置
  {
    if(Mid==0)
    {
      Serial1.print('3');
      place=3;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---3---");
      u8g2.sendBuffer(); 
    }
    if(Mid==1)
    {
      Serial1.print('4');
      place=4;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---4---");
      u8g2.sendBuffer(); 
    }
  }else{
    if(Far==0&&Long==0)
    {
      Serial1.print('5');
      place=5;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---5---");
      u8g2.sendBuffer(); 
    }
    if(Far==0&&Long==1)
    {
      Serial1.print('7');
      place=7;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---7---");
      u8g2.sendBuffer(); 
    }
    if(Far==1&&Long==0)
    {
      Serial1.print('6');
      place=6;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---6---");
      u8g2.sendBuffer(); 
    }
    if(Far==1&&Long==1)
    {
      Serial1.print('8');
      place=8;
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---8---");
      u8g2.sendBuffer(); 
    }
  }
}


// /*步进，给脉冲的*/
/*前进*/
void Advance(int a,int t)  
//a为前进距离，x=800时为轮子转一圈，距离约为23.56cm。
//t为一次脉冲的周期时间（us）。频率f=1/t。
{
   digitalWrite(AdirPin,LOW);//A电机逆时针为向前  ////LOW-向前  //??既然LOW-向前，那么HIGH-向后or停止？--->HIGH向前，LOW向后
   digitalWrite(BdirPin,HIGH);//B电机顺时针为向前 ////HIGH-向前
   for(int i=0;i<a;i++)
   {
     digitalWrite(AstepPin,HIGH); 
     digitalWrite(BstepPin,HIGH); 
     delayMicroseconds(t/2);
     digitalWrite(AstepPin,LOW);  
     digitalWrite(BstepPin,LOW);  
     delayMicroseconds(t/2);   
   }
}

/*后退*/
void Back(int a,int t)  //a为后退距离，x=800时为轮子转一圈，距离约为23.56cm。 t为一次脉冲的周期时间。频率f=1/t。
{
   digitalWrite(AdirPin,HIGH);//A电机顺时针为向后
   digitalWrite(BdirPin,LOW);//B电机逆时针为向后
   for(int i=0;i<a;i++)
   {
     digitalWrite(AstepPin,HIGH); 
     digitalWrite(BstepPin,HIGH);
     delayMicroseconds(t/2);
     digitalWrite(AstepPin,LOW); 
     digitalWrite(BstepPin,LOW);
     delayMicroseconds(t/2);
   }
}

/*右转*/
void Clockwise(int a,int t)  //a为相对顺时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
{
   digitalWrite(BdirPin,HIGH);//B电机顺时针为向前
   for(int i=0;i<a;i++)
   {
     digitalWrite(BstepPin,HIGH);
     delayMicroseconds(t/2);
     digitalWrite(BstepPin,LOW);
     delayMicroseconds(t/2);
   }
   while(!digitalRead(Q4)&&!digitalRead(Q5))
   {
     for(int i=0;i<10;i++)
     {
       digitalWrite(BstepPin,HIGH);
       delayMicroseconds(t/2);
       digitalWrite(BstepPin,LOW);
       delayMicroseconds(t/2);
     }
   }
   for(int i=0;i<10;i++)
   {
     digitalWrite(BstepPin,HIGH);
     delayMicroseconds(t/2);
     digitalWrite(BstepPin,LOW);
     delayMicroseconds(t/2);
   }
}

/*左转，右轮在动*/
void AnitClockwise(int a,int t)  //a为相对逆时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
{   
   digitalWrite(AdirPin,LOW);//A电机逆时针为向前  ////此处仅A-向前 --->左轮停机-左转（右轮转）xxx错误 视频上是左轮不动右轮动--->-ALOW,右轮前进
   for(int i=0;i<a;i++)
   {
     digitalWrite(AstepPin,HIGH); 
     delayMicroseconds(t/2);
     digitalWrite(AstepPin,LOW);  
     delayMicroseconds(t/2);
   }
   while(!digitalRead(Q4)&&!digitalRead(Q5))  ////白底上时左转
   {
     for(int i=0;i<10;i++)
     {
       digitalWrite(AstepPin,HIGH); 
       delayMicroseconds(t/2);
       digitalWrite(AstepPin,LOW); 
       delayMicroseconds(t/2);
     }
   }
   for(int i=0;i<10;i++)  ////?
   {
     digitalWrite(AstepPin,HIGH); 
     delayMicroseconds(t/2);
     digitalWrite(AstepPin,LOW); 
     delayMicroseconds(t/2);
   }
}


///*据注释顺时针，但是函数英文名为反顺时针-逆时针*/
/*右转循迹函数*/
void AnitClockwisePX(int a,int t)  //a为相对顺时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
{
   digitalWrite(AdirPin,HIGH);//A电机顺时针为向后 ////--->A-左轮-正转
   digitalWrite(BdirPin,HIGH);//B电机顺时针为向前 ////B？
   for(int i=0;i<a;i++) ////给DIR脉冲控制，一个高一个低，走一步
   {
     digitalWrite(AstepPin,HIGH); 
     digitalWrite(BstepPin,HIGH);
     delayMicroseconds(t/2);
     digitalWrite(AstepPin,LOW); 
     digitalWrite(BstepPin,LOW);
     delayMicroseconds(t/2);
   }
}

/*左转循迹函数*/
void ClockwisePX(int a,int t)  //a为相对逆时针自转角度，x=800时为轮子转一圈，旋转角度未知。 t为一次脉冲的周期时间（us）。频率f=1/t。
{
   digitalWrite(AdirPin,LOW);//A电机逆时针为向前
   digitalWrite(BdirPin,LOW);//B电机逆时针为向后
   for(int i=0;i<a;i++)
   {
     digitalWrite(AstepPin,HIGH); 
     digitalWrite(BstepPin,HIGH);
     delayMicroseconds(t/2);
     digitalWrite(AstepPin,LOW); 
     digitalWrite(BstepPin,LOW);
     delayMicroseconds(t/2);
   }
}

void track_Q()    ////循迹
{
  int D1=digitalRead(Q3);
  int D2=digitalRead(Q4);
  int D3=digitalRead(Q5);
  int D4=digitalRead(Q6);
  if((D1==0)&&(D2==0)&&(D3==0)&&(D4==1))  ////假设D4（最右边）-左偏=右转-左轮向前--->左偏
  {
     AnitClockwisePX(10, 3000); ////右转循迹
  }else if((D1==0)&&(D2==0)&&(D3==1)&&(D4==0))
  {
    AnitClockwisePX(10,3000);
  }else if((D1==0)&&(D2==0)&&(D3==1)&&(D4==1))
  {
    AnitClockwisePX(10,3000);
  }else if((D1==1)&&(D2==0)&&(D3==0)&&(D4==0))  ////右偏
  {
    ClockwisePX(10,3000); ////顺时针            ////左转循迹
  }else if((D1==0)&&(D2==1)&&(D3==0)&&(D4==0))
  {
    ClockwisePX(10,3000);
  }else if((D1==1)&&(D2==1)&&(D3==0)&&(D4==0))
  {
    ClockwisePX(10,3000);
  }
  Advance(50,1800); ////否则前进
}

void loop() {
  //调试
//  if(Serial1.available())//蓝牙接收器
//  {
//    Data=Serial1.readString();//读取第一个接收的字符
//    if(Data=="ON")
//    {
      //主代码------------------------------------
      OpenmvRead();//获取任务码
      OLED_Show();//显示目的地任务
      while(digitalRead(box)==0);//等待药物放下
      delay(500);


      /*以下，1号目标*/
      if(Aim==1)  //目的地编号  ////1号位置在左边
      {
        int X=1;
        while(X)
        {
          track_Q();  ////循迹的时候就前进了
          if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))  //到达第一个T字路口
          {
            X=0;
            break;
          }
        }
        Advance(Line,2000); ////过线后移动的距离
        AnitClockwise(ZJ,2000); ////左转90度-视频上看，左轮不动，右轮动
//        while(!digitalRead(Q4)||!digitalRead(Q5)){AnitClockwise(ZJ,2000);}
        X=1;
        while(X)
        {
          track_Q();//灰度传感器状态全白即到达位置------------------------------------------------ ////如此来看1-黑
          if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
          {
            Advance(30,2000);
            if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
            {
              X=0;
              break;
            }
          }
        }
        //已到达收货区1
        digitalWrite(R,LOW);//亮红灯
        digitalWrite(BEEP,HIGH);
        delay(500);
        digitalWrite(BEEP,LOW);
        BTpalce();
        while(digitalRead(box)==1); ////--->药品放上面时是高电平
        delay(500);
        digitalWrite(R,HIGH);//灭红灯
        //开始返回
        Back(100,2000); ////后退一定时间
        X=1;
        while(X)
        {
          Back(30,2000);
          if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))  ////后退直到线上
          {
            X=0;
            break;
          }
        }
        Advance(Line,2000); ////前进一段时间
        AnitClockwise(ZJ,2000);
        X=1;
        while(X)
        {
          track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
          if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
          {
            Advance(30,2000);
            if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
            {
              X=0;
              break;
            }
          }
        }
        digitalWrite(G,LOW);//亮绿灯
        digitalWrite(BEEP,HIGH);
        delay(500);
        digitalWrite(BEEP,LOW);
        while(1);
      } 
      /*以上，1号目标完成*/


      if(Aim==2)
      {
        int X=1;
        while(X)
        {
          track_Q();
          if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
          {
            X=0;
            break;
          }
        }
        Advance(Line,2000);
        Clockwise(ZJ,2000);
//        while(!digitalRead(Q4)||!digitalRead(Q5)){Clockwise(ZJ,2000);}
        X=1;
        while(X)
        {
          track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
          if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
          {
            Advance(30,2000); ////到达全白后又向前跑了点，再判断一次全白
            if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
            {
              X=0;
              break;
            }
          }
        }
        //已到达收货区2
        digitalWrite(R,LOW);//亮红灯
        digitalWrite(BEEP,HIGH);
        delay(500);
        digitalWrite(BEEP,LOW);
        BTpalce();
        while(digitalRead(box)==1);
        delay(500);
        digitalWrite(R,HIGH);//灭红灯
        //开始返回
        Back(100,2000);
        X=1;
        while(X)
        {
          Back(30,2000);
          if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))
          {
            X=0;
            break;
          }
        }
        Advance(Line,2000);
        Clockwise(ZJ,2000);
        X=1;
        while(X)
        {
          track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
          if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
          {
            Advance(30,2000);
            if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
            {
              X=0;
              break;
            }
          }
        }
        digitalWrite(G,LOW);//亮绿灯
        digitalWrite(BEEP,HIGH);
        delay(500);
        digitalWrite(BEEP,LOW);
        while(1);
      }
      if(Aim>2)//中部或远端病房
      {
        int X=1;
        while(X)
        {
          track_Q();
          if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
          {
            X=0;
            break;
          }
        }
        int i=53;//近端到中端---------------------------------------------------------  ////从近端开始累计-到达中端是累计循迹,到达中端的十字路口前开始识别
        while(i--){track_Q();}       
        OpenmvZY();//读取左右
        switch(ZY)
        {
          case 0:{//在左边  ////ZY-0-左
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))  ////前进到十字路口
              {
                X=0;
                break;
              }
            }
            Advance(Line,2000);
            AnitClockwise(ZJ,2000);//逆时针左转 ////左转
            Mid=0;
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            //已到达收货区3
            digitalWrite(R,LOW);//亮红灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            BTpalce();
            while(digitalRead(box)==1);
            delay(500);
            Serial1.print('O');
            digitalWrite(R,HIGH);//灭红灯
            //开始返回
            Back(100,2000);
            X=1;
            while(X)
            {
              Back(30,2000);
              if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))
              {
                X=0;
                break;
              }
            }
            Advance(Line,2000);
            AnitClockwise(ZJ,2000);
            Serial1.print('K');
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            digitalWrite(G,LOW);//亮绿灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            while(1);
            break;
          }
          case 1:{//在右边
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line,2000);
            Clockwise(ZJ,2000);//顺时针左转
            Mid=1;
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            //已到达收货区4
            digitalWrite(R,LOW);//亮红灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            BTpalce();
            while(digitalRead(box)==1);
            delay(500);
            Serial1.print('O');
            digitalWrite(R,HIGH);//灭红灯
            //开始返回
            Back(100,2000);
            X=1;
            while(X)
            {
              Back(30,2000);
              if(digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6))
              {
                X=0;
                break;
              }
            }
            Advance(Line,2000);
            Clockwise(ZJ,2000);
            Serial1.print('K');
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            digitalWrite(G,LOW);//亮绿灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            while(1);
            break;
          }
          case 2:break;
        }


        
//基础任务3=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        
        
        
        X=1;
        while(X)
        {
          track_Q();
          if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))  ////到达十字路口
          {
            X=0;
            break;
          }
        }
        i=53;//中端到远端-----------------------------------------------------  ////从中端开始累计-到远端
        while(i--){track_Q();}
        //到达4个数字前
        OpenmvZY();//读取左右
        switch(ZY)
        {

/*以下，为完成远端的第一个弯*/
          case 0:{//在左边  ////ZY-0-左
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            AnitClockwise(ZJ,2000);//逆时针左转
            Far=0;//记录左转
            break;
          }
          case 1:{//在右边  ////ZY-1-有
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            Clockwise(ZJ,2000);//顺时针左转
            Far=1;//记录右转  ////记录右转，错误时原路返回使用
            break;
          }
/*以上，为完成远端的第一个弯*/



/*以下，为什么？*///走错
          case 2:{//随机转一个方向：右  ////既然远端第一个弯已经转完了，为什么要进行ZY=2无意义的操作？
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            Clockwise(ZJ,2000);//顺时针
            Far=1;//记录右转
            break;
          }
        }
/*以上，为什么？*/
        
   //==============================================================================
        
        i=34;//远端到分支左、右
        while(i--){track_Q();}
        OpenmvZY();//读取左右
        switch(ZY)
        {
          case 0:{//在左边
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            AnitClockwise(ZJ,2000);//逆时针左转
            Long=0;
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            //已到达收货区5、8
            digitalWrite(R,LOW);//亮红灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            BTpalce();
            while(digitalRead(box)==1);
            delay(500);
            digitalWrite(R,HIGH);//灭红灯
            //开始返回（特殊T形）
            Back(100,2000);
            X=1;
            while(X)
            {
              Back(30,2000);
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
              {
                X=0;
                break;
              }
            }
            Advance(Line,2000);
            AnitClockwise(ZJ,2000);
            //经过第二个T形
            if(Far==0)
            {
              X=1;
              while(X)
              {
                track_Q();
                if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
              Advance(Line,2000);
              Clockwise(ZJ,2000);
            }
            if(Far==1)
            {
              X=1;
              while(X)
              {
                track_Q();
                if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
                {
                  X=0;
                  break;
                }
              }
              Advance(Line,2000);
              AnitClockwise(ZJ,2000);
            }
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                Serial1.print('K');//全黑是发送指令
              }
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            digitalWrite(G,LOW);//亮绿灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            while(1);
            break;
          }
          case 1:{//在右边
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            Clockwise(ZJ,2000);//顺时针右转 ////远端左-右
            Long=1;
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            //已到达收货区6、7
            digitalWrite(R,LOW);//亮红灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            BTpalce();
            while(digitalRead(box)==1);
            delay(500);
            digitalWrite(R,HIGH);//灭红灯
            //开始返回（特殊T形）
            Back(100,2000);
            X=1;
            while(X)
            {
              Back(30,2000);
              if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line,2000);
            Clockwise(ZJ,2000);
            //经过第二个T形
            if(Far==0)  ////远端第一个路口-左转
            {
              X=1;
              while(X)
              {
                track_Q();
                if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
              Advance(Line,2000);
              Clockwise(ZJ,2000);
            }
            if(Far==1)
            {
              X=1;
              while(X)
              {
                track_Q();
                if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
                {
                  X=0;
                  break;
                }
              }
              Advance(Line,2000);
              AnitClockwise(ZJ,2000);
            }
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                Serial1.print('K');//全黑是发送指令
              }
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            digitalWrite(G,LOW);//亮绿灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            while(1);
            break;
          }
          case 2:{//走错了
            Far=0;//记录右转
            X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            AnitClockwise(ZJ,2000);
            X=1;
            while(X)
            {
              Back(30,2000);
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            AnitClockwise(ZJ,2000);
            X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
              {
                X=0;
                break;
              }
            }
//走错了============================================================================
            i=51;
            while(i--){track_Q();}
            OpenmvZY();//读取左右
            while(ZY==3)
            {
              track_Q();
              delay(100);
              OpenmvZY();
            }
        switch(ZY)
            {
              case 0:
              {//在左边
                int X=1;
                while(X)
                {
                  track_Q();
                  if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
                  {
                    X=0;
                    break;
                  }
                }
                Advance(Line2,2000);
                AnitClockwise(ZJ,2000);//逆时针左转
                Long=0;
                X=1;
                while(X)
                {
                  track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
                  if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                  {
                    Advance(30,2000);
                    if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                    {
                      X=0;
                      break;
                    }
                  }
                }
                //已到达收货区5、8
                digitalWrite(R,LOW);//亮红灯
                digitalWrite(BEEP,HIGH);
                delay(500);
                digitalWrite(BEEP,LOW);
                BTpalce();
                while(digitalRead(box)==1);
                delay(500);
                digitalWrite(R,HIGH);//灭红灯
                //开始返回（特殊T形）
                Back(100,2000);
                X=1;
                while(X)
                {
                  Back(30,2000);
                  if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
                  {
                    X=0;
                    break;
                  }
                }
                Advance(Line,2000);
                AnitClockwise(ZJ,2000);
                //经过第二个T形
                if(Far==0)
                {
                  X=1;
                  while(X)
                  {
                    track_Q();
                    if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
                    {
                      X=0;
                      break;
                    }
                  }
                  Advance(Line,2000);
                  Clockwise(ZJ,2000);
                }
                if(Far==1)
                {
                  X=1;
                  while(X)
                  {
                    track_Q();
                    if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
                    {
                      X=0;
                      break;
                    }
                  }
                  Advance(Line,2000);
                  AnitClockwise(ZJ,2000);
                }
                X=1;
                while(X)
                {
                  track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
                  if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
                  {
                    Serial1.print('K');//全黑是发送指令
                  }
                  if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                  {
                    Advance(30,2000);
                    if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                    {
                      X=0;
                      break;
                    }
                  }
                }
                digitalWrite(G,LOW);//亮绿灯
                digitalWrite(BEEP,HIGH);
                delay(500);
                digitalWrite(BEEP,LOW);
                while(1);
                break;
          }  
          case 1:{//在右边
            int X=1;
            while(X)
            {
              track_Q();
              if(digitalRead(Q1)&&digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line2,2000);
            Clockwise(ZJ,2000);//顺时针右转
            Long=1;
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            //已到达收货区6、7
            digitalWrite(R,LOW);//亮红灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            BTpalce();
            while(digitalRead(box)==1);
            delay(500);
            digitalWrite(R,HIGH);//灭红灯
            //开始返回（特殊T形）
            Back(100,2000);
            X=1;
            while(X)
            {
              Back(30,2000);
              if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                X=0;
                break;
              }
            }
            Advance(Line,2000);
            Clockwise(ZJ,2000);
            //经过第二个T形
            if(Far==0)
            {
              X=1;
              while(X)
              {
                track_Q();
                if(digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
              Advance(Line,2000);
              Clockwise(ZJ,2000);
            }
            if(Far==1)
            {
              X=1;
              while(X)
              {
                track_Q();
                if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4))
                {
                  X=0;
                  break;
                }
              }
              Advance(Line,2000);
              AnitClockwise(ZJ,2000);
            }
            X=1;
            while(X)
            {
              track_Q();//灰度传感器状态全白即到达位置------------------------------------------------
              if(digitalRead(Q1)&&digitalRead(Q2)&&digitalRead(Q3)&&digitalRead(Q4)&&digitalRead(Q5)&&digitalRead(Q6)&&digitalRead(Q7)&&digitalRead(Q8))
              {
                Serial1.print('K');//全黑是发送指令
              }
              if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
              {
                Advance(30,2000);
                if(!digitalRead(Q1)&&!digitalRead(Q2)&&!digitalRead(Q3)&&!digitalRead(Q4)&&!digitalRead(Q5)&&!digitalRead(Q6)&&!digitalRead(Q7)&&!digitalRead(Q8))
                {
                  X=0;
                  break;
                }
              }
            }
            digitalWrite(G,LOW);//亮绿灯
            digitalWrite(BEEP,HIGH);
            delay(500);
            digitalWrite(BEEP,LOW);
            while(1);
            break;
          }
          }
          }
        }
        while(1);
      }
//    }//结束
//    if(Data=="A"){OpenmvRead();OLED_Show();}
//    if(Data=="B"){OpenmvZY();Serial1.println(ZY);}
//    if(Data=="C"){OpenmvZY4();Serial1.println(ZY);}
//    if(Data=="TQ"){while(1){track_Q();}}
//    if(Data=="place")BTpalce();
//    if(Data=="Qian")Advance(200,2000);
//    if(Data=="Hou")Back(200,2000);
//    if(Data=="QianShort")Advance(100,2000);
//    if(Data=="HouShort")Back(100,2000);
//    if(Data=="QianLong")Advance(400,1000);
//    if(Data=="HouLong")Back(400,1000);
//    if(Data=="Shun")AnitClockwisePX(1120,1000);
//    if(Data=="Ni")ClockwisePX(1120,1000);
//    if(Data=="readQ")
//    {
//      Serial1.print(digitalRead(Q1));Serial1.print(digitalRead(Q2));
//      Serial1.print(digitalRead(Q3));Serial1.print(digitalRead(Q4));
//      Serial1.print(digitalRead(Q5));Serial1.print(digitalRead(Q6));
//      Serial1.print(digitalRead(Q7));Serial1.println(digitalRead(Q8));
//    }
//    Data=" ";
//  }
}

/*调试，OLED显示目标位置*/
void OLED_Show(void)
{
  switch(Aim)
  {
    case 1:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---1---");
      u8g2.sendBuffer(); 
      break;
    }
    case 2:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---2---");
      u8g2.sendBuffer(); 
      break;
    }
    case 3:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---3---");
      u8g2.sendBuffer(); 
      break;
    }
    case 4:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---4---");
      u8g2.sendBuffer(); 
      break;
    }
    case 5:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---5---");
      u8g2.sendBuffer(); 
      break;
    }
    case 6:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---6---");
      u8g2.sendBuffer(); 
      break;
    }
    case 7:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---7---");
      u8g2.sendBuffer(); 
      break;
    }
    case 8:{
      u8g2.clearBuffer();  
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,48,"---8---");
      u8g2.sendBuffer(); 
      break;
    }
  }
}


