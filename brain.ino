#include "Timer.h"
#include "LiquidCrystal.h"
LiquidCrystal lcd(3, 4, 6, 7, 8, 9);
int button;
int currPosition = 0;
bool movingRight = true;
int cnt = 0;
int hcnt = 0;
   unsigned long elapsedTime = 0;
  const unsigned long timerPeriod = 10;
char pin[] = {A0,A1,A2,A3,A4,A5,A6};
void displayNumTo7Seg(unsigned int targetNum, int digitPin) {
char gSegPins[] = {26,28,30,32,34,36,38};
    unsigned char encodeInt[10] = {
        // 0     1     2     3     4     5     6     7     8     9
        0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67,
    };
    digitalWrite(digitPin, 1);
    for (int k = 0; k < 7; ++k) {
        digitalWrite(gSegPins[k], encodeInt[targetNum] & (1 << k));
    }
    digitalWrite(digitPin, 0);
}
enum currState0 {first,second} currState0 = first;
void display() { 
 switch(currState0) { 
 case first:
 currState0 = second;
 break;
 
 case second:
 currState0 = first;
 break;
 }

switch(currState0) { 
  case first:
  digitalWrite(24,HIGH);
  digitalWrite(22,LOW);
  displayNumTo7Seg((cnt%10),22);
  break;

  case second:
  digitalWrite(22,HIGH);
  digitalWrite(24,LOW);
  displayNumTo7Seg((cnt/10),24);
  break;

}
}
enum currState1 {wait,inc,dec, pauseLight} currState1 = wait;
void light() { 
   switch(currState1) { 
      case wait:
      if(button == 0){
        currState1 = pauseLight;
      }
      else if(movingRight){
        currState1 = inc;
      }
      else{
        currState1 = dec;
      }
      break;

      case inc:
      if(button == 0){
        currState1 = pauseLight;
      }
      else if(movingRight){
        currState1 = inc;
      }
      else{
        currState1 = wait;
      }
      break;

      case dec:
      if(button == 0){
        currState1 = pauseLight;
      }
      else if(!movingRight){
        currState1 = dec;
      }
      else{
        currState1 = wait;
      }
      break;

      case pauseLight:
      if(button == 0){
        currState1 = pauseLight;
      }
      else{
        currState1 = wait;
      }
      break;
   }
   switch(currState1) { 

      case inc:
      ++currPosition;
      if(currPosition == 6){
        movingRight = false;
      }
      digitalWrite(pin[currPosition], HIGH);
      digitalWrite(pin[currPosition]-1, LOW);
      break;

      case dec:
      --currPosition;
      if(currPosition == 0){
      movingRight = true;
      }
      digitalWrite(pin[currPosition], HIGH);
      digitalWrite(pin[currPosition]+1, LOW);
      break;
      case pauseLight:
      break;
}
}
enum SM2_STATES {start, pressRelease, confirm} currState2 = start;
void counter() { 
   switch(currState2) { 
      case start:
      if(button == 1){
        currState2 = start;
      }
      else if(button == 0){
        currState2 = pressRelease;
      }
      break;

      case pressRelease:
      if(button == 1){
        currState2 = confirm;
      }
      else if(button == 0){
        currState2 = pressRelease;
      }
      break;

      case confirm:
      currState2 = start;
      break;
   }
   switch(currState2) { //actions
   case start:
   break;

   case pressRelease:
   break;

   case confirm:
      if(currPosition == 3){
        ++cnt;
        if(cnt >= hcnt){
          hcnt = cnt;
        }
      }
      else{
        cnt = 0;

      }
   break;
}

}



void setup() {
Serial.begin(9600);

pinMode(2, INPUT); //button
pinMode(A0, OUTPUT);//left light
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);//red light
pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);
pinMode(A6, OUTPUT);//right ligjt

pinMode(22, OUTPUT);//1
pinMode(24, OUTPUT);//2
pinMode(26, OUTPUT);//a
pinMode(28, OUTPUT);
pinMode(30, OUTPUT);
pinMode(32, OUTPUT);
pinMode(34, OUTPUT);
pinMode(36, OUTPUT);
pinMode(38, OUTPUT);//g


}

void loop(){
  
  TimerSet(10);
  TimerOn();
  lcd.setCursor(0,0);
  lcd.print("HIGHSCORE:");
  lcd.setCursor(10,0);
  lcd.print(hcnt);
  Serial.print(cnt);
  button = digitalRead(2);
  while(!TimerFlag){}
  TimerFlag = 0;  
  display();
  if(elapsedTime > 60){
    light();
    counter();
    elapsedTime = 0;
  }
  elapsedTime = elapsedTime + timerPeriod;

}
