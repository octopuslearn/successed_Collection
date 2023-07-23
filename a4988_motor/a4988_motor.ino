
#define X_STEP 2
#define X_DIR  5
#define Y_STEP 3
#define Y_DIR  6
//#define X      9
//#define Y      10
#define EN       8

//const int STEP_cnt = 800;
const int STEP_cnt = 1600;
void setup() {
  // put your setup code here, to run once:
  pinMode(X_STEP,OUTPUT);
  pinMode(X_DIR,OUTPUT);
  pinMode(Y_STEP,OUTPUT);
  pinMode(Y_DIR,OUTPUT);
//  pinMode(X,OUTPUT);
//  pinMode(Y,OUTPUT);
  pinMode(EN,OUTPUT);
  digitalWrite(EN,LOW);
}

void loop() {
  digitalWrite(X_DIR,HIGH);//高电平-逆时针
  digitalWrite(Y_DIR,LOW);//高电平-逆时针
  for(int i=0;i<STEP_cnt;i++)
  {
//    digitalWrite(X_STEP,HIGH);//1/4
//    digitalWrite(Y_STEP,HIGH);
//    delayMicroseconds(300);
//    digitalWrite(X_STEP,LOW);
//    digitalWrite(Y_STEP,LOW);
//    delayMicroseconds(300);//2000us
        digitalWrite(X_STEP,HIGH);//1/8
    digitalWrite(Y_STEP,HIGH);
    delayMicroseconds(200);
    digitalWrite(X_STEP,LOW);
    digitalWrite(Y_STEP,LOW);
    delayMicroseconds(200);//2000us
  }
  //delay(1000);//1000ms
}
