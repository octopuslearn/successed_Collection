#define PPR 11//每圈11个脉冲

#define encode_A 2//A相
#define encode_B 3
#define IN1 4
#define IN2 5
#define PWM_A 6


volatile long encoderCount = 0;//脉冲数

int interval = 80;//1s进行一次测量与控制

long previousMillis = 0; long currentMilles = 0;

unsigned int rpm = 0;  int setPRM = 0;//rpm-实际测到的转速，setRPM-目标转速

int pwm = 50;//pwm初值0-255

#define pot A0//编码器



void setup() {
  Serial.begin(9600);
  pinMode(encode_A,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encode_A), ISR_Encoder, CHANGE);//外部中断，2号引脚，上升沿触发
  previousMillis = millis();
  // put your setup code here, to run once:

}

void loop() {
  setPRM = map(analogRead(pot), 0,1023, 2000,10000);//设定目标值

  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);

  currentMilles = millis();

  if(currentMilles-previousMillis>interval){
    previousMillis = currentMilles;
    rpm = (float)((encoderCount/PPR)*(1000/interval)*60);
      Serial.print("脉冲数：");Serial.print(encoderCount); Serial.print('\t');
        // 串口监视器上显示目标，Arduino 输出的 PWM， 以及测量到的RPM
      Serial.print("设定值: ");         Serial.print(setPRM);//设定值
      Serial.print('\t');
      Serial.print("当前: ");    Serial.print(rpm);//当前
      Serial.print('\t');
      Serial.print("PWM: ");    Serial.println(pwm);//PWM


      if     (rpm<setPRM) {pwm++;}
      else if(rpm>setPRM) {pwm--;}
      else                {pwm=pwm;}

      analogWrite(PWM_A, pwm);//调整马达转速

      encoderCount = 0;//累计脉冲清零，为下一秒测量做准备
  }
  // put your main code here, to run repeatedly:

}

void ISR_Encoder(){
  encoderCount++;
}
