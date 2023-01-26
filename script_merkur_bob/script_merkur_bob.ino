#include "U8glib.h"

// inicializace OLED displeje z knihovny U8glib
U8GLIB_SSD1306_128X32 mujOled(U8G_I2C_OPT_NONE);

// Motor Codes
int S1 = 3; //M1 Speed Control 
int S2 = 11; //M2 Speed Control
int MotorL = 12;
int MotorR = 13;  
int MotorLb = 9;
int MotorRb = 8;  
int LED = 7;
int IR = 4;
int EZO = 10;

void stop() //Stop 
{ 
analogWrite(S1, 0); 
analogWrite(S2, 0); 
} 
void advance(int a,int b) //Move forward 
{ 
  digitalWrite(MotorL, HIGH);  
  digitalWrite(MotorR, HIGH);
  digitalWrite(MotorLb, LOW);
  digitalWrite(MotorRb, LOW);
  analogWrite (S1,a); //PWM Speed Control 
  analogWrite (S2,b); 
} 
void back_off (int a,int b) //Move backward 
{
  digitalWrite(MotorL, LOW); 
  digitalWrite(MotorR, LOW); 
  digitalWrite(MotorLb, LOW);
  digitalWrite(MotorRb, LOW);
  analogWrite (S1,a); //PWM Speed Control 
  analogWrite (S2,b);  
}   
void turn_L (int a, int b) //Turn Left 
{ 
  digitalWrite(MotorL, HIGH);
  digitalWrite(MotorR, LOW);
  digitalWrite(MotorLb, LOW);
  digitalWrite(MotorRb, LOW);
  analogWrite (S1,a); //PWM Speed Control 
  analogWrite (S2,b); 
} 

void turn_R (int a,int b) //Turn Right 
{ 
  digitalWrite(MotorL, LOW);
  digitalWrite(MotorR, HIGH);
  digitalWrite(MotorLb, LOW);
  digitalWrite(MotorRb, LOW);
  analogWrite (S1,a); //PWM Speed Control 
  analogWrite (S2,b); 
}   

// Pin Codes
const int TRIG_PIN = 6; // A2
const int ECHO_PIN = 5; // A1
// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

int pozice = 0;

 //R 820 ... 90st
 //R 400 ... 45st
 //L 720 .. 90st

int speedl = 255;
int speedr = 250;
int timeL = 720;
int timeR = 400;

void setup() {
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(MotorL, OUTPUT);
  pinMode(MotorLb, OUTPUT);
  pinMode(MotorR, OUTPUT);
  pinMode(MotorRb, OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(IR,INPUT);
  pinMode(EZO,OUTPUT);
  digitalWrite(LED, LOW);
  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  tone(EZO, 1000);
  delay(1000);
  noTone(EZO);
  digitalWrite(EZO, HIGH);
  String zprava = " MARS MARS";
  mujOled.firstPage();
   do {
      // vykreslení zadané zprávy od zadané pozice
      vykresliText(pozice, zprava);
    } while( mujOled.nextPage() );
  while(digitalRead(IR) == LOW){
    //go forward
    advance (speedl,speedr);
    delay(10);
  }
  digitalWrite(LED, HIGH);
}

void loop() {
  float dist = sensor();
  while(dist >= 30){
    advance (speedl,speedr);
    dist = sensor();
  }
  turn_R (speedl,speedr);
  delay(timeR);
  advance (speedl,speedr);
  delay(1840);
 
  turn_L (speedl,speedr);
  delay(timeL);
  advance (speedl,speedr);
  delay(2250);
  
  turn_L (speedl,speedr);
  delay(timeL);
  advance (speedl,speedr);
  delay(2200);

  turn_L (speedl,speedr);
  delay(timeL+10);
  advance (speedl,speedr);
  delay(2200);
  
  //turn_R (speedl,speedr);
  //delay(timeR);
  while(digitalRead(IR) == LOW){
    advance (speedl,speedr);
  }
  digitalWrite(LED, LOW);
}

float sensor(){
  // Wait at least 60ms before next measurement
  delay(30);
  //---Pin Code Start---
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed 
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  return{cm};
}

// funkce vykresliText pro výpis textu na OLED od zadané pozice
void vykresliText(int posun, String text) {
  mujOled.setFont(u8g_font_fub14);
  // nastavení výpisu od souřadnic x=0, y=25; y záleží na velikosti písma
  mujOled.setPrintPos(0, 25);
  // uložení části zprávy - od znaku posun uložíme 15 znaků
  String vypis;
  vypis = text.substring(posun, posun+15);
  // výpis uložené části zprávy na OLED displej
  mujOled.print(vypis);
}
