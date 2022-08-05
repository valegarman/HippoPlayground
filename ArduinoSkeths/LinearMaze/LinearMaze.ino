
/* 
 *  Sofware for linear maze ...
 *  
 *  Ver 1.0, 08/2020
 *  BuzsakiLab-MV
 */

#include <MsTimer2.h> // // Extrict control time library

// USER VARIABLES
int caudalR = 80; // time solenoid is open, in ms
int caudalL = 80; // time solenoid is open, in ms

int beginStimTrial1 = 1000;
int endStimTrial1 = 1000; // 50 60
int beginStimTrial2 = 1000; // 61 70 
int endStimTrial2 = 1000;  // 100 120

// IO P
const int syncPin = 7; // For Intan/ Camera sync
const int valve4 = 48; // For left solenoid 
const int valve5 = 50; // For left solenoid

const int IRsensorL = 22; // IR sensor Left
const int IRsensorR = 24; // IR sensor Right

const int valve4Intan = 2; // output for intan 2
const int valve5Intan = 3; // output for intan pin 3

const int optStim = 5; //intan pin 6
boolean valveLeftState = 0; // Valve left on/off
boolean syncState = 0; // Sync state
const int blockPin = 40; // To other arduino for uLED control

// state variables
int previousTrial = 0;
const boolean aMouse = LOW; 
const boolean noMouse = HIGH;
int state = -1; // 0  is starting a new trial

long int timeCounter = 0;
int rTrial = 0;
int lTrial = 0;
int blocksCount = 0;
boolean stimBlock = false;
 
void setup() {
  // OUTPUTS
  pinMode(syncPin, OUTPUT);
  pinMode(valve4, OUTPUT); 
  pinMode(valve5, OUTPUT);
  pinMode(valve4Intan, OUTPUT); digitalWrite(valve4Intan, LOW);
  pinMode(valve5Intan, OUTPUT); digitalWrite(valve5Intan, LOW);
  pinMode(optStim, OUTPUT); digitalWrite(optStim, LOW);
  pinMode(blockPin, OUTPUT); digitalWrite(blockPin, LOW);

  // INPUTS
  pinMode(IRsensorL,INPUT); //IR1 input 
  pinMode(IRsensorR,INPUT); //IR2 input
  
  Serial.begin(9600); //Start serial communication boud rate at 9600
  Serial.flush();

  // MsTimer setup, running every 1000 ms
  MsTimer2::set(2000,flash); // run flash every 1000 ms
  MsTimer2::start(); //enable the interrupt
}

void loop() {
  if (digitalRead(IRsensorL)==aMouse){// Mouse go left
    if (previousTrial == 0 || previousTrial == 1){ // if first trial or previosly in right
      digitalWrite(valve5Intan, HIGH);
      digitalWrite(valve5, HIGH); delay(caudalL); digitalWrite(valve5, LOW);
      digitalWrite(valve5Intan, LOW);
      lTrial = lTrial + 1;
      previousTrial = 2;
    }
  }

  if (digitalRead(IRsensorR)==aMouse){// Mouse go right
    if (previousTrial == 0 || previousTrial == 2){ // if first trial or previosly in left
      digitalWrite(valve4Intan, HIGH);
      digitalWrite(valve4, HIGH); delay(caudalR); digitalWrite(valve4, LOW);
      digitalWrite(valve4Intan, LOW);
      rTrial = rTrial + 1;
      previousTrial = 1;
    }
  }

  if (rTrial + lTrial >= beginStimTrial1 & rTrial + lTrial <= endStimTrial1){
    digitalWrite(blockPin, HIGH);
    stimBlock = true;
  } else if (rTrial + lTrial >= beginStimTrial2 & rTrial + lTrial <= endStimTrial2) {
    digitalWrite(blockPin, HIGH);
    stimBlock = true;  
  } else {
    digitalWrite(blockPin, LOW);
    stimBlock = false;
  }
}
void flash(){
  syncState = !syncState;
  digitalWrite(syncPin, syncState);
  Serial.print(timeCounter);
  timeCounter = timeCounter + 2;
  Serial.print("s, right trials: ");
  Serial.print(rTrial);
  Serial.print(", left trials: ");
  Serial.print(lTrial);
  Serial.print(", total trials: ");
  Serial.print(rTrial + lTrial);
  if (stimBlock == false){
    Serial.println(", No stimulating");
    } else {Serial.println(", Stimulating!");}
}
