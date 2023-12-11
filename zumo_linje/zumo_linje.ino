#include <Wire.h>
#include <Zumo32U4.h>
#include "PololuOLEDHelpers.h"
#include <LSM6.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Encoders encoders;
Zumo32U4OLED display;
Zumo32U4Motors motors; 

int firekantEncoderTicks;
int encoderValue = 0;
int previousEncoderValue = 0;

float distance = 0.0;
float totalDistance = 0.0;

const float pi = 3.14159;
const float diameter = 0.039; // 39mm = 0.039 m 3.9 cm
const float omkrets = pi * diameter;
const float counts_per_revolution = 75.81 * 12;

int encoderTicks;
int turns = 0;

void setup() {
  Serial.begin(9600);
  buttonA.waitForButton();
  delay(1000);
  // put your setup code here, to run once:

}

float calculateDistance() {
  encoderTicks = (encoders.getCountsAndResetLeft() + encoders.getCountsAndResetRight()) / 2;
  distance = (encoderTicks / counts_per_revolution) * omkrets;
  totalDistance = totalDistance + distance;
  return totalDistance;
}

void firekant(float totalDistance) {
  float previousDistance = 0.0;
  float currentDistance = totalDistance;
  motors.setSpeeds(100,100);
  display.clear();
  display.print("distance");
  display.gotoXY(0,1);
  display.print(totalDistance);

  if (currentDistance - previousDistance >= 0.2 && turns == 0){
    motors.setSpeeds(0,0);
    turns = 1;
    firekantEncoderTicks = encoders.getCountsAndResetRight();
    encoderValue = encoderValue + firekantEncoderTicks;
    if(encoderValue < 227) {
      motors.setSpeeds(-80,80);
    }
    else{
      motors.setSpeeds(0,0);
    }
  }
}
 
void loop() {
  //display.clear();
  //display.print(F("distance"));
  //display.gotoXY(0,1);
  //display.print(totalDistance);
  calculateDistance();
  firekant(totalDistance);

  // put your main code here, to run repeatedly:

}
