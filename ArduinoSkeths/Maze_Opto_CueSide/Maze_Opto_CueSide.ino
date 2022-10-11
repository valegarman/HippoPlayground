#include <Adafruit_NeoPixel.h>
#include <MsTimer2.h> // // Extrict control time library
#include <Servo.h>
Servo myServo;

// USER VARIABLES
int caudalR =60; // time solenoid is open, in ms
int caudalL =50; // time solenoid is open, in ms
int homeDelay = 5000; // minium 100ms, to be sure that is recorded!!!! 
int optArm = 0; // 1 - optogenetics on right arm, 
                // 2 - on left arm, 
                // 3 - side arms
                // 4 - central arm
                // 5 - delay
                // 0 - No stimulation
                // 6 - delay and central arm (block cues)
int blocksSize = 0;// Blocks with forceAlternation

// IO P
const int syncPin = 0; // For Intan/ Camera sync
const int valve4 = 3; // For left solenoid 
const int valve5 = 2; // For left solenoid

const int IRsensor1 = 4; // IR sensor 1
const int door1Pin = 12; // Servo motor 1

const int IRsensor2 = 5; // IR sensor 2
const int door2Pin = 11; // Servo motor 2

const int door3Pin = 10; // Servo motor 3

const int IRsensor4 = 22; // IR sensor 4
const int door4Pin = 9; // Servo motor 4

const int IRsensor5 = 23; // IR sensor 5
const int door5Pin = 8; // Servo motor 5

const int valve4Intan = 31; // output for intan 2
const int valve5Intan = 33; // output for intan pin 3
const int homeDelayIntan = 35; // intan pin 4
const int forceAlternationIntan = 37; // intan pin 5
const int cueSide = 30; // intan pin 6 

const int optStim = 39; //intan pin 7

boolean syncState = 0; // Sync state
boolean valveLeftState = 0; // Valve left on/off
const int openDoor = 30; // Parameters for servo doors
const int closeDoor = 169;
const int openDoorN = 169; // New servos have an inverse movement
const int closeDoorN = 30;
boolean firstTrial = true;

// LED matrix parameters
#define PIN 52    // in pin
#define ROW_16

 // MASK DEFINITION
#define ROW_SIZE 16
byte maskLeft[][ROW_SIZE] = {
  {2,2,2,2,0,0,0,0, 
   2,2,2,2,0,0,0,0},
  {2,2,2,2,0,0,0,0, 
   2,2,2,2,0,0,0,0},
  {0,0,0,0,2,2,2,2, 
   0,0,0,0,2,2,2,2},
  {0,0,0,0,2,2,2,2, 
   0,0,0,0,2,2,2,2},
 

  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0}
  };

 byte maskRight[][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
   
  {2,2,2,2,0,0,0,0, 
   2,2,2,2,0,0,0,0},
  {2,2,2,2,0,0,0,0, 
   2,2,2,2,0,0,0,0},
  {0,0,0,0,2,2,2,2, 
   0,0,0,0,2,2,2,2},
  {0,0,0,0,2,2,2,2, 
   0,0,0,0,2,2,2,2}
 };
   
 int colorMask1[3]={50, 200, 50}; // {50,  200, 100};   
 int colorMask2[3]={0, 150, 250}; // {200, 200, 0};
 int brightness1 = 1;
 int brightness2 = 1;

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(128, PIN, NEO_GRB + NEO_KHZ800);
long cueSideNumber; // 0 is left, 1 is right

// state variables
int sideTrial;

const boolean aMouse = LOW; 
const boolean noMouse = HIGH;
boolean forceAlternation = false; // not a parameter in cue version

int state = -1; // 0  is starting a new trial

long int timeCounter = 0;
int rTrial = 0;
int lTrial = 0;
int goodTrial = 0;
int blocksCount = 0;
boolean stimTrial = true;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
 
