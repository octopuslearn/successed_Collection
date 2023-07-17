#include <Arduino.h>
#include "avr8-stub.h"
// #include "app_api.h"/*不需要这一行*/
// put function declarations here:
int myFunction(int, int);

void setup() {
  debug_init();
  pinMode(13,OUTPUT);
  Serial3.begin(9600);//串口3不影响
  Serial1.begin(9600);//串口1不影响
  // Serial.begin(9600);//串口0影响
  // put your setup code here, to run once:
}

void loop() {
  digitalWrite(13,HIGH);
  Serial3.println("sss!!!");
   Serial1.println("sss!!!");
  // Serial.println("aaa!!!");
  delay(1000);
  digitalWrite(13,LOW);
  Serial3.println("sss###");
  // Serial.println("aaa!!!");
  delay(1000);
  // put your main code here, to run repeatedly:
}