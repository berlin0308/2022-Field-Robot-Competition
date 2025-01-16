char Receiver, STAGE, STATE, pwmL[4], pwmR[4];
int PWM_L, PWM_R;
String commandStr;

int motor_Lf = 11;
int motor_Lb = 12;
int motor_Rf = 5;
int motor_Rb = 6;

int len1 = 9;
int ren1 = 10;
int len2 = 7;
int ren2 = 8;


void setup(){
    // motor pins
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(11, OUTPUT);

    pinMode(len1,OUTPUT);
    pinMode(ren1,OUTPUT);
    pinMode(len2,OUTPUT);
    pinMode(ren2,OUTPUT);

    digitalWrite(len1,HIGH);
    digitalWrite(ren1,HIGH);
    digitalWrite(len2,HIGH);
    digitalWrite(ren2,HIGH);

    Serial.begin(9600);
}


void loop(){
  runMotor(150, 150);
  delay(4000);
  runMotor(100, -100);
  delay(2200);
  runMotor(150, 150);
  delay(3200);  
  runMotor(100, -100);
  delay(2000);
}



void runMotor(int pwm_L, int pwm_R){
  
  //Serial.println("run motor!!!");
  Serial.print("pwm L:");
  Serial.println(pwm_L);
  Serial.print("pwm R:");
  Serial.println(pwm_R);

  if(pwm_L>0){
    analogWrite(motor_Lf,pwm_L);
    analogWrite(motor_Lb,0);
  }else{
    analogWrite(motor_Lf,0);
    analogWrite(motor_Lb,-pwm_L);
  }
  
  if(pwm_R>0){
    analogWrite(motor_Rf,pwm_R);
    analogWrite(motor_Rb,0);
  }else{
    analogWrite(motor_Rf,0);
    analogWrite(motor_Rb,-pwm_R);
  }
  
}