/* 
 *  Sofware for 8 maze ...
 *  
 *  Ver 1.0, 10/11/2022
 *  Manuel Valero
 */

#include <MsTimer2.h> // // Extrict control time library
#include <Servo.h>
Servo myServo;

// USER VARIABLES0

// To include random delay trials (true or false)
boolean randDelay = false;
int homeDelayVec[] = {100,1000*10};
int homeDelayRand; 

int caudalR = 55; // time solenoid is open, in ms
int caudalL = 60; // time solenoid is open, in ms
boolean forceAlternation = false; // for training or single track, true
int homeDelay = 100; // minium 100ms, to be sure that is recorded!!!! 
int optArm = 7; // 1 - optogenetics on right arm, 
                // 2 - on left arm, 
                // 3 - side arms
                // 4 - central arm
                // 5 - delay
                // 6 - random 
                // 7 - blocks (signal activation for an stimulus generator)
                // 0 - No stimulation
int blocksSize = 10; // Run subsequent control and stimulated blocks of this size, 0 is continous stim

// IO P
const int valve1 = 48; // For left solenoid 
const int valve2 = 50; // For left solenoid

const int IRsensor1 = 30; // IR sensor 1
const int IRsensor2 = 32; // IR sensor 2
const int IRsensor4 = 34; // IR sensor 4
const int IRsensor5 = 36; // IR sensor 5

const int door1Pin = 38; // Servo motor 1
const int door2Pin = 40; // Servo motor 2
const int door3Pin = 42; // Servo motor 3
const int door4Pin = 44; // Servo motor 4
const int door5Pin = 46; // Servo motor 5

const int valve1Intan = 2; // output for intan 2
const int valve2Intan = 3; // output for intan pin 3
const int homeDelayIntan = 4; // intan pin 4

const int cueSide = 10; //intan pin 6
const int optStim = 11;// to stimulus generator arduino

boolean valveLeftState = 0; // Valve left on/off

//Parameters for servo doors
const int openDoor1 = 50;
const int closeDoor1 = 179;

const int openDoor2 = 50; // 169
const int closeDoor2 = 179; // 31

const int openDoor3 = 50;
const int closeDoor3 = 179;

const int openDoor4 = 179;
const int closeDoor4 = 50;

const int openDoor5 = 173;
const int closeDoor5 = 30;

// state variables
const boolean aMouse = LOW; 
const boolean noMouse = HIGH;

int state = -1; // 0  is starting a new trial

long int timeCounter = 0;
int rTrial = 0;
int lTrial = 0;
int goodTrial = 0;
int cueSideNumber; // 0 is left, 1 is right
int sideTrial;
boolean goRight = false;
boolean goLeft = false;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
 
void setup() {
  // OUTPUTS
  pinMode(valve1, OUTPUT); 
  pinMode(valve2, OUTPUT);
  pinMode(valve1Intan, OUTPUT); digitalWrite(valve1Intan, LOW);
  pinMode(valve2Intan, OUTPUT); digitalWrite(valve2Intan, LOW);
  pinMode(homeDelayIntan, OUTPUT); digitalWrite(homeDelayIntan, LOW);
  pinMode(optStim, OUTPUT); digitalWrite(optStim, LOW);

  servo1.attach(door1Pin);
  servo2.attach(door2Pin);
  servo3.attach(door3Pin);
  servo4.attach(door4Pin);
  servo5.attach(door5Pin);

  // INPUTS
  pinMode(IRsensor1,INPUT); //IR1 input 
  pinMode(IRsensor2,INPUT); //IR2 input
  pinMode(IRsensor4,INPUT); //IR4 input
  pinMode(IRsensor5,INPUT); //IR5 input
  
  Serial.begin(9600); //Start serial communication boud rate at 9600
  Serial.flush();

  // MsTimer setup, running every 1000 ms
  MsTimer2::set(5000,flash); // run flash every 1000 ms
  MsTimer2::start(); //enable the interrupt

  randomSeed(analogRead(0));
}

