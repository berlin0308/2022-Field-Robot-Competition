/* serial unavailable */
/* commandStr: Receiver/STAGE/STATE/pwmL/pwmR/frontCamLED/sideCamLED/StepperMotor/Pump */

// include all the libraries
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <Servo.h>
#include <Unistep2.h>
#include "arduino_util.h"

Servo myservo1;
Servo myservo0;
Unistep2 stepper1(38, 39, 40, 41, 4096, 1000);// IN1, IN2, IN3, IN4, 總step數, 每步的延遲(in micros)
Unistep2 stepper2(42, 43, 44, 45, 4096, 1000);// IN1, IN2, IN3, IN4, 總step數, 每步的延遲(in micros)

int stepper_front = 0;
int stepper_back = 0;
int grab = 0;
char go_grab;

UTIL U;
LiquidCrystal_PCF8574 lcd(0x27);

// pins
int displayerLED_pin[3] = {29,30,31}; 
int stage_button_pin[8] = {A2,A3,A4,A5,A6,A7,A8,A9}; // STOP,N1,N2,N3,T1,T2,T3,U
int ultra_trig_pin[2] = {18,24};
int ultra_echo_pin[2] = {19,25};

int motor_Hbridge_pin[4] = {12,11,6,5};
int motor_EN_pin[4] = {7,8,9,10};

int stepper_motor_A[4] = {38,39,40,41};
int stepper_motor_B[4] = {42,43,44,45};
int pump_relay = 4;
int servo_claw = 58; // A4
int servo_camera = 59; // A5

// declarations
String commandStr;
char Receiver, STAGE, STATE, pwmL[4], pwmR[4], frontCamLED, sideCamLED, stepperMotor, pump, other;
int PWM_L, PWM_R;

Ultrasonic Ultra_L(ultra_trig_pin[0],ultra_echo_pin[0]);
Ultrasonic Ultra_R(ultra_trig_pin[1],ultra_echo_pin[1]);

void setup(){

    pinMode(displayerLED_pin[0],OUTPUT);
    pinMode(displayerLED_pin[1],OUTPUT);
    pinMode(displayerLED_pin[2],OUTPUT);

    pinMode(stage_button_pin[0],INPUT);
    pinMode(stage_button_pin[1],INPUT);
    pinMode(stage_button_pin[2],INPUT);
    pinMode(stage_button_pin[3],INPUT);
    pinMode(stage_button_pin[4],INPUT);
    pinMode(stage_button_pin[5],INPUT);
    pinMode(stage_button_pin[6],INPUT);
    pinMode(stage_button_pin[7],INPUT);

    pinMode(motor_EN_pin[0],OUTPUT);
    pinMode(motor_EN_pin[1],OUTPUT);
    pinMode(motor_EN_pin[2],OUTPUT);
    pinMode(motor_EN_pin[3],OUTPUT);

    digitalWrite(motor_EN_pin[0],HIGH);
    digitalWrite(motor_EN_pin[1],HIGH);
    digitalWrite(motor_EN_pin[2],HIGH);
    digitalWrite(motor_EN_pin[3],HIGH);

    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("STAGE:     ");
    lcd.setCursor(0, 1);
    lcd.print("STATE:     ");

    pinMode(A1, OUTPUT);
    myservo1.attach(A1);
    myservo1.write(5);
    
    pinMode(A0, OUTPUT);
    myservo0.attach(A0);
    myservo0.write(30);
    

}