void setup() {
  // OUTPUTS
  pinMode(syncPin, OUTPUT);
  pinMode(valve4, OUTPUT); 
  pinMode(valve5, OUTPUT);
  pinMode(valve4Intan, OUTPUT); digitalWrite(valve4Intan, LOW);
  pinMode(valve5Intan, OUTPUT); digitalWrite(valve5Intan, LOW);
  pinMode(homeDelayIntan, OUTPUT); digitalWrite(homeDelayIntan, LOW);
  pinMode(forceAlternationIntan, OUTPUT);
  pinMode(optStim, OUTPUT); digitalWrite(optStim, LOW);

  servo1.attach(door1Pin);
  servo2.attach(door2Pin);
  servo3.attach(door3Pin);
  servo4.attach(door4Pin);
  servo5.attach(door5Pin);

  // INPUTS
  pinMode(IRsensor1,INPUT); //IR1 input 
  pinMode(IRsensor2,INPUT); //IR2 input
  pinMode(IRsensor4,INPUT); //IR2 input
  pinMode(IRsensor5,INPUT); //IR2 input
  
  Serial.begin(9600); //Start serial communication boud rate at 9600
  Serial.flush();

  // MsTimer setup, running every 1000 ms
  MsTimer2::set(2000,flash); // run flash every 1000 ms
  MsTimer2::start(); //enable the interrupt

  // LED matrix
  matrix.begin();
  matrix.show(); // Initialize all pixels to 'off'

  randomSeed(analogRead(15));
  // Initial cue
 sideTrial = changeCue();
}

void loop() {
  if (state == 0)  {// if newTrial 0, 1 or 2, check sensors
    if(digitalRead(IRsensor4) == aMouse){ // ANIMAL GO RIGHT
      digitalWrite(valve4Intan, HIGH); delay(1) ;digitalWrite(valve4Intan, LOW);
      if (sideTrial == true) {// if right cue is present
         digitalWrite(valve4, HIGH); delay(caudalR); digitalWrite(valve4, LOW);
         goodTrial = goodTrial + 1;
      }
      if (optArm == 1 || optArm ==3) {// Start stim on the right arm
        digitalWrite(optStim, stimTrial);
        } else {digitalWrite(optStim, LOW);}
      rTrial = rTrial + 1;
      // CLOSE LEFT AND STEAM, AND DOOR 1
      servo1.write(closeDoorN); // new doors are different
      servo3.write(closeDoor);
      servo4.write(closeDoorN); 
      delay(500);
      servo5.write(closeDoorN);
      // OPEN 2 AND LEFT
      servo2.write(openDoorN);
      //servo5.write(openDoor);
      sideTrial = changeCue();
      state = 1; // right!!
      }
    else if (digitalRead(IRsensor5) == aMouse) { // ANIMAL GO LEFT
      digitalWrite(valve5Intan, HIGH); delay(1) ;digitalWrite(valve5Intan, LOW);
      if (sideTrial == false) {// if left cue is present
        digitalWrite(valve5, HIGH); delay(caudalL); digitalWrite(valve5, LOW);
        goodTrial = goodTrial + 1;
      }
      if (optArm == 2 || optArm == 3){// Start stim on the left arm
        digitalWrite(optStim, stimTrial);
      } else {digitalWrite(optStim, LOW);}
      lTrial = lTrial + 1;
      // CLOSE RIGHT AND STEAM, AND DOOR 2
      servo2.write(closeDoorN);
      servo3.write(closeDoor);
      servo5.write(closeDoorN);
      delay(500);
      servo4.write(closeDoorN);
      // OPEN 2 AND LEFT
      servo1.write(openDoorN);
      //servo4.write(openDoor);
      sideTrial = changeCue();
      state = 2; // left
      }
    else {
  }}
  
  else if (state == 1) { // if animal comes from right
    if (digitalRead(IRsensor2) == aMouse){ // when detected
       servo2.write(closeDoorN);
       // servo4.write(closeDoor);
       // servo5.write(openDoor);
       digitalWrite(homeDelayIntan, HIGH);
       digitalWrite(optStim, LOW);
       if (optArm == 5 || optArm == 6){// Start stim on the delay
          digitalWrite(optStim, stimTrial);
       } else {digitalWrite(optStim, LOW);}
       delay(homeDelay);
       state = -1;
  }}
  else if (state == 2) {  // if animal comes from left
    if (digitalRead(IRsensor1) == aMouse){ // when detected
       servo1.write(closeDoorN);
       // servo5.write(closeDoor);
       // servo4.write(openDoor);
       digitalWrite(homeDelayIntan, HIGH);
       if (optArm == 5 || optArm == 6){// Start stim on the delay
          digitalWrite(optStim, stimTrial);
       } else {digitalWrite(optStim, LOW);}
       delay(homeDelay);
       state = -1;
  }}
  else if (state == -1) { // starting new
    digitalWrite(homeDelayIntan, LOW);
    servo3.write(openDoor);
    state = 0;
    if (optArm == 4 || optArm == 6){// Start stim on the center arm
      digitalWrite(optStim, stimTrial);
    } else {digitalWrite(optStim, LOW);};
    
    if (blocksCount <= blocksSize && blocksSize > 0) {
      forceAlternation = false;
      blocksCount = blocksCount + 1;
      if (sideTrial == true){
        servo4.write(openDoorN);
        servo5.write(closeDoorN);
      } else {
        servo4.write(closeDoorN);
        servo5.write(openDoorN);  
      }
    } 
    else {
      forceAlternation = false;
      }

    if (firstTrial==true) {
      digitalWrite(optStim, LOW);
      servo1.write(closeDoorN);
      servo2.write(closeDoorN);
      firstTrial = false;
      }
      
    digitalWrite(forceAlternationIntan, forceAlternation);
    if (forceAlternation == false) {
      servo4.write(openDoorN);
      servo5.write(openDoorN);
      }
    }
}

