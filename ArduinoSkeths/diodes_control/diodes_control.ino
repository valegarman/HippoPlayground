/* Send a TTL pulse on any of the 1-12 trigger-in channel of the OSCLite 1 for 
/ controlling the uLED, according to some specification */
// settings
int modeOfOperation = 1; // 1 is tagging, 2 is continuous-triggered stimulation with only one pulse duration
int delayBetweenPulsesFactor = 14; // factor that multiply pulses duration to compute delay
int randomness = 6; // factor that multiply pulses duraction to compute randomness
int pulseDuration[] = {5, 20, 50, 100}; // arary of duractions in ms, for example {5 20 50 100}
int nPulsesDuration = 4;
int pulsesNumber = 500;
boolean continousStimulation = true;// stimulation no dependent of activation pin
int pulseChoiceForStimulation = 1; // array position of the pulse that will be use during triggered stimulation

// Pins
const int diodesPin = 2;
const int activationPin = 12; // From the control behaviour arduino

// state variables
int stimBlock = 0; // 0 is no stimulating
int delayBetweenPulses;
int pulse;
int pulsesCounter;
int delayFrom;
int delayTo;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(diodesPin, OUTPUT);
  pinMode(activationPin, INPUT);
  randomSeed(analogRead(A0));
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
  digitalWrite(diodesPin, LOW);   // turn the diodes off

  // compute pulse features
  pulse = pulseDuration[pulseChoiceForStimulation];
  delayFrom = (delayBetweenPulsesFactor - randomness) * pulse;
  delayTo = (delayBetweenPulsesFactor + randomness) * pulse;
}

void loop() {
  if (modeOfOperation == 1){
    if (digitalRead(activationPin) == HIGH | continousStimulation){ // if activated  or Continous Stimulation mode
      for (int i = 0; i < nPulsesDuration; ++i){
        delay(1000);
        pulse = pulseDuration[i];
        pulsesCounter = 0;
        delayFrom = (delayBetweenPulsesFactor - randomness) * pulse;
        delayTo = (delayBetweenPulsesFactor + randomness) * pulse;
          while (pulsesCounter < pulsesNumber){
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
            digitalWrite(diodesPin, HIGH);   // turn the diodes on
  
            delay(pulse);
  
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
            digitalWrite(diodesPin, LOW);   // turn the diodes off         
  
            delayBetweenPulses = random(delayFrom,delayTo); // random number from 1 to 12
            delay(delayBetweenPulses);
  
            pulsesCounter++;
            }
        }
      modeOfOperation = 3;
    }
  } else if (modeOfOperation == 2){
    if (digitalRead(activationPin) == HIGH){ // if activated
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
      digitalWrite(diodesPin, HIGH);   // turn the diodes on

      delay(pulse);

      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
      digitalWrite(diodesPin, LOW);   // turn the diodes off

      delayBetweenPulses = random(delayFrom,delayTo); // random number from 1 to 12
      delay(delayBetweenPulses);
    }
  }
}
