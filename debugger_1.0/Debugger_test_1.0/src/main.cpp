#include <Arduino.h>
#include "avr8-stub.h"
// #include "app_api.h"/*不需要这一行*/
// put function declarations here:
void clock_ms(unsigned int ms);

void setup() {
  debug_init();
  pinMode(13,OUTPUT);
  Serial3.begin(9600);//串口3不影响
  // Serial1.begin(9600);//串口1不影响
  // Serial.begin(9600);//串口0影响
  // put your setup code here, to run once:
}

void loop() {
  Serial3.println("start!!!");
  clock_ms(30);
  Serial3.println("sss###");
  digitalWrite(13,HIGH);
  delay(500);
  // put your main code here, to run repeatedly:
}

void clock_ms(unsigned int ms)
{
  unsigned long last_time = millis();
  digitalWrite(13,LOW);
  while((millis()-last_time)<ms);
}

