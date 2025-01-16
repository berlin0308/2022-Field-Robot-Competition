#include "arduino_util.h"
#include <LiquidCrystal_PCF8574.h>
#include <Arduino.h>


UTIL::UTIL(){
}

UTIL::~UTIL(){}

void UTIL::LCD_display_STAGE_STATE(LiquidCrystal_PCF8574 LCD,char stage, char state){
    
    //LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("STAGE:     ");
    LCD.setCursor(0, 1);
    LCD.print("STATE:     ");
    
    LCD.setCursor(6, 0);
    LCD.print(STAGE_char2Str(stage));
    LCD.setCursor(6, 1);
    LCD.print(STATE_char2Str(state));

}


String UTIL::STAGE_char2Str(char stage){
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


String UTIL::STATE_char2Str(char state){
    switch(state){
        case '0':
            return "TRACK         ";
        case '1':
            return "TRACK_R       ";
        case '2':
            return "SLOW          ";
        case '3':
            return "DRIFT         ";
        case '4':
            return "TURN          ";
        case '5':
            return "U_TURN        ";
        case '6':
            return "FIND          ";
        case '7':
            return "WATER_CASE    ";
        case '8':
            return "TRACK_U       ";
        case '9':
            return "SWITCH        ";
    }
}

void UTIL::LED_display_STAGE_STATE(int displayerLED_pin[],char stage,char state){

    switch (state)
    {
    case '0': // TRACK
        igniteLED(displayerLED_pin,'3'); // green
        break;
    case '1': // TRACK_R
        igniteLED(displayerLED_pin,'5');// cyan
        break;
    case '2': // SLOW
        igniteLED(displayerLED_pin,'2'); // red
        break;
    case '3': // DRIFT
        igniteLED(displayerLED_pin,'6'); // yellow
        break;
    case '4': // TURN
        igniteLED(displayerLED_pin,'4'); // blue 
        break;
    case '5': // U_TURN
        igniteLED(displayerLED_pin,'4'); // blue 
        break;
    case '6': // FIND_SIGN, FIND_FRUIT, FIND_CASE
        igniteLED(displayerLED_pin,'0');  
        break;
    case '7': // WATER_CASE
        igniteLED(displayerLED_pin,'7'); // megenta
        break;
    case '8': // TRACK_U
        igniteLED(displayerLED_pin,'5'); // cyan 
        break;
    case '9': // SWITCH
        igniteLED(displayerLED_pin,'1'); // white 
        break;   
    
    default:
        break;
    }

}


void UTIL::igniteLED(int LEDpins[] ,char choice){
  int R = LEDpins[0];  
  int G = LEDpins[1];  
  int B = LEDpins[2];
  
  switch(choice){
    case '0':
      light(R,G,B,0,0,0);  // light off
      break;
      
    case '1':
      light(R,G,B,255,255,255);  // white
      break;
 
    case '2':
      light(R,G,B,255,0,0);  // Red
      break;

    case '3':
      light(R,G,B,0,255,0);  // Green
      break;
     
    case '4':
      light(R,G,B,0,0,255);  // Blue
      break;
    
    case '5':
      light(R,G,B,0,255,255);  // Cyan
      break;
      
    case '6':
      light(R,G,B,255,255,0);  // Yellow
      break;
      
    case '7':
      light(R,G,B,255,0,255);  // Megenta
      break;
   }
}

void UTIL::light(int R,int G,int B,int r,int g,int b){
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  
    if(r==0){
      digitalWrite(R,LOW);
    }
    else{
      digitalWrite(R,HIGH);
    }
    if(g==0){
      digitalWrite(G,LOW);
    }
    else{
      digitalWrite(G,HIGH);
    }
    if(b==0){
      digitalWrite(B,LOW);
    }
    else{
      digitalWrite(B,HIGH);
    }
    
  }

void UTIL::resetLED(int displayerLED_pin[],int frontCamLED_pin[],int sideCamLED_pin[]){
     igniteLED(displayerLED_pin,'0');
     igniteLED(frontCamLED_pin,'0');
     igniteLED(sideCamLED_pin,'0');
}

void UTIL::STAGE_change_buttons(int stage_button_pin[]){

   if(digitalRead(stage_button_pin[0])==HIGH | digitalRead(stage_button_pin[1])==HIGH | digitalRead(stage_button_pin[2])==HIGH | digitalRead(stage_button_pin[3])==HIGH | 
             digitalRead(stage_button_pin[4]==HIGH) ){
       digitalWrite(13,HIGH);
   }else{
       digitalWrite(13,LOW);
   }




     if(digitalRead(stage_button_pin[0])==HIGH){ // STOP
      digitalWrite(13,HIGH);
      delay(200);
      if(digitalRead(stage_button_pin[0])==HIGH){
          Serial.println("D change STAGE");  
          Serial.println("P10xx");          
      }
      digitalWrite(13,LOW);
    } 
    
   if(digitalRead(stage_button_pin[1])==HIGH){ // N1
      delay(200);
      if(digitalRead(stage_button_pin[1])==HIGH){
          Serial.println("D change STAGE");  
          Serial.println("P11xx");
      }
    }

     if(digitalRead(stage_button_pin[2])==HIGH){ // N2
      delay(200);
      if(digitalRead(stage_button_pin[2])==HIGH){
          Serial.println("D change STAGE");  
          Serial.println("P12xx");
          digitalWrite(13,HIGH);
      }
    } 

   if(digitalRead(stage_button_pin[3])==HIGH){ // N3
      delay(200);
      if(digitalRead(stage_button_pin[3])==HIGH){
          Serial.println("D change STAGE");  
          Serial.println("P13xx");
      }
    }

     if(digitalRead(stage_button_pin[4])==HIGH){ // T1
      delay(200);
      if(digitalRead(stage_button_pin[4])==HIGH){
          Serial.println("D change STAGE:"); 
          Serial.println("P14xx");
          digitalWrite(13,HIGH);
      }
    } 
    
}
