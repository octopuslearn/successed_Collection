/* Author = helscream (Omer Ikram ul Haq)
Last edit date = 2014-06-22
Website: http://hobbylogs.me.pn/?p=17
Location: Pakistan
Ver: 0.1 beta --- Start
Ver: 0.2 beta --- Debug feature included
*/

#include <Wire.h>
#include "compass.h"
#include <Arduino.h>
#define Task_t 10          // Task Time in milli seconds

int dt=0;
unsigned long t;
// Main code -----------------------------------------------------------------
void setup(){
  Serial.begin(9600);
  // Serial.print("Setting up I2C ........\n");
  Wire.begin();
  compass_x_offset = -765.19;
  compass_y_offset = 338.37;
  compass_z_offset = 321.88;
  compass_x_gainError = 1.86;
  compass_y_gainError = 0.94;
  compass_z_gainError = 0.89;
  
  
  
  compass_init(2);
  compass_debug = 1;
    // compass_offset_calibration(3);


}

// Main loop 
// Main loop -----------------------------------------------------------------
void loop(){
  compass_scalled_reading();
  
  Serial.print("x = ");
  Serial.print(compass_x_scalled);
  Serial.print("y = ");
  Serial.print(compass_y_scalled);
  Serial.print("z = ");
  Serial.print(compass_z_scalled);
  

  compass_heading();
  Serial.print ("Heading angle = ");
  Serial.print (bearing);
  Serial.println(" Degree");
  
  delay(100);
  
  
}











