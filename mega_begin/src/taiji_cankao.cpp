// void contal_pulse(char servoName,int last_pos,int toPos,int servoDelay)
// {
//     switch(servoName){
//     case 'x':
//       if(toPos >= 500 && toPos <= 2500){
//         if(toPos>last_pos)
//         {
//           for(int i=last_pos;i<toPos;i+=10)
//           {
//             myservo_x.writeMicroseconds(i);
//             delay(servoDelay);
//           }
//         }
//         else if(toPos<last_pos)
//         {
//           for(int i=last_pos;i<toPos;i-=10)
//           {
//             myservo_x.writeMicroseconds(i);
//             delay(servoDelay);
//           }
//         }


// /*以下，显示*/
//         u8g2.firstPage();
//         do
//         {
//           u8g2.setFont(u8g_font_7x14); // 设置字体
//           u8g2.drawStr(0, 10, "contal_pulse");
//           u8g2.drawStr(0,25, "cp_x");
//           u8g2.setCursor(sizeof("cp_x: ") * 6, 25);
//           u8g2.print(toPos);
//         } while (u8g2.nextPage());
// /*以上，显示*/
//         break;
//       } else {
//         Serial.println("+Warning: x_Base舵机角度超范围!");
//         return;
//       }

//     case 'y':
//       if(toPos >= 500 && toPos <= 2500){ 
//         if(toPos>last_pos)
//         {
//           for(int i=last_pos;i<toPos;i+=10)
//           {
//             myservo_y.writeMicroseconds(i);
//             delay(servoDelay);
//           }
//         }
//         else if(toPos<last_pos)
//         {
//           for(int i=last_pos;i<toPos;i-=10)
//           {
//             myservo_y.writeMicroseconds(i);
//             delay(servoDelay);
//           }
//         }   
// /*以下，显示*/
//         u8g2.firstPage();
//         do
//         {
//           u8g2.setFont(u8g_font_7x14); // 设置字体
//           u8g2.drawStr(0, 10, "contrl_reading");
//           u8g2.drawStr(0,25, "cr_y");
//           u8g2.setCursor(sizeof("cr_y: ") * 6, 25);
//           u8g2.print(toPos);
//         } while (u8g2.nextPage());
// /*以上，显示*/
//         break;
//       } else {
//         Serial.println("+Warning: y_Claw舵机角度超范围!");
//         return;        
//       }
//     }
// }