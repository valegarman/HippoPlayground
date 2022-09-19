
/* Co-activation of up to four group of uLEDs.
 
 User should specificy first up to 4 groups of LEDs according to the following scheme:
 *  
 *   | 3|  | 6|  | 9|  |12| 
 *   | 2|  | 5|  | 8|  |11|
 *   | 1|  | 4|  | 7|  |10|
 *   \  /  \  /  \  /  \  /
 *   

 Then define the co-activation epochs (number of pulses and inter pulse interval), for example 6 pulses, 8.3 ms appart,
 for a ripple like co-activation).
        Epoch of 6 pulses
  _________|_|_|_|_|_|_____________


And then, define the range (min-max) of the inter_epochs interval.
 
                 Epochs    Inter_Epochs
LED A1 _________|_|_|_|_|_________________________________________________
LED A2 _________|_|_|_|_|_________________________________________________

LED B1 ______________________________|_|_|_|_|____________________________
LED B2 ______________________________|_|_|_|_|____________________________

The different groups of LEDs will be selected and activated in a random fashion, with a random inter-epoch interval 
(between the defined max and min).

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

int group1_lights[] = {1, 0, 0,   0, 0, 0,   0, 1, 0,   0, 0, 0}; // 1 and 8

int group2_lights[] = {0, 0, 0,   1, 1, 1,   0, 0, 0,   1, 1, 1}; // 4, 5, 6, 10, 11, 12

int group3_lights[] = {0, 0, 0,   1, 1, 1,   0, 0, 0,   1, 1, 1}; // 1 and 8

int group4_lights[] = {0, 0, 0,   0, 0, 0,   0, 0, 0,   0, 0, 0}; // 1 and 8

int pulses_per_epoch = 6;

int epoch_inter_event_interval = 4150; // in microseconds, half of a ripple cycle, 8.300 ms/1000/2 = 4150

int delayBetweenEpochsFrom =  200;     // in ms, first value on range of for random epochs delay, for example 200
int delayBetweenEpochsTo   = 1000;     // in ms, last value on range of for random pulses delay, for example 1000
int pulseDuration = 4150; //           // in microseconds, half of a ripple cycle, 8.300 ms/1000/2 = 4150

boolean continousStimulation = true;// stimulation no dependent of the behavoiur arduino


// Pins
const int s1l1Pin = 34; // #1 txt
const int s1l2Pin = 32; // #2 txt
const int s1l3Pin = 36; // #3 txt

const int s2l1Pin = 30; // #4 txt
const int s2l2Pin = 38; // #5 txt
const int s2l3Pin = 28; // #6 txt

const int s3l1Pin = 40; // #7 txt
const int s3l2Pin = 26; // #8 txt
const int s3l3Pin = 42; // #9 txt

const int s4l1Pin = 24; // #10 txt
const int s4l2Pin = 44; // #11 txt
const int s4l3Pin = 22; // #12 txt

const int ttlPin = 10;

const int BlockPin = 53;// From the control behaviour arduino

// state variables
int stimBlock = 0;      // 0 is no stimulating
int ledGroup = 0;
int delayBetweenEpochs;
int value = 0;

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

  pinMode(ttlPin,OUTPUT);

  pinMode(BlockPin, INPUT);

  delayBetweenEpochs = delayBetweenEpochs + 1;
  randomSeed(analogRead(A0));   
}

void loop() {

  if (digitalRead(BlockPin) == HIGH | continousStimulation){

    ledGroup = random(1,3); // random number from 1 to 3, selecting group
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   

    if        (ledGroup==1){
      runEpochs(group1_lights[0],group1_lights[1],group1_lights[2],group1_lights[3],group1_lights[4],group1_lights[5],group1_lights[6],group1_lights[7],group1_lights[8],group1_lights[9],group1_lights[10],group1_lights[11],epoch_inter_event_interval,pulseDuration,pulses_per_epoch);
    } else if (ledGroup==2){
      runEpochs(group2_lights[0],group2_lights[1],group2_lights[2],group2_lights[3],group2_lights[4],group2_lights[5],group2_lights[6],group2_lights[7],group2_lights[8],group2_lights[9],group2_lights[10],group2_lights[11],epoch_inter_event_interval,pulseDuration,pulses_per_epoch);
    } else if (ledGroup==3){
      runEpochs(group3_lights[0],group3_lights[1],group3_lights[2],group3_lights[3],group3_lights[4],group3_lights[5],group3_lights[6],group3_lights[7],group3_lights[8],group3_lights[9],group3_lights[10],group3_lights[11],epoch_inter_event_interval,pulseDuration,pulses_per_epoch);
    } else if (ledGroup==4){
      runEpochs(group4_lights[0],group4_lights[1],group4_lights[2],group4_lights[3],group4_lights[4],group4_lights[5],group4_lights[6],group4_lights[7],group4_lights[8],group4_lights[9],group4_lights[10],group4_lights[11],epoch_inter_event_interval,pulseDuration,pulses_per_epoch);
    }
    
    delayBetweenEpochs = random(delayBetweenEpochsFrom,delayBetweenEpochsTo); // random number from 1 to 12
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on
    delay(delayBetweenEpochs);
      
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
