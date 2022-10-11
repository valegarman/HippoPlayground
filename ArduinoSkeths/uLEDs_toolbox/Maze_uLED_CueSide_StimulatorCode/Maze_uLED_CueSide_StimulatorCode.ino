
/* Co-activation of up to four group of uLEDs based on inputs
 
 User should specificy first up to 2 groups of LEDs according to the following scheme:
 *  
 *   | 3|  | 6|  | 9|  |12| 
 *   | 2|  | 5|  | 8|  |11|
 *   | 1|  | 4|  | 7|  |10|
 *   \  /  \  /  \  /  \  /
 *   
 - sidePin: associates HIGH to groups 1 and LOW to groups 2.

 Then define inter pulse interval).
        Epoch of 6 pulses
  _|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|__

And different groups of LEDs will be selected and activated based on two inputs.
 - sidePin: indicates one or the other choice (tipically go left or right)
 - stimPin: indicates stimulation time.
 Pulses will be delivered when stimPin is activated based on the LED groups defined.

HARDWARE: Arduino (DUE, but any would work) DIGITAL OUT connected to DIGITAL IN of the Stimulator, according to 
the following mapping:
 ARDUINO  |  SIMULATOR  |  SCHME
 ---------------------------------------------------
    34          1          {1, 0, 0,   0, 0, 0,   0, 0, 0,   0, 0, 0}; 
    32          2          {0, 1, 0,   0, 0, 0,   0, 0, 0,   0, 0, 0}; 
    36          3          {0, 0, 1,   0, 0, 0,   0, 0, 0,   0, 0, 0}; 
    30          4          {0, 0, 0,   1, 0, 0,   0, 0, 0,   0, 0, 0}; 
    38          5          {0, 0, 0,   0, 1, 0,   0, 0, 0,   0, 0, 0}; 
    28          6          {0, 0, 0,   0, 0, 1,   0, 0, 0,   0, 0, 0}; 
    40          7          etc
    26          8
    42          9
    24         10
    44         11
    22         12
----------------------

Manuel Valero 2022
*/

// settings
int numberOfGroups = 2; // declare number of groups that will be use

int group1_lights[] = {1, 1, 1,   0, 0, 0,   0, 0, 0,   0, 0, 0}; // 

int group2_lights[] = {0, 0, 0,   0, 0, 0,   0, 0, 0,   1, 1, 1}; // 

int epoch_inter_event_interval = 9400; // in microseconds, 18.8ms (53.19Hz, gamma) x1000 = 18800

int pulseDuration = 9400; //

int maxStimEpoch = 5000; //           // in milliseconds

int pulses_per_epoch = 1; //

// Pins
const int s1l1Pin = 34; // 
const int s1l2Pin = 32; // 
const int s1l3Pin = 36; // 

const int s2l1Pin = 30; // 
const int s2l2Pin = 38; // 
const int s2l3Pin = 28; // 

const int s3l1Pin = 40; // 
const int s3l2Pin = 26; // 
const int s3l3Pin = 42; // 

const int s4l1Pin = 24; // 
const int s4l2Pin = 44; // 
const int s4l3Pin = 22; // 

const int stimPin = 50;// From the control behaviour arduino
const int sidePin = 51;// From the control behaviour arduino

// state variables
int stimBlock = 0;      // 0 is no stimulating
int ledGroup = 0;
int delayBetweenEpochs;
int value = 0;
bool waitingForStim = true;
int startStim;
int now;
bool doStimulate = false;

void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(s1l1Pin, OUTPUT);
  pinMode(s1l2Pin, OUTPUT);
  pinMode(s1l3Pin, OUTPUT);
  pinMode(s2l1Pin, OUTPUT);
  pinMode(s2l2Pin, OUTPUT);
  pinMode(s2l3Pin, OUTPUT);
  pinMode(s3l1Pin, OUTPUT);
  pinMode(s3l2Pin, OUTPUT);
  pinMode(s3l3Pin, OUTPUT);
  pinMode(s4l1Pin, OUTPUT);
  pinMode(s4l2Pin, OUTPUT);
  pinMode(s4l3Pin, OUTPUT);

  pinMode(stimPin, INPUT);
  pinMode(sidePin, INPUT);

  randomSeed(analogRead(A0));   

  Serial.begin(9600); //Start serial communication boud rate at 9600
  Serial.flush();
}

