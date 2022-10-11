

/* 
 *  Arduino code for flute maze
 *  
 *  Ver 1.1, 8/2022
 *  08/19/22 Now includes LED to signal new trial
 *  08/19/22 Includes sound when error
 *  08/19/22 Now it is possible to specify number of rewards position to find
 *  Manuel Valero
 */

 #include <MsTimer2.h> // // Extrict control time library
 #include <toneAC.h>

// USER VARIABLES
int caudal1 = 100; // time solenoid is open, in ms
int caudal2 = 80; // time solenoid is open, in ms
int caudal3 = 120; // time solenoid is open, in ms
int caudal4 = 120; // time solenoid is open, in ms
int caudal5 = 80; // time solenoid is open, in ms
int caudal6 = 80; // time solenoid is open, in ms
int caudal7 = 80; // time solenoid is open, in ms

int phase = 1; // trainning is 0, test is 1, probe is 2;
bool soundErrors = true;
int soundErrorFreq = 5000; // in HZ
int soundErrorDuration = 500; // in ms

/* ==================== SELECT REWARDED POSITIONS =======================*/
// pos1 and pos7 are at both extremes of the maze. They are always rewarded
// and their activation starts a new forward (pos1) or reverse (pos2) trial
int pos2_forward = 1;
int pos3_forward = 1;
int pos4_forward = 1;
int pos5_forward = 0;
int pos6_forward = 0;

int pos2_reverse = 0;
int pos3_reverse = 0;
int pos4_reverse = 0;
int pos5_reverse = 0;
int pos6_reverse = 0;

int pos1_forward = 0;
int pos7_forward = 0;

int rewardsToFind_FORWAD = 1;;
int rewardsToFind_REVERSE = 0;

/* ==================== PINS ============================================*/
// Solenoid Pins
const int solPin1 = 40; // Ouputpin for close circuit on transistor - #1
const int solPin2 = 42; // Ouputpin for close circuit on transistor - #2
const int solPin3 = 44; // Ouputpin for close circuit on transistor - #3
const int solPin4 = 46; // Ouputpin for close circuit on transistor - #4
const int solPin5 = 48; // Ouputpin for close circuit on transistor - #5
const int solPin6 = 50; // Ouputpin for close circuit on transistor - #6
const int solPin7 = 52; // Ouputpin for close circuit on transistor - #7

// IR sensors
const int lickPin1 = 41;  // Input pin for IR #1
const int lickPin2 = 43;  // Input pin for IR #2
const int lickPin3 = 45;  // Input pin for IR #3
const int lickPin4 = 47;  // Input pin for IR #4
const int lickPin5 = 49;  // Input pin for IR #5
const int lickPin6 = 51;  // Input pin for IR #6
const int lickPin7 = 53;  // Input pin for IR #7

// LED
const int ledPin1 = 10; // Output pin to power an LED to start trial

/* ==============INTAN PINS ============================================*/

// Solenoid Pins
const int solTTLPin1 = 27; // Ouputpin for close circuit on transistor - #1
const int solTTLPin2 = 29; // Ouputpin for close circuit on transistor - #2
const int solTTLPin3 = 31; // Ouputpin for close circuit on transistor - #3
const int solTTLPin4 = 33; // Ouputpin for close circuit on transistor - #4
const int solTTLPin5 = 35; // Ouputpin for close circuit on transistor - #5
const int solTTLPin6 = 37; // Ouputpin for close circuit on transistor - #6
const int solTTLPin7 = 39; // Ouputpin for close circuit on transistor - #7

// IR sensors
const int lickTTLPin1 = 26;  // Input pin for touch state - #1
const int lickTTLPin2 = 28;  // Input pin for touch state - #2
const int lickTTLPin3 = 30;  // Input pin for touch state - #3
const int lickTTLPin4 = 32;  // Input pin for touch state - #4
const int lickTTLPin5 = 34;  // Input pin for touch state - #5
const int lickTTLPin6 = 36;  // Input pin for touch state - #6
const int lickTTLPin7 = 38;  // Input pin for touch state - #7

/*==================== SETTINGS ========================================================*/
int lickmeterState1 = 0;
int lickmeterState2 = 0;
int lickmeterState3 = 0;
int lickmeterState4 = 0;
int lickmeterState5 = 0;
int lickmeterState6 = 0;
int lickmeterState7 = 0;