void flash(){
  syncState = !syncState;
  digitalWrite(syncPin, syncState);
  Serial.print(timeCounter);
  timeCounter = timeCounter + 2;
  Serial.print("s, Good trials: ");
  Serial.print(goodTrial);
  Serial.print("/ ");
  Serial.print(rTrial + lTrial);
  Serial.print(", performance: ");
  Serial.print(100*goodTrial/(rTrial + lTrial));
  Serial.println("%");
}

void maskedColorWipe(uint8_t wait, uint32_t color1, uint8_t brightness1, uint32_t color2, uint8_t brightness2,
    uint8_t shift, byte mask[][ROW_SIZE]) {
  uint16_t i, j;

  for(i=0; i<matrix.numPixels(); i++) {
    if (drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, shift, mask)==1){
      matrix.setBrightness(brightness1);
      matrix.setPixelColor(i, color1);
    } else if (drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, shift, mask)==2){
      matrix.setBrightness(brightness2);
      matrix.setPixelColor(i, color2);
    } else{
      matrix.setPixelColor(i, 0);
    }
  }
  
  matrix.show();
  delay(wait);
}

int drawGivenMask(int row, int col, int shift, byte mask[][ROW_SIZE]){
  #ifdef FLIPMODE
  if (row & 1) {
    col = ROW_SIZE - (col + 1);
  }  
  #endif
  col = (col + shift) % ROW_SIZE;
  
  if (mask[row][col]==1 & 1){
    return 1;
  }
  else if (mask[row][col]==2 & 1){
    return 2;
  }   
  return 0;
}

boolean changeCue(){
  cueSideNumber = random(300);
  // Serial.println(cueSideNumber);
  if ( (cueSideNumber % 2) == 0){
    uint32_t color1 = matrix.Color(colorMask1[0],colorMask1[1],colorMask1[2]);
    uint32_t color2 = matrix.Color(colorMask2[0],colorMask2[1],colorMask2[2]);
    maskedColorWipe(1, color1, brightness1, color2, brightness2, 0, maskLeft);
    digitalWrite(cueSide, LOW);
    return false;

  } else {
    uint32_t color1 = matrix.Color(colorMask1[0],colorMask1[1],colorMask1[2]);
    uint32_t color2 = matrix.Color(colorMask2[0],colorMask2[1],colorMask2[2]);
    maskedColorWipe(1, color1, brightness1, color2, brightness2, 0, maskRight);
    digitalWrite(cueSide, HIGH);
    return true;
  }
}


