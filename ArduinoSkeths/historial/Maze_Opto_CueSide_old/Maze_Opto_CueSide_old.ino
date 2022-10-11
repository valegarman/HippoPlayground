#include <Adafruit_NeoPixel.h>
#include <MsTimer2.h> // // Extrict control time library
#include <Servo.h>
Servo myServo;

// USER VARIABLES
int caudalR = 70; // time solenoid is open, in ms
int caudalL = 70; // time solenoid is open, in ms
int homeDelay = 5000; // minium 100ms, to be sure that is recorded!!!! 
int optArm = 0; // 1 - optogenetics on right arm, 
                // 2 - on left arm, 
                // 3 - side arms
                // 4 - central arm
                // 5 - delay
                // 0 - No stimulation
int blocksSize = 0;// Blocks with forceAlternation

// IO P
const int syncPin = 7; // For Intan/ Camera sync
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

const int optStim = 39; //intan pin 6

boolean syncState = 0; // Sync state
boolean valveLeftState = 0; // Valve left on/off
const int openDoor = 20; // Parameters for servo doors
const int closeDoor = 179;
boolean firstTrial = true;

// LED matrix parameters
#define PIN 52    // in pin
#define ROW_16

// MASK DEFINITION
#define ROW_SIZE 16
byte maskLeft[][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0, 
   0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
 
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0}
  };
 int colorMaskLeft[3]={50, 200, 100};

 byte maskRight[][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
 
   {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0, 
   0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0}};
   
 int colorMaskRight[3]={50, 200, 100};
 
 byte empty[][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
 
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0, 
   0,0,0,0,0,0,0,0}
  };

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
boolean stimTrial = false;

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
      servo1.write(closeDoor);
      servo3.write(closeDoor);
      servo4.write(closeDoor);
      servo5.write(closeDoor);
      // OPEN 2 AND LEFT
      servo2.write(openDoor);
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
      servo2.write(closeDoor);
      servo3.write(closeDoor);
      servo4.write(closeDoor);
      servo5.write(closeDoor);
      // OPEN 2 AND LEFT
      servo1.write(openDoor);
      //servo4.write(openDoor);
      sideTrial = changeCue();
      state = 2; // left
      }
    else {
  }}
  
  else if (state == 1) { // if animal comes from right
    if (digitalRead(IRsensor2) == aMouse){ // when detected
       servo2.write(closeDoor);
       // servo4.write(closeDoor);
       // servo5.write(openDoor);
       digitalWrite(homeDelayIntan, HIGH);
       digitalWrite(optStim, LOW);
       if (optArm == 5){// Start stim on the delay
          digitalWrite(optStim, stimTrial);
       } else {digitalWrite(optStim, LOW);}
       delay(homeDelay);
       state = -1;
  }}
  else if (state == 2) {  // if animal comes from left
    if (digitalRead(IRsensor1) == aMouse){ // when detected
       servo1.write(closeDoor);
       // servo5.write(closeDoor);
       // servo4.write(openDoor);
       digitalWrite(homeDelayIntan, HIGH);
       if (optArm == 5){// Start stim on the delay
          digitalWrite(optStim, stimTrial);
       } else {digitalWrite(optStim, LOW);}
       delay(homeDelay);
       state = -1;
  }}
  else if (state == -1) { // starting new
    digitalWrite(homeDelayIntan, LOW);
    servo3.write(openDoor);
    state = 0;
    if (optArm == 4){// Start stim on the right arm
      digitalWrite(optStim, stimTrial);
    } else {digitalWrite(optStim, LOW);};
    
    if (blocksCount <= blocksSize && blocksSize > 0) {
      forceAlternation = true;
      blocksCount = blocksCount + 1;
      if (sideTrial == true){
        servo4.write(openDoor);
        servo5.write(closeDoor);
      } else {
        servo4.write(closeDoor);
        servo5.write(openDoor);  
      }
    } 
    else {
      forceAlternation = false;
      }

    if (firstTrial==true) {
      digitalWrite(optStim, LOW);
      servo1.write(closeDoor);
      servo2.write(closeDoor);
      firstTrial = false;
      }
      
    digitalWrite(forceAlternationIntan, forceAlternation);
    if (forceAlternation == false) {
      servo4.write(openDoor);
      servo5.write(openDoor);
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

void maskedColorWipe(uint8_t wait, uint32_t color, uint8_t brightness, uint8_t shift, byte mask[][ROW_SIZE]) {
  uint16_t i, j;

  for(i=0; i<matrix.numPixels(); i++) {
    if (drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, shift, mask)){
      matrix.setBrightness(brightness);
      matrix.setPixelColor(i, color);
    }else{
      matrix.setPixelColor(i, 0);
    }
  }
  
  matrix.show();
  delay(wait);
}

boolean drawGivenMask(int row, int col, int shift, byte mask[][ROW_SIZE]){
  #ifdef FLIPMODE
  if (row & 1) {
    col = ROW_SIZE - (col + 1);
  }  
  #endif
  col = (col + shift) % ROW_SIZE;
  
  if (mask[row][col] & 1){
    return true;
  }
  return false;
}

boolean changeCue(){
  cueSideNumber = random(300);
  Serial.println(cueSideNumber);
  if ( (cueSideNumber % 2) == 0){
    uint32_t color = matrix.Color(colorMaskLeft[0],colorMaskLeft[1],colorMaskLeft[2]);
    maskedColorWipe(1, color, 3, 0, maskLeft);
    digitalWrite(cueSide, LOW);
    return false;

  } else {
    uint32_t color = matrix.Color(colorMaskRight[0],colorMaskRight[1],colorMaskRight[2]);
    maskedColorWipe(1, color, 3, 0, maskRight);
    digitalWrite(cueSide, HIGH);
    return true;
  }
}