void loop() {
  
  now = millis(); 
  if (waitingForStim == true && digitalRead(stimPin) == HIGH){
    startStim = now;
    waitingForStim = false;
  } else if (digitalRead(stimPin) == LOW){
    waitingForStim = true;
  }
  
  if (now <= startStim + maxStimEpoch){
    doStimulate = true;
  } else {
    doStimulate = false;
  }
  
  if        (doStimulate && digitalRead(sidePin) == LOW){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
    runEpochs(group1_lights[0],group1_lights[1],group1_lights[2],group1_lights[3],group1_lights[4],group1_lights[5],group1_lights[6],group1_lights[7],group1_lights[8],group1_lights[9],group1_lights[10],group1_lights[11],epoch_inter_event_interval,pulseDuration,pulses_per_epoch);
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on
    
  } else if (doStimulate && digitalRead(sidePin) == HIGH){  
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
    runEpochs(group2_lights[0],group2_lights[1],group2_lights[2],group2_lights[3],group2_lights[4],group2_lights[5],group2_lights[6],group2_lights[7],group2_lights[8],group2_lights[9],group2_lights[10],group2_lights[11],epoch_inter_event_interval,pulseDuration,pulses_per_epoch);
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on
      
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
  }
}

void runEpochs(int light1, int light2, int light3, int light4, int light5, int light6, int light7, int light8, int light9, int light10, int light11, int light12, int epoch_inter_event_interval, int pulseDuration, int pulses_per_epoch){
    for (int j = 0; j < pulses_per_epoch; j++){
      if (light1 == 1){
      digitalWrite(s1l1Pin, HIGH);}
      if (light2 == 1){
      digitalWrite(s1l2Pin, HIGH);}
      if (light3 == 1){
      digitalWrite(s1l3Pin, HIGH);}
      if (light4 == 1){
      digitalWrite(s2l1Pin, HIGH);}
      if (light5 == 1){
      digitalWrite(s2l2Pin, HIGH);}
      if (light6 == 1){ 
      digitalWrite(s2l3Pin, HIGH);}
      if (light7 == 1){
      digitalWrite(s3l1Pin, HIGH);}
      if (light8 == 1){
      digitalWrite(s3l2Pin, HIGH);}
      if (light9 == 1){
      digitalWrite(s3l3Pin, HIGH);}
      if (light10 ==1){
      digitalWrite(s4l1Pin, HIGH);}
      if (light11 ==1){
      digitalWrite(s4l2Pin, HIGH);}
      if (light12 ==1){
      digitalWrite(s4l3Pin, HIGH);}

      delayMicroseconds(pulseDuration);

      if (light1 == 1){
      digitalWrite(s1l1Pin, LOW);}
      if (light2 == 1){
      digitalWrite(s1l2Pin, LOW);}
      if (light3 == 1){
      digitalWrite(s1l3Pin, LOW);}
      if (light4 == 1){
      digitalWrite(s2l1Pin, LOW);}
      if (light5 == 1){
      digitalWrite(s2l2Pin, LOW);}
      if (light6 == 1){ 
      digitalWrite(s2l3Pin, LOW);}
      if (light7 == 1){
      digitalWrite(s3l1Pin, LOW);}
      if (light8 == 1){
      digitalWrite(s3l2Pin, LOW);}
      if (light9 == 1){
      digitalWrite(s3l3Pin, LOW);}
      if (light10 ==1){
      digitalWrite(s4l1Pin, LOW);}
      if (light11 ==1){
      digitalWrite(s4l2Pin, LOW);}
      if (light12 ==1){
      digitalWrite(s4l3Pin, LOW);}
      
      delayMicroseconds(epoch_inter_event_interval);
    }
  }
