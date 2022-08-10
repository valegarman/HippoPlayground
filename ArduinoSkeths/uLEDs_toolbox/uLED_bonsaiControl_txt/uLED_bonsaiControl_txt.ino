
/* Send a TTL pulse on any of the 1-12 trigger-in channel of the OSCLite 1/ NeuroLight Stimulator System (Plexon)  for 
/ controlling the uLED, according to some specification

HARDWARE: Arduino (DUE, but any would work) DIGITAL OUT connected to DIGITAL IN of the Stimulator, according to 
the following mapping:
 ARDUINO  |  SIMULATOR
 ---------------------
    34          1
    32          2
    36          3
    30          4
    38          5
    28          6
    40          7
    26          8
    42          9
    24         10
    44         11
    22         12
----------------------

Manuel Valero 2022
Palbo Abad 2022 (Bonsai interfacing)
*/

// settings
int delayBetweenPulsesFrom = 20; // in ms, first value on range of for random pulses delay, for example 20
int delayBetweenPulsesTo = 40; // in ms, last value on range of for random pulses delay, for example 40
int pulseDuration = 20; // in ms, for example 20
boolean continousStimulation = true;// stimulation no dependent of the behavoiur arduino
boolean stimulateShank1 = true;
boolean stimulateShank2 = true;
boolean stimulateShank3 = true;
boolean stimulateShank4 = true; 

int state = 0;
int value = 0;

// ASCII code:
int stimOn_ascii = 105;
int stimOff_ascii  = 115;

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

const int BlockPin = 53; // From the control behaviour arduino

// state variables
int stimBlock = 0; // 0 is no stimulating
int uled_channel = 0;
int delayBetweenPulses;

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

  delayBetweenPulsesTo = delayBetweenPulsesTo + 1;
  randomSeed(analogRead(A0));   
}

void loop() {
  if (Serial.available() > 0){
    value = Serial.read();
  if (value == stimOn_ascii){
    state = 1;
    }else if (value == stimOff_ascii){
      state = 0;
    }
  }
  if (state == 1){
      digitalWrite(LED_BUILTIN,HIGH);
  if (digitalRead(BlockPin) == HIGH | continousStimulation){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
    uled_channel = random(1,13); // random number from 1 to 12
    if (uled_channel == 1 & stimulateShank1 == true){
      digitalWrite(s1l1Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(1); delay(pulseDuration); digitalWrite(s1l1Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 2 & stimulateShank1 == true){
      digitalWrite(s1l2Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(2); delay(pulseDuration); digitalWrite(s1l2Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 3 & stimulateShank1 == true){
      digitalWrite(s1l3Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(3); delay(pulseDuration); digitalWrite(s1l3Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 4 & stimulateShank2 == true){
      digitalWrite(s2l1Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(4); delay(pulseDuration); digitalWrite(s2l1Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 5 & stimulateShank2 == true){
      digitalWrite(s2l2Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(5); delay(pulseDuration); digitalWrite(s2l2Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 6 & stimulateShank2 == true){ 
      digitalWrite(s2l3Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(6); delay(pulseDuration); digitalWrite(s2l3Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 7 & stimulateShank3 == true){
      digitalWrite(s3l1Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(7); delay(pulseDuration); digitalWrite(s3l1Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 8 & stimulateShank3 == true){
      digitalWrite(s3l2Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(8); delay(pulseDuration); digitalWrite(s3l2Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 9 & stimulateShank3 == true){
      digitalWrite(s3l3Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(9); delay(pulseDuration); digitalWrite(s3l3Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 10 & stimulateShank4 == true){
      digitalWrite(s4l1Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(10); delay(pulseDuration); digitalWrite(s4l1Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 11 & stimulateShank4 == true){
      digitalWrite(s4l2Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(11); delay(pulseDuration); digitalWrite(s4l2Pin, LOW); digitalWrite(ttlPin,LOW);
      } else if (uled_channel == 12 & stimulateShank4 == true){
      digitalWrite(s4l3Pin, HIGH); digitalWrite(ttlPin,HIGH); Serial.println(12); delay(pulseDuration); digitalWrite(s4l3Pin, LOW); digitalWrite(ttlPin,LOW);
      }
    delayBetweenPulses = random(delayBetweenPulsesFrom,delayBetweenPulsesTo); // random number from 1 to 12
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on   
    delay(delayBetweenPulses);
      
    } else {
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
    }
  }else if (state == 0){
    digitalWrite(LED_BUILTIN,LOW);
    }
}
