/*
    RemoteXY + ESP32 Motor Control & Buzzer "Hi" Melody
    Connection via WiFi Point
    Motors: IN1=12, IN2=14, IN3=27, IN4=26
    Buzzer: Pin 32
*/

#define REMOTEXY_MODE__WIFI_POINT
#include <WiFi.h>

#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

#include <RemoteXY.h>

#pragma pack(push, 1)  
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] =  
  { 255,5,0,0,0,58,0,19,0,0,0,0,31,1,106,200,1,1,5,0,
  1,40,13,24,24,0,2,31,0,1,39,88,24,24,0,2,31,0,1,67,
  115,24,24,0,2,31,0,1,14,117,24,24,0,2,31,0,1,42,140,24,
  24,0,2,31,0 };
  
struct {
  uint8_t sound; 
  uint8_t up; 
  uint8_t right; 
  uint8_t left; 
  uint8_t down; 

  uint8_t connect_flag; 
} RemoteXY;    
#pragma pack(pop)

// Motor pins
#define IN1 12  
#define IN2 14  
#define IN3 27  
#define IN4 26  

// Buzzer pin
const int buzzerPin = 32; 

void moveMotors(int leftSpeed, int rightSpeed) {
  if (leftSpeed > 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  } else if (leftSpeed < 0) {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  }

  if (rightSpeed > 0) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  } else if (rightSpeed < 0) {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  }
}

// "Hi" melody function
void playHiSound() {
  tone(buzzerPin, 880, 100); 
  RemoteXYEngine.delay(120);  
  
  tone(buzzerPin, 1319, 150); 
  RemoteXYEngine.delay(150); 

  noTone(buzzerPin);
}

void setup() 
{
  Serial.begin(115200);
  RemoteXY_Init();  
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  moveMotors(0, 0);
  Serial.println("System Ready...");
}

void loop() 
{ 
  RemoteXYEngine.handler();    
  
  // Play "Hi" sound when sound button is pressed
  if (RemoteXY.sound == 1) {
    playHiSound();
  }
  else if (RemoteXY.up == 1) {
    moveMotors(1, 1);
  } 
  else if (RemoteXY.down == 1) {
    moveMotors(-1, -1);
  } 
  else if (RemoteXY.left == 1) {
    moveMotors(-1, 1);
  } 
  else if (RemoteXY.right == 1) {
    moveMotors(1, -1);
  } 
  else {
    moveMotors(0, 0);
  }
}