void loop() {

  if (state == 0)  {
    if(digitalRead(IRsensor2) == aMouse){ // ANIMAL GO RIGHT
      digitalWrite(valve2Intan, HIGH); delay(2) ;digitalWrite(valve2Intan, LOW);
      if (sideTrial == false) {// if right trial was present, water!
         digitalWrite(valve2, HIGH); delay(caudalR); digitalWrite(valve2, LOW);
         goodTrial = goodTrial + 1;
      }
      if (optArm == 1 || optArm ==3) {// Start stim on the right arm
        digitalWrite(optStim, HIGH);
      } else {digitalWrite(optStim, LOW);}
      rTrial = rTrial + 1;
      // CLOSE LEFT AND STEAM, AND DOOR 1
      servo2.write(closeDoor2);
      servo5.write(openDoor5);
      delay(500);
      servo3.write(closeDoor3);
      servo4.write(closeDoor4);
      servo1.write(closeDoor1);
      // OPEN 5 AND LEFT
      state = 1; //
      }
    else if (digitalRead(IRsensor1) == aMouse) { // ANIMAL GO LEFT
      digitalWrite(valve1Intan, HIGH); delay(2) ;digitalWrite(valve1Intan, LOW);
      if (sideTrial == true) {// if left trial was present, water!
        digitalWrite(valve1, HIGH); delay(caudalL); digitalWrite(valve1, LOW);
        goodTrial = goodTrial + 1;
      } else {digitalWrite(optStim, LOW);}
      if (optArm == 2 || optArm == 3){// Start stim on the left arm
        digitalWrite(optStim, HIGH);
      } else {digitalWrite(optStim, LOW);}
      lTrial = lTrial + 1;
      // CLOSE RIGHT AND STEAM, AND DOOR 2
      servo1.write(closeDoor1);
      servo4.write(openDoor4);
      delay(500);
      servo3.write(closeDoor3);
      servo5.write(closeDoor5);
      servo2.write(closeDoor2);
      // OPEN 4 AND LEFT
      state = 2; // left
      }
    else {
  }}
  
  else if (state == 1) { // if animal comes from right
    if (digitalRead(IRsensor5) == aMouse){ // when detected
       servo5.write(closeDoor5);
       servo5.write(closeDoor4);
       servo1.write(openDoor1);
       servo2.write(openDoor2);
       //
       if (randDelay){
          homeDelayRand = random(2);
          homeDelay = homeDelayVec[homeDelayRand];
          }
       digitalWrite(homeDelayIntan, HIGH);
       digitalWrite(optStim, LOW);
       if (optArm == 6 || optArm == 5){// Start stim on the delay
          digitalWrite(optStim, HIGH);
       } else {digitalWrite(optStim, LOW);}
       delay(homeDelay);
       
       state = -1;
  }}
  else if (state == 2) {  // if animal comes from left
    if (digitalRead(IRsensor4) == aMouse){ // when detected
       servo4.write(closeDoor4);
       servo4.write(closeDoor5);
       servo1.write(openDoor1);
       servo2.write(openDoor2);
       //
       if (randDelay){
          homeDelayRand = random(2);
          homeDelay = homeDelayVec[homeDelayRand];
          }
       digitalWrite(homeDelayIntan, HIGH);
       if (optArm == 6 || optArm == 5){// Start stim on the delay
          digitalWrite(optStim, HIGH);
       } else {digitalWrite(optStim, LOW);}
       delay(homeDelay);
       servo3.write(openDoor3);
       state = -1;
  }}
  else if (state == -1) { // starting new
    sideTrial = changeCue();
    //
    servo3.write(openDoor3);
    state = 0;
    if (optArm == 6 || optArm == 4){// 
      digitalWrite(optStim, HIGH);
    } else {digitalWrite(optStim, LOW);
    };

    servo4.write(closeDoor4);
    servo5.write(closeDoor5);

    if (forceAlternation == true) {
      if (sideTrial == true){
        servo1.write(closeDoor1);
        servo2.write(openDoor2);
      } else {
        servo1.write(openDoor1);
        servo2.write(closeDoor2);
      }
      }
    }
}
void flash(){

  Serial.print(timeCounter);
  timeCounter = timeCounter + 5;
  Serial.print("s, right trials: ");
  Serial.print(rTrial);
  Serial.print(", left trials: ");
  Serial.print(lTrial);
  Serial.print(", good trials: ");
  Serial.print(goodTrial);
  Serial.print(", performance: ");
  Serial.print(100*goodTrial/(rTrial + lTrial));
  Serial.println("%");
}

boolean changeCue(){
  cueSideNumber = random(300);
  if ( (cueSideNumber % 2) == 0){
    digitalWrite(cueSide, LOW);
    return false;

  } else {
    digitalWrite(cueSide, HIGH);
    return true;
  }
}
