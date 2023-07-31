// #include <util/atomic.h> // For the ATOMIC_BLOCK macro//对于 ATOMIC_BLOCK 宏

// #define ENCA 2 // YELLOW
// #define ENCB 3 // WHITE

// volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/

// void setup() {
//   Serial.begin(9600);
//   pinMode(ENCA,INPUT);
//   pinMode(ENCB,INPUT);
//   attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
// }

// void loop() {
// //读取原子块中的位置以避免潜在的
// //如果中断与此代码运行同时发生，则误读
// //参见：https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
//   // Read the position in an atomic block to avoid a potential
//   // misread if the interrupt coincides with this code running
//   // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
//   int pos = 0; 
//   ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
//     pos = posi;
//   }

//   Serial.println(pos);
// }

// void readEncoder(){
//   int b = digitalRead(ENCB);
//   if(b > 0){
//     posi++;
//   }
//   else{
//     posi--;
//   }
// }