void loop(){
   
        // STAGE-change buttons
        U.STAGE_change_buttons(stage_button_pin);
   
        commandStr = Serial.readStringUntil('e');
        //Serial.println("D Arduino received");
    
      //  while(commandStr[0] != 'A'){
      //    commandStr = Serial.readStringUntil('e');
      //  }
            
        Receiver = commandStr[0];
        if(Receiver=='A'){

            STAGE = commandStr[1];
            STATE = commandStr[2];

            pwmL[0] = commandStr[3];
            pwmL[1] = commandStr[4];
            pwmL[2] = commandStr[5];
            pwmL[3] = '\0';
            PWM_L = atoi(pwmL);

            pwmR[0] = commandStr[6];
            pwmR[1] = commandStr[7];
            pwmR[2] = commandStr[8];
            pwmR[3] = '\0';
            PWM_R = atoi(pwmR);

            frontCamLED = commandStr[9];
            sideCamLED = commandStr[10];
            stepperMotor = commandStr[11];
            pump = commandStr[12];
        }

    
    
    // LCD
    lcd.setCursor(6, 0);
    lcd.print(STAGE_char2Str(STAGE)); //STAGE_char2Str(stage));
    lcd.setCursor(6, 1);
    lcd.print(STATE_char2Str(STATE)); //STATE_char2Str(state));

    // LED
    if(sideCamLED!='R' && sideCamLED!='Y'&& sideCamLED!='B'&&sideCamLED!='L'){
        U.LED_display_STAGE_STATE(displayerLED_pin,STAGE,STATE);
    }
    
        if (STAGE=='0') // STOP
        {
            U.runMotor(0,0);
        }
        if (STAGE=='1') // N1
        {
            if(STATE=='1'){ // TRACK
                int L_read=0, R_read=0;
                for(int i=0;i<20;i++){
                    L_read += Ultra_L.read();
                    R_read += Ultra_R.read();
                    delay(10);
                }
                U.TRACK_checkDist(L_read/20,R_read/20);
                //Serial.println("D Start Tracking...");
                delay(50);
                U.STAGE_change_buttons(stage_button_pin);
            }
            if(STATE=='3'){
                
                U.LED_display_STAGE_STATE(displayerLED_pin,STAGE,STATE);
                U.runMotor(150, 150);
                delay(4000);
                U.runMotor(100, -100);
                delay(2200);
                U.runMotor(150, 150);
                delay(3200);  
                U.runMotor(100, -100);
                delay(2000);
                Serial.println("Pxx19");
                U.runMotor(0,0);
                delay(2000);
            }
            if(STATE=='9'){
                U.LED_display_STAGE_STATE(displayerLED_pin,STAGE,STATE);
                U.runMotor(0,0);
                delay(2000);
                Serial.println("P12xx");
            }
        }
        if (STAGE=='2') // N2
        {
            if(STATE=='6'){ // FORWARD
                U.runMotor(200,230);
            }
            if(STATE=='2'){ // SLOW
                U.runMotor(40,40);
            }
            if(STATE=='S'){ // SIGN
                U.runMotor(0,0);
                char Color = sideCamLED;
                if(Color=='R'){
                    U.igniteLED(displayerLED_pin,'2');
                }
                if(Color=='Y'){
                    U.igniteLED(displayerLED_pin,'6');
                }
                if(Color=='B'){
                    U.igniteLED(displayerLED_pin,'4');
                }
                if(Color=='K'){
                    U.igniteLED(displayerLED_pin,'1');
                }
            }
            if(STATE=='F'){ // FRUIT
                U.runMotor(0,0);
                myservo0.write(45);
            }
            if(STATE=='G'){ // GRAB
                while(true){   // extend, open claw
                    stepper1.run();
                    stepper2.run();
                    if(stepper1.stepsToGo() == 0 && stepper2.stepsToGo() == 0 && stepper_front == 0 ){ // 如果stepsToGo=0，表示步進馬達已轉完應走的step了
                        stepper1.move(9100);  
                        stepper2.move(9100); 
                        stepper_front++;
                    }
                    if(stepper1.stepsToGo() == 0 && stepper2.stepsToGo() == 0 && grab == 0) {
                        myservo1.write(5);
                        delay(3000);
                        myservo1.write(45);
                        delay(3000);
                        grab ++;
                    }
                    if(stepper1.stepsToGo() == 0 && stepper2.stepsToGo() == 0 && stepper_back == 0 ){ // 如果stepsToGo=0，表示步進馬達已轉完應走的step了
                        stepper1.move(-9100);
                        stepper2.move(-9100);   
                        stepper_back++;
                    }

                    if(stepper1.stepsToGo() == 0 && stepper2.stepsToGo() == 0 && stepper_back!=0)
                    {
                        break;
                    }

                }


                
                Serial.println("Pxx11");
                
            }

            if(STATE=='1'){ // TRACK
                int L_read=0, R_read=0;
                for(int i=0;i<20;i++){
                    L_read += Ultra_L.read();
                    R_read += Ultra_R.read();
                    delay(10);
                }
                U.TRACK_checkDist(L_read/20,R_read/20);
                //Serial.println("D Start Tracking...");
                delay(50);
                U.STAGE_change_buttons(stage_button_pin);
            }
            if(STATE=='D'){ // DROP

            }
        }
        if (STAGE=='3') // N3
        {
            /* code */
        }
        if (STAGE=='4') // T1
        {
            /* code */
        }
        if (STAGE=='5') // T2
        {
            /* code */
        }
        if (STAGE=='6') // T3
        {
            /* code */
        }
        if (STAGE=='7') // U
        {
            U.runMotor(PWM_L,PWM_R);
        }

}


String STAGE_char2Str(char stage){
    switch(stage){
        case '0':
            return "STOP       ";
        case '1':
            return "N1         ";
        case '2':
            return "N2         ";
        case '3':
            return "N3         ";
        case '4':
            return "T1         ";
        case '5':
            return "T2         ";
        case '6':
            return "T3         ";
        case '7':
            return "U          ";
    }
}


String STATE_char2Str(char state){
    switch(state){
        case '0':
            return "NON         ";
        case '1':
            return "TRACK       ";
        case '2':
            return "SLOW          ";
        case '3':
            return "DRIFT         ";
        case '4':
            return "TURN          ";
        case '5':
            return "U_TURN        ";
        case '6':
            return "TRACK_U       ";
        case '7':
            return "SWITCH        ";
        case 'S':
            return "Find SIGN!!!  ";
        case 'F':
            return "Find FRUIT!!!  ";
        case 'G':
            return "Grabbing...  ";
        case 'D':
            return "Dropping...  ";

    }
}

