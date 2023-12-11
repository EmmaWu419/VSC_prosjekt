#include <Wire.h>
#include <Zumo32U4.h>
#include "PololuOLEDHelpers.h"
#include <LSM6.h>


Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;


// variables to buttonHoldTime
unsigned long startPress;
unsigned long endPress;
unsigned long holdTime;

//buttonstate
bool buttonStateA;
bool previousButtonStateA = false;

// variables to buttonChargeTime
unsigned long interval3 = 200;
unsigned long interval1 = 5000;

int chargeTime;
int price;

// variables to pricelist
int bankBalance = 1000;
int priceList[6] = {fullCharge, upToCharge, chargeUntilStop, chargeUntilTime, batteryService, batteryReplacement}
int listCount = 6;

//variables to buttonchargepercent
int chargePercent;

//variables to switch case
int buttonCountB = 0;

  


long buttonHoldTime(){
  if(buttonA.isPressed()){
    startPress = millis();
  }
  else {
    endPress = millis();
    holdTime = endPress - startPress;
  }
  return holdTime;
}

int buttonTimeCharge(long holdTime){
  if (holdTime >= interval1) {
    chargeTime = 450000;
    price = 75;
  }
  if(holdTime > interval3 && holdTime < interval1) {
    chargeTime = 300000;
    price = 50;
  }
  if (holdTime <= interval3 && holdTime > 0) {
    chargeTime = 150000;
    price = 25;
  }
  return chargeTime, price;
}

int buttonChargePercent(int holdTime){
  if (holdTime >= interval1) {
    chargePercent = 30; 
  }
  if(holdTime > interval3 && holdTime < interval1 ) {
    chargePercent = 50;
  }
  if(holdTime <= interval3 && holdTime > 0) {
    chargePercent = 70;
  } 
  return chargePercent;
}



void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  buttonStateA = buttonA.isPressed();

  if(buttonStateA != previousButtonStateA) {
    Serial.println("hi");
    buttonHoldTime();
  }
  previousButtonStateA = buttonStateA;

  buttonTimeCharge(holdTime);

  buutonPercentCharge(holdTime);
  
  // la oss ha det slik at vi teller hvor mange ganger vi trykker og antall nummer vi trykker er den casen vi går inn i
  switch (priceList){
    case fullCharge:
    price = 100;
    while(batteryLevel < 100){
      batteryLevel = batteryLevel + 1;
      delay(5000);
    }
    chargingCycles = chargingCycles + 1;
    break;


    case upToCharge:
    buttonPercentCharge();
    price = 0;
    while(batteryLevel < charge) {
      price = price + 2;
  }
  }
  // put your main code here, to run repeatedly:
}