/*
 *  
 *  Pattern1: vertical strippes and diagonal
byte maskLeft[][ROW_SIZE] = {
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
 
  {2,2,2,2,0,0,0,0, 
   0,2,2,2,2,0,0,0},
  {0,0,2,2,2,2,0,0, 
   0,0,0,2,2,2,2,0},
  {0,0,0,0,2,2,2,2, 
   0,0,0,0,0,2,2,2},
  {0,0,0,0,0,0,2,2, 
   0,0,0,0,0,0,0,2}
  };

 byte maskRight[][ROW_SIZE] = {
  {0,0,0,0,2,2,2,2, 
   0,0,0,2,2,2,2,0},
  {0,0,2,2,2,2,0,0, 
   0,2,2,2,2,0,0,0},
  {2,2,2,2,0,0,0,0, 
   2,2,2,0,0,0,0,0},
  {2,2,0,0,0,0,0,0, 
   2,0,0,0,0,0,0,0},

  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
   };
 int colorMask1[3]={0, 150, 200}; // {50,  200, 100};   
 int colorMask2[3]={200, 200, 0}; // {200, 200, 0};
 int brightness1 = 3;
 int brightness2 = 3;


*  Pattern3: soft diagonal and big horizontal stripples: good!!

 byte maskLeft[][ROW_SIZE] = {
  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
 
  {2,2,2,2,0,0,0,2, 
   2,2,2,0,0,0,2,0},
  {2,2,0,0,0,2,0,0, 
   2,0,0,0,2,0,0,0},
  {0,0,0,2,0,0,0,2, 
   0,0,2,0,0,0,2,2},
  {0,2,0,0,0,2,2,2, 
   2,0,0,0,2,2,2,2}
  };

 byte maskRight[][ROW_SIZE] = {
  {2,2,0,0,2,2,2,2, 
   0,2,0,0,0,2,2,2},
  {0,0,2,0,0,0,2,2, 
   0,0,0,2,0,0,0,2},
  {2,0,0,0,2,0,0,0, 
   2,2,0,0,0,2,0,0},
  {2,2,2,0,0,0,2,0, 
   2,2,2,2,0,0,0,2},

  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
   };
 int colorMask1[3]={250, 250, 0}; // {50,  200, 100};   
 int colorMask2[3]={000, 0, 100}; // {200, 200, 0};
 int brightness1 = 3;
 int brightness2 = 3;

 %% Pattern 4: cheesboard and diagonal: good!!
byte maskLeft[][ROW_SIZE] = {
  {1,1,1,1,0,0,0,0, 
   1,1,1,1,0,0,0,0},
  {1,1,1,1,0,0,0,0, 
   1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,1, 
   0,0,0,0,1,1,1,1},
  {0,0,0,0,1,1,1,1, 
   0,0,0,0,1,1,1,1},
 

  {2,2,2,0,0,0,0,0, 
   0,2,2,2,0,0,0,0},
  {0,0,2,2,2,0,0,0, 
   0,0,0,2,2,2,0,0},
  {0,0,0,0,2,2,2,0, 
   0,0,0,0,0,2,2,2},
  {0,0,0,0,0,0,2,2, 
   0,0,0,0,0,0,0,2}
  };

 byte maskRight[][ROW_SIZE] = {
  {0,0,0,0,0,2,2,2, 
   0,0,0,0,2,2,2,0},
  {0,0,0,2,2,2,0,0, 
   0,0,2,2,2,0,0,0},
  {0,2,2,2,0,0,0,0, 
   2,2,2,0,0,0,0,0},
  {2,2,0,0,0,0,0,0, 
   2,0,0,0,0,0,0,0},

  {0,0,0,0,1,1,1,1, 
   0,0,0,0,1,1,1,1},
  {0,0,0,0,1,1,1,1, 
   0,0,0,0,1,1,1,1},
  {1,1,1,1,0,0,0,0, 
   1,1,1,1,0,0,0,0},
  {1,1,1,1,0,0,0,0, 
   1,1,1,1,0,0,0,0},
   };
 int colorMask1[3]={50, 200, 50}; // {50,  200, 100};   
 int colorMask2[3]={0, 150, 250}; // {200, 200, 0};
 int brightness1 = 3;
 int brightness2 = 3;
 
 % pattern: horizontal and vertical
 byte maskLeft[][ROW_SIZE] = {
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
 
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {2,2,2,2,2,2,2,2, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   2,2,2,2,2,2,2,2},
  {2,2,2,2,2,2,2,2, 
   2,2,2,2,2,2,2,2},
  };

 byte maskRight[][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {2,2,2,2,2,2,2,2, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   2,2,2,2,2,2,2,2},
  {2,2,2,2,2,2,2,2, 
   2,2,2,2,2,2,2,2},

  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
   };
 int colorMask1[3]={0, 150, 200}; // {50,  200, 100};   
 int colorMask2[3]={150, 150, 0}; // {200, 200, 0};
 int brightness1 = 3;
 int brightness2 = 3;

 // MASK DEFINITION % cross and diagonal: good!!!
#define ROW_SIZE 16
 byte maskRight[][ROW_SIZE] = {
  {0,0,0,1,1,0,0,0, 
   0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0, 
   1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1, 
   0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0, 
   0,0,0,1,1,0,0,0},
 
  {0,2,2,0,0,2,2,0, 
   0,0,2,2,0,0,2,2},
  {2,0,0,2,2,0,0,2, 
   2,2,0,0,2,2,0,0},
  {0,2,2,0,0,2,2,0, 
   0,0,2,2,0,0,2,2},
  {2,0,0,2,2,0,0,2, 
   2,2,0,0,2,2,0,0},
  };

 byte maskLeft[][ROW_SIZE] = {
  {0,2,2,0,0,2,2,0, 
   2,2,0,0,2,2,0,0},
  {2,0,0,2,2,0,0,2, 
   0,0,2,2,0,0,2,2},
  {0,2,2,0,0,2,2,0, 
   2,2,0,0,2,2,0,0},
  {2,0,0,2,2,0,0,2, 
   0,0,2,2,0,0,2,2},
   
  {0,0,0,1,1,0,0,0, 
   0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0, 
   1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1, 
   0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0, 
   0,0,0,1,1,0,0,0},
   };
 int colorMask1[3]={200, 200, 50}; // {50,  200, 100};   
 int colorMask2[3]={50,0, 250}; // {200, 200, 0};
 int brightness1 = 3;
 int brightness2 = 3;

 ** stripes and square
 * // MASK DEFINITION
#define ROW_SIZE 16
 byte maskLeft[][ROW_SIZE] = {
  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
  {0,0,0,0,0,1,1,1, 
   0,0,0,1,1,1,0,0},
  {0,1,1,1,0,0,0,0, 
   1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
 
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,2,2,2,2,0,0, 
   0,0,2,2,2,2,0,0},
  {0,0,2,2,2,2,0,0, 
   0,0,2,2,2,2,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  };

 byte maskRight[][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,2,2,2,2,0,0, 
   0,0,2,2,2,2,0,0},
  {0,0,2,2,2,2,0,0, 
   0,0,2,2,2,2,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
   
  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
  {1,1,1,0,0,0,0,0, 
   0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,0,0, 
   0,0,0,0,0,1,1,1},
  {1,1,1,1,1,1,1,1, 
   1,1,1,1,1,1,1,1},
   };
 int colorMask1[3]={0, 250, 50}; // {50,  200, 100};   
 int colorMask2[3]={150,0, 250}; // {200, 200, 0};
 int brightness1 = 3;
 int brightness2 = 3;


 * Pattern square and random dots! good!!!
 // MASK DEFINITION
#define ROW_SIZE 16
 byte maskRight[][ROW_SIZE] = {
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {0,0,1,1,0,0,1,0, 
   0,0,1,1,0,0,1,1},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
 
  {2,2,2,2,2,2,2,2, 
   2,2,2,2,2,2,2,2},
  {2,2,0,0,0,0,2,2, 
   2,2,0,0,0,0,2,2},
  {2,2,0,0,0,0,2,2, 
   2,2,0,0,0,0,2,2},
  {2,2,2,2,2,2,2,2, 
   2,2,2,2,2,2,2,2},
  };

 byte maskLeft[][ROW_SIZE] = {
  {2,2,2,2,2,2,2,2, 
   2,2,2,2,2,2,2,2},
  {2,2,0,0,0,0,2,2, 
   2,2,0,0,0,0,2,2},
  {2,2,0,0,0,0,2,2, 
   2,2,0,0,0,0,2,2},
  {2,2,2,2,2,2,2,2, 
   2,2,2,2,2,2,2,2},
   
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
  {0,0,1,1,0,0,1,1, 
   0,0,1,1,0,0,1,1},
  {1,1,0,0,1,1,0,0, 
   1,1,0,0,1,1,0,0},
   };
 int colorMask1[3]={0, 250, 50}; // {50,  200, 100};   
 int colorMask2[3]={50,0, 150}; // {200, 200, 0};
 int brightness1 = 3;
 int brightness2 = 3;

 
 
 */
