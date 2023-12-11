#include <Wire.h>
#include <Zumo32U4.h>
#include "PololuOLEDHelpers.h"
#include <LSM6.h>

//speedometer 
//bergen hastighet og distanse

//distanse
Zumo32U4Encoders encoders;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors; 
Zumo32U4OLED display;
Zumo32U4ButtonA buttonA;
LSM6 imu;

//linesensors variabls
const int numSensors = 5;
unsigned int lineSensorsValue[numSensors];
int16_t lastError = 0;
const uint16_t maxSpeed = 400;

// time variables
unsigned long previousMillis = 0;
unsigned long previousTime = 0;
unsigned long last_time = 0;
const uint8_t periode = 100;
unsigned long previousBatteryTime = 0;


// variables concerning distance calculation
float distance = 0.0;

const float pi = 3.14159;
const float diameter = 0.039; // 39mm = 0.039 m 3.9 cm
const float omkrets = pi * diameter;


//counts per revolution
const float counts_per_revolution = 75.81 * 12; // hjulene roterer 1 gang per 75.81 motorrotasjon. Encoder teller 12 ganger per motorrotasjon


// counts
int countsLeft;
int countsRight;
int encoderTicks;

int speed = 200;

float velocity = 0.0;
float velocity_2 = 0.0;
unsigned long dt = 100;

int battery_level = 100;
float currentPlace;
int batteryPrecentage = 100;
int prevBatteryPercentage = 100;


float antallCm; // gir oss antall cm
float totalCm;
float totalM;


// det er ca 5390 ticks på 65 cm 
// bilen kan kjøre 65 cm per sekund
// som vil si 5390 / 65 gir oss 
// 1 cm = 82 ticks
// antall ticks elt på 82 for å få cm / s
// konvertere det til m per sekund 
/*
encoder counts = encoders.getCountsAndresetLeft() + encoders.getCounts() / 2 

antall cm = counts / 82
totalcm = totalcm + antall cm
total m = total cm / 100 
*/

int batteryPercentage = 100;
float chargedbatterym = 0.0;


// bank variabler
int bankBalance = 1000;
priceList [5] = {fullCharge, upToCharge, chargeUntilStop, chargeUntilTime, batteryService, BatteryReplacement};

unsigned long interval3 = 500;
unsigned long interval2 = 3000;
unsigned long interval1 = 5000;

speedReadings[60] = {};

unsigned long previousMillis = 0;
int secondsOverPercentage = 0;

int chargingCycles = 0;


void calibrateSensors() {
  display.clear();
  motors.setSpeeds(200,-200);

  for(int i = 0; i <100; i++) {
    delay(10);
    lineSensors.calibrate();
  }
  motors.setSpeeds(0,0);
}


// beregne gjennomsnittshastighet for hvert 60 sekund, makshastighet, og antall 
float calculateDistance() {
  encoderTicks = (encoders.getCountsAndResetLeft() + encoders.getCountsAndResetRight()) / 2;
  distance = (encoderTicks / counts_per_revolution) * omkrets;
  totalDistance = totalDistance + distance;
  return totalDistance;
}

float calculateSpeed() {
  float encoderCounts = (encoders.getCountsAndResetLeft() + encoders.getCountsAndResetRight()) / 2

  unsigned long time_since_driving = millis();

  antallCm = encoderCounts  / 82; // dette er farten 
  totalCm = totalCm + antallCm
  //TotalM = totalCm / 100 // gjør om fra cm til m
  return antallCm

  //float velocity = 0.0195 * maxSpeed; // v = r * w
}

void lineFollower() {
  display.clear();

  uint16_t linePosition = lineSensors.readLine(lineSensorsValue);

  //display.print(linePosition);

  uint16_t error = linePosition -2000;

  int16_t speedDifference = error /4 + 6 *(error - lastError);

  lastError = error;

  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

  leftSpeed = constrain(leftSpeed, (int16_t)-maxSpeed, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, (int16_t)-maxSpeed, (int16_t)maxSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed);
}
  

void setup() {
  lineSensors.initFiveSensors();
  Serial.begin(9600);
  display.clear();
  display.print(F("BL"));
  display.gotoXY(3,0);
  display.print("%");
  display.gotoXY(0,1);
  display.print(F("BH"));
  display.gotoXY(3,1);
  display.print("%");
  display.gotoXY(0,2);
  display.print("CC");
  display.gotoXY(3,2);
  display.print("%");
  buttonA.waitForButton();
  delay(1000);

  calibrateSensors();
  // put your setup code here, to run once:

}
/*
void batteryStatus() {
  unsigned long currentBatteryTime = millis();
  int battery_level = 100;
  int charging_cycles = 0;
  //int battery_health = 

  if (currentBatteryTime )

}
*/

/*
void drive() {
  motors.setSpeeds(speed,speed);
}
*/

void drive() {
  motors.setSpeeds(200,200);
}

float batterycalculation() {
  float batteryCalculations = 100 - totalDistance + chargedbatterym; // 100 - totalm + chargedbatterym
  batteryPercentage = round(batteryCalculations);
  if (batteryPercentage % 5 == 0 || batteryPercentage != prevBatteryPercentage) {
    prevBatteryPercentage = batteryPercentage;
  }
}

  
void loop() {
  display.clear();
  hastighetPerSekund();
  lineFollower();
  //drive();
  //updateEncoderCount();
  batterycalculation();
  //display.print(F("battery"));
  //display.gotoXY(0,1);
  //display.print(batteryPercentage);



  /*
  display.clear();
  drive();
  calculateDistance();
  calculateVelocity(encoderTicks);
  calculateVelocity_2(distance);
  //display.print(F("distance"));
  //display.print(F("velocity"));
  display.print(F("velocity"));
  display.gotoXY(0,1);
  //display.print(distance);
  //display.print(velocity);
  display.print(velocity_2);
  */
 }