// state variables
int state = 0; // 1 is forward, 2 is reverse, 0 is initial trial
const boolean aMouse = HIGH; 
const boolean noMouse = LOW;
int rewardsFound = 0;
long int timeCounter = 0;
int trialNumber = 0;

int state_pos1_forward = pos1_forward;
int state_pos2_forward = pos2_forward;
int state_pos3_forward = pos3_forward;
int state_pos4_forward = pos4_forward;
int state_pos5_forward = pos5_forward;
int state_pos6_forward = pos6_forward;
int state_pos7_forward = pos7_forward;

int state_pos2_reverse = pos2_reverse;
int state_pos3_reverse = pos3_reverse;
int state_pos4_reverse = pos4_reverse;
int state_pos5_reverse = pos5_reverse;
int state_pos6_reverse = pos6_reverse;

// int rewardsToFind_FORWAD = pos2_forward + pos3_forward + pos4_forward + pos5_forward + pos6_forward;;
// int rewardsToFind_REVERSE = pos2_reverse + pos3_reverse + pos4_reverse + pos5_reverse + pos6_reverse;

void setup() {

  Serial.begin(9600);
  randomSeed(analogRead(A1));
  
  pinMode(lickPin1, INPUT); // set pin as INPUT
  pinMode(lickPin2, INPUT); // set pin as INPUT
  pinMode(lickPin3, INPUT); // set pin as INPUT
  pinMode(lickPin4, INPUT); // set pin as INPUT
  pinMode(lickPin5, INPUT); // set pin as INPUT
  pinMode(lickPin6, INPUT); // set pin as INPUT
  pinMode(lickPin7, INPUT); // set pin as INPUT
  
  pinMode(solPin1, OUTPUT); // set pin as OUTPUT
  pinMode(solPin2, OUTPUT); // set pin as OUTPUT
  pinMode(solPin3, OUTPUT); // set pin as OUTPUT
  pinMode(solPin4, OUTPUT); // set pin as OUTPUT
  pinMode(solPin5, OUTPUT); // set pin as OUTPUT
  pinMode(solPin6, OUTPUT); // set pin as OUTPUT
  pinMode(solPin7, OUTPUT); // set pin as OUTPUT
  
  pinMode(solTTLPin1, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin2, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin3, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin4, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin5, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin6, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin7, OUTPUT); // set pin as OUTPUT

  pinMode(lickTTLPin1, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin2, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin3, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin4, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin5, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin6, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin7, OUTPUT); // set pin as OUTPUT

  pinMode(ledPin1, OUTPUT); // set pin as OUTPUT

  digitalWrite(solPin1, LOW); // OFF
  digitalWrite(solTTLPin1, LOW); // OFF  
  digitalWrite(solPin2, LOW); // OFF
  digitalWrite(solTTLPin2, LOW); // OFF         
  digitalWrite(solPin3, LOW); // OFF
  digitalWrite(solTTLPin3, LOW); // OFF  
  digitalWrite(solPin4, LOW); // OFF
  digitalWrite(solTTLPin4, LOW); // OFF   
  digitalWrite(solPin5, LOW); // OFF
  digitalWrite(solTTLPin5, LOW); // OFF         
  digitalWrite(solPin6, LOW); // OFF
  digitalWrite(solTTLPin6, LOW); // OFF  
  digitalWrite(solPin7, LOW); // OFF
  digitalWrite(solTTLPin7, LOW); // OFF     

  // MsTimer setup, running every 2000 ms
  MsTimer2::set(2000,flash); // run flash every 1000 ms
  MsTimer2::start(); //enable the interrupt
}

void loop() {
    // read (and write into Intan) licking behaviour
    lickmeterState1 = digitalRead(lickPin1);
    lickmeterState2 = digitalRead(lickPin2);
    lickmeterState3 = digitalRead(lickPin3);
    lickmeterState4 = digitalRead(lickPin4);
    lickmeterState5 = digitalRead(lickPin5);
    lickmeterState6 = digitalRead(lickPin6);
    lickmeterState7 = digitalRead(lickPin7);  
  
    digitalWrite(lickTTLPin1, lickmeterState1);
    digitalWrite(lickTTLPin2, lickmeterState2);
    digitalWrite(lickTTLPin3, lickmeterState3);
    digitalWrite(lickTTLPin4, lickmeterState4);
    digitalWrite(lickTTLPin5, lickmeterState5);
    digitalWrite(lickTTLPin6, lickmeterState6);
    digitalWrite(lickTTLPin7, lickmeterState7);
  
  if (phase == 1){
    if (state == 0){ // first trial, mouse should lick in pos1 one to activate a FORWARD trial
      digitalWrite(ledPin1, HIGH);    // ON
      if (lickmeterState1==aMouse){// Mouse licks in first port,
        digitalWrite(solPin1, HIGH);    // ON
        digitalWrite(solTTLPin1, HIGH); // ON  
        delay(caudal1);
        digitalWrite(solPin1, LOW);     // OFF
        digitalWrite(solTTLPin1, LOW);  // OFF 
        state = 1; // FORWARD trial
        digitalWrite(ledPin1, LOW);    // OFF
      }
     }
  
    if (state == 1){ // if FORWARD trial
        if (lickmeterState2==aMouse && state_pos2_forward){
            digitalWrite(solPin2, HIGH);    // ON
            digitalWrite(solTTLPin2, HIGH); // ON  
            delay(caudal2);
            digitalWrite(solPin2, LOW);     // OFF
            digitalWrite(solTTLPin2, LOW);  // OFF 
            state_pos2_forward = 0; // deactivate port
            rewardsFound = rewardsFound + 1;}
        else if (lickmeterState2==aMouse && soundErrors && !pos2_forward){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }
            
        if (lickmeterState3==aMouse && state_pos3_forward){
            digitalWrite(solPin3, HIGH);    // ON
            digitalWrite(solTTLPin3, HIGH); // ON  
            delay(caudal3);
            digitalWrite(solPin3, LOW);     // OFF
            digitalWrite(solTTLPin3, LOW);  // OFF 
            state_pos3_forward = 0; // deactivate port
            rewardsFound = rewardsFound + 1;}
        else if (lickmeterState3==aMouse && soundErrors && !pos3_forward){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }
            
        if (lickmeterState4==aMouse && state_pos4_forward){
            digitalWrite(solPin4, HIGH);    // ON
            digitalWrite(solTTLPin4, HIGH); // ON  
            delay(caudal4);
            digitalWrite(solPin4, LOW);     // OFF
            digitalWrite(solTTLPin4, LOW);  // OFF 
            state_pos4_forward = 0; // deactivate port
            rewardsFound = rewardsFound + 1;} 
        else if (lickmeterState4==aMouse && soundErrors && !pos4_forward){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }
            
        if (lickmeterState5==aMouse && state_pos5_forward){
            digitalWrite(solPin5, HIGH);    // ON
            digitalWrite(solTTLPin5, HIGH); // ON  
            delay(caudal5);
            digitalWrite(solPin5, LOW);     // OFF
            digitalWrite(solTTLPin5, LOW);  // OFF 
            state_pos5_forward = 0; // deactivate port
            rewardsFound = rewardsFound + 1;}
        else if (lickmeterState5==aMouse && soundErrors && !pos5_forward){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }

        if (lickmeterState6==aMouse && state_pos6_forward){
            digitalWrite(solPin6, HIGH);    // ON
            digitalWrite(solTTLPin6, HIGH); // ON  
            delay(caudal6);
            digitalWrite(solPin6, LOW);     // OFF
            digitalWrite(solTTLPin6, LOW);  // OFF 
            state_pos6_forward = 0; // deactivate port
            rewardsFound = rewardsFound + 1;
        } else if (lickmeterState6==aMouse && soundErrors && !pos6_forward){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }

        if (lickmeterState7==aMouse && rewardsFound >= rewardsToFind_FORWAD){ // If reward was found
            digitalWrite(solPin7, HIGH);    // ON
            digitalWrite(solTTLPin7, HIGH); // ON  
            delay(caudal7);
            digitalWrite(solPin7, LOW);     // OFF
            digitalWrite(solTTLPin7, LOW);  // OFF 
            
            state = 2; // activate REVERSE trial
            state_pos2_forward = pos2_forward;
            state_pos3_forward = pos3_forward;
            state_pos4_forward = pos4_forward;
            state_pos5_forward = pos5_forward;
            state_pos6_forward = pos6_forward;
            rewardsFound = 0;
            
            trialNumber = trialNumber + 1;
            digitalWrite(ledPin1, HIGH);    // ON
        } 
      }
  
     if (state == 2){ // if REVERSE trial
       //if (rewardsFound < rewardsToFind_REVERSE){
        if (lickmeterState2==aMouse && state_pos2_reverse){
            digitalWrite(solPin2, HIGH);    // ON
            digitalWrite(solTTLPin2, HIGH); // ON  
            delay(caudal2);
            digitalWrite(solPin2, LOW);     // OFF
            digitalWrite(solTTLPin2, LOW);  // OFF 
            state_pos2_reverse = 0; // deactivate port
            rewardsFound = rewardsFound + 1;}
        else if (lickmeterState2==aMouse && soundErrors && !pos2_reverse){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }

        if (lickmeterState3==aMouse && state_pos3_reverse){
            digitalWrite(solPin3, HIGH);    // ON
            digitalWrite(solTTLPin3, HIGH); // ON  
            delay(caudal3);
            digitalWrite(solPin3, LOW);     // OFF
            digitalWrite(solTTLPin3, LOW);  // OFF 
            state_pos3_reverse = 0; // deactivate port
            rewardsFound = rewardsFound + 1;}
        else if (lickmeterState3==aMouse && soundErrors && !pos3_reverse){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }
            
        if (lickmeterState4==aMouse && state_pos4_reverse){
            digitalWrite(solPin4, HIGH);    // ON
            digitalWrite(solTTLPin4, HIGH); // ON  
            delay(caudal4);
            digitalWrite(solPin4, LOW);     // OFF
            digitalWrite(solTTLPin4, LOW);  // OFF 
            state_pos4_reverse = 0; // deactivate port
            rewardsFound = rewardsFound + 1;} 
        else if (lickmeterState4==aMouse && soundErrors && !pos4_reverse){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }
            
        if (lickmeterState5==aMouse && state_pos5_reverse){
            digitalWrite(solPin5, HIGH);    // ON
            digitalWrite(solTTLPin5, HIGH); // ON  
            delay(caudal5);
            digitalWrite(solPin5, LOW);     // OFF
            digitalWrite(solTTLPin5, LOW);  // OFF 
            state_pos5_reverse = 0; // deactivate port
            rewardsFound = rewardsFound + 1;}
        else if (lickmeterState5==aMouse && soundErrors && !pos5_reverse){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }

        if (lickmeterState6==aMouse && state_pos6_reverse){
            digitalWrite(solPin6, HIGH);    // ON
            digitalWrite(solTTLPin6, HIGH); // ON  
            delay(caudal6);
            digitalWrite(solPin6, LOW);     // OFF
            digitalWrite(solTTLPin6, LOW);  // OFF 
            state_pos6_reverse = 0; // deactivate port
            rewardsFound = rewardsFound + 1;
        } else if (lickmeterState6==aMouse && soundErrors && !pos6_reverse){
              toneAC(soundErrorFreq);
              delay(soundErrorDuration);
              toneAC(0);
        }

        if (lickmeterState1==aMouse && rewardsFound >= rewardsToFind_REVERSE){ // If reward was found
            digitalWrite(solPin7, HIGH);    // ON
            digitalWrite(solTTLPin7, HIGH); // ON  
            delay(caudal7);
            digitalWrite(solPin7, LOW);     // OFF
            digitalWrite(solTTLPin7, LOW);  // OFF 
            
            state = 1; // activate FORWARD trial
            digitalWrite(ledPin1, LOW);    // OFF
            state_pos2_reverse = pos2_reverse;
            state_pos3_reverse = pos3_reverse;
            state_pos4_reverse = pos4_reverse;
            state_pos5_reverse = pos5_reverse;
            state_pos6_reverse = pos6_reverse;
            rewardsFound = 0;
          }
      }
    } else if (phase == 0){
    // release water for all designated ports
      if        (lickmeterState2==aMouse && state_pos2_forward){
        digitalWrite(solPin2, HIGH);    // ON
        digitalWrite(solTTLPin2, HIGH); // ON  
        delay(caudal2);
        digitalWrite(solPin2, LOW);     // OFF
        digitalWrite(solTTLPin2, LOW);  // OFF 
        state_pos2_forward = 0; // deactivate port
        // activate all other ports
        state_pos1_forward = pos1_forward;
        state_pos3_forward = pos3_forward;
        state_pos4_forward = pos4_forward;
        state_pos5_forward = pos5_forward;
        state_pos6_forward = pos6_forward;
        state_pos7_forward = pos7_forward;
        trialNumber = trialNumber + 1;
        
      } else if (lickmeterState1==aMouse && state_pos1_forward){
        digitalWrite(solPin1, HIGH);    // ON
        digitalWrite(solTTLPin1, HIGH); // ON  
        delay(caudal1);
        digitalWrite(solPin1, LOW);     // OFF
        digitalWrite(solTTLPin1, LOW);  // OFF 
        state_pos1_forward = 0; // deactivate port
        // activate all other ports
        state_pos2_forward = pos2_forward;
        state_pos3_forward = pos3_forward;
        state_pos4_forward = pos4_forward;
        state_pos5_forward = pos5_forward;
        state_pos6_forward = pos6_forward;
        state_pos7_forward = pos7_forward;
        trialNumber = trialNumber + 1;
        
      } else if (lickmeterState3==aMouse && state_pos3_forward){
        digitalWrite(solPin3, HIGH);    // ON
        digitalWrite(solTTLPin3, HIGH); // ON  
        delay(caudal3);
        digitalWrite(solPin3, LOW);     // OFF
        digitalWrite(solTTLPin3, LOW);  // OFF 
        state_pos3_forward = 0; // deactivate port
        // activate all other ports
        state_pos2_forward = pos2_forward;
        state_pos1_forward = pos1_forward;
        state_pos4_forward = pos4_forward;
        state_pos5_forward = pos5_forward;
        state_pos6_forward = pos6_forward;
        state_pos7_forward = pos7_forward;
        trialNumber = trialNumber + 1;
        
      } else if (lickmeterState4==aMouse && state_pos4_forward){
        digitalWrite(solPin4, HIGH);    // ON
        digitalWrite(solTTLPin4, HIGH); // ON  
        delay(caudal4);
        digitalWrite(solPin4, LOW);     // OFF
        digitalWrite(solTTLPin4, LOW);  // OFF 
        state_pos4_forward = 0; // deactivate port
        // activate all other ports
        state_pos2_forward = pos2_forward;
        state_pos1_forward = pos1_forward;
        state_pos3_forward = pos3_forward;
        state_pos5_forward = pos5_forward;
        state_pos6_forward = pos6_forward;
        state_pos7_forward = pos7_forward;
        trialNumber = trialNumber + 1;
        
      } else if (lickmeterState5==aMouse && state_pos5_forward){
        digitalWrite(solPin5, HIGH);    // ON
        digitalWrite(solTTLPin5, HIGH); // ON  
        delay(caudal5);
        digitalWrite(solPin5, LOW);     // OFF
        digitalWrite(solTTLPin5, LOW);  // OFF 
        state_pos5_forward = 0; // deactivate port
        // activate all other ports
        state_pos2_forward = pos2_forward;
        state_pos1_forward = pos1_forward;
        state_pos3_forward = pos3_forward;
        state_pos4_forward = pos4_forward;
        state_pos6_forward = pos6_forward;
        state_pos7_forward = pos7_forward;
        trialNumber = trialNumber + 1;
        
      } else if (lickmeterState6==aMouse && state_pos6_forward){
        digitalWrite(solPin6, HIGH);    // ON
        digitalWrite(solTTLPin6, HIGH); // ON  
        delay(caudal6);
        digitalWrite(solPin6, LOW);     // OFF
        digitalWrite(solTTLPin6, LOW);  // OFF 
        state_pos6_forward = 0; // deactivate port
        // activate all other ports
        state_pos2_forward = pos2_forward;
        state_pos1_forward = pos1_forward;
        state_pos3_forward = pos3_forward;
        state_pos4_forward = pos4_forward;
        state_pos5_forward = pos5_forward;
        state_pos7_forward = pos7_forward;
        trialNumber = trialNumber + 1;
        
      } else if (lickmeterState6==aMouse && state_pos7_forward){
        digitalWrite(solPin7, HIGH);    // ON
        digitalWrite(solTTLPin7, HIGH); // ON  
        delay(caudal7);
        digitalWrite(solPin7, LOW);     // OFF
        digitalWrite(solTTLPin7, LOW);  // OFF 
        state_pos7_forward = 0; // deactivate port
        // activate all other ports
        state_pos2_forward = pos2_forward;
        state_pos1_forward = pos1_forward;
        state_pos3_forward = pos3_forward;
        state_pos4_forward = pos4_forward;
        state_pos5_forward = pos5_forward;
        state_pos6_forward = pos6_forward;
        trialNumber = trialNumber + 1;
      }
      
    } else if (phase == 2){
    // do nothing, just tell intan about licking behaviour  
      
    }
}

 void flash(){
  
  Serial.print(timeCounter);
  timeCounter = timeCounter + 2;
  Serial.print("s, total trials: ");
  Serial.println(trialNumber);
} 
