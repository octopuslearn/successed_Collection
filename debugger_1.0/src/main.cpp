#include <Arduino.h>
#include "avr8-stub.h"
// #include "app_api.h"/*不需要这一行*/
// put function declarations here:
int myFunction(int, int);

void setup() {
  debug_init();
  pinMode(13,OUTPUT);
  // put your setup code here, to run once:
}

void loop() {
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
  // put your main code here, to run repeatedly:
}
