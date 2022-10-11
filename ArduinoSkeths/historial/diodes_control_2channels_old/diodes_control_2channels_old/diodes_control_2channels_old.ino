/* Send a TTL pulse on any of the 2 trigger-in channel of the diodesBoard_2ch for 
/ controlling the diodes, according to some specification */
// settings
int modeOfOperation = 1; // 1 is tagging, 2 is continuous-triggered stimulation with only one pulse duration, 3 is continuous-triggered stimulation with one pulse duration for 2 diodes run independently
int delayBetweenPulsesFactor = 14; // factor that multiply pulses duration to compute delay
int randomness = 6; // factor that multiply pulses duraction to compute randomness
int pulseDuration[] = {5, 20, 50, 100}; // arary of duractions in ms, for example {5 20 50 100}
int nPulsesDuration = 4;
int pulsesNumber = 500;
boolean continousStimulation = true;// stimulation no dependent of activation pin
int pulseChoiceForStimulation = 2; // array position of the pulse that will be use during triggered stimulation
int shortFactor = 3; // if pulses less than 10ms, multiply delayBetweenPulsesFactor by this
int runChirp = 20; // run chirp protocol. Scalar will indicate number of repetition per pulse.

// Pins
const int diodesPinA = 2;
const int diodesPinB = 3;
const int activationPin = 12; // From the control behaviour arduino

// state variables
int stimBlock = 0; // 0 is no stimulating
int delayBetweenPulsesA;
int delayBetweenPulsesB;
int pulse;
int pulsesCounter;
int delayFrom;
int delayTo;
int activeShortFactor;
int chirpDuration;
unsigned long previousMillisA = 0;        // will store last time diodeA was active
unsigned long previousMillisB = 0;        // will store last time diodeB was active
int diodeAState = LOW;                    // diode A state used to set the LED
int diodeBState = LOW;                    // diode B used to set the LED
int increasedPeriodFactor;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(diodesPinA, OUTPUT);
  pinMode(diodesPinB, OUTPUT);
  pinMode(activationPin, INPUT);
  randomSeed(analogRead(A0));
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   

  // compute pulse features
  pulse = pulseDuration[pulseChoiceForStimulation];
  delayFrom = (delayBetweenPulsesFactor - randomness) * pulse;
  delayTo = (delayBetweenPulsesFactor + randomness) * pulse;
}

void loop() {
  if (modeOfOperation == 1){
    if (digitalRead(activationPin) == HIGH | continousStimulation){ // if activated  or Continous Stimulation mode
      for (int i = 0; i < nPulsesDuration; ++i){
        digitalWrite(diodesPinA, LOW);   // turn the diodes off
        digitalWrite(diodesPinB, LOW);   // turn the diodes off
        delay(1000);
        pulse = pulseDuration[i];

        if (pulse < 10){ // if pulses less than 10ms, multiply delayBetweenPulsesFactor by 3
          activeShortFactor = shortFactor;
          } else {
          activeShortFactor = 1; 
          }
        
        pulsesCounter = 0;
        delayFrom = (delayBetweenPulsesFactor - randomness) * pulse * activeShortFactor;
        delayTo = (delayBetweenPulsesFactor + randomness) * pulse * activeShortFactor;
          while (pulsesCounter < pulsesNumber){
            // Use millis() instead of delay
            unsigned long currentMillis = millis();

            if((diodeAState == HIGH) && (currentMillis - previousMillisA >= pulse))
            {
              diodeAState = LOW;  // Turn it off
              previousMillisA = currentMillis;  // Remember the time
              digitalWrite(diodesPinA, diodeAState);  // Update the actual LED
              delayBetweenPulsesA = random(delayFrom,delayTo); // random number from 1 to 12
            }
            else if ((diodeAState == LOW) && (currentMillis - previousMillisA >= delayBetweenPulsesA))
            {
              diodeAState = HIGH;  // turn it on
              previousMillisA = currentMillis;   // Remember the time
              digitalWrite(diodesPinA, diodeAState);    // Update the actual LED
              pulsesCounter++;
            }
            else if((diodeBState == HIGH) && (currentMillis - previousMillisB >= pulse))
            {
              diodeBState = LOW;  // Turn it off
              previousMillisB = currentMillis;  // Remember the time
              digitalWrite(diodesPinB, diodeBState);  // Update the actual LED
              delayBetweenPulsesB = random(delayFrom,delayTo); // random number from 1 to 12
            }
            else if ((diodeBState == LOW) && (currentMillis - previousMillisB >= delayBetweenPulsesB))
            {
              diodeBState = HIGH;  // turn it on
              previousMillisB = currentMillis;   // Remember the time
              digitalWrite(diodesPinB, diodeBState);    // Update the actual LED
            }
          }
      }
      // chirp A
      delay(2000);
      if (runChirp > 0){
        while (chirpDuration < 500){
          chirpDuration = chirpDuration + 5;
          for (int j = 0; j < runChirp; j++){
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
            digitalWrite(diodesPinA, HIGH);   // turn the diodes on

            delay(chirpDuration);

            digitalWrite(LED_BUILTIN, LOW);   // turn the LED on   
            digitalWrite(diodesPinA, LOW);   // turn the diodes on

            delay(chirpDuration);
          }
        }
      }
      // chirp b
      delay(2000);
      if (runChirp > 0){
        while (chirpDuration < 500){
          chirpDuration = chirpDuration + 5;
          for (int j = 0; j < runChirp; j++){
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
            digitalWrite(diodesPinB, HIGH);   // turn the diodes on

            delay(chirpDuration);

            digitalWrite(LED_BUILTIN, LOW);   // turn the LED on   
            digitalWrite(diodesPinB, LOW);   // turn the diodes on

            delay(chirpDuration);
          }
        }
      }
      // chirp A and B
      delay(2000);
      if (runChirp > 0){
        while (chirpDuration < 500){
          chirpDuration = chirpDuration + 5;
          for (int j = 0; j < runChirp; j++){
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
            digitalWrite(diodesPinA, HIGH);   // turn the diodes on
            digitalWrite(diodesPinB, HIGH);   // turn the diodes on

            delay(chirpDuration);

            digitalWrite(LED_BUILTIN, LOW);   // turn the LED on   
            digitalWrite(diodesPinA, LOW);   // turn the diodes on
            digitalWrite(diodesPinB, LOW);   // turn the diodes on

            delay(chirpDuration);
          }
        }
      }

      modeOfOperation = 3;
    }
  } else if (modeOfOperation == 3){
      increasedPeriodFactor = 1;
      pulse = pulseDuration[pulseChoiceForStimulation];
      delayFrom = (delayBetweenPulsesFactor - randomness) * pulse * increasedPeriodFactor;
      delayTo = (delayBetweenPulsesFactor + randomness) * pulse * increasedPeriodFactor;
      digitalWrite(diodesPinA, LOW);   // turn the diodes off
      digitalWrite(diodesPinB, LOW);   // turn the diodes off
      
      if (digitalRead(activationPin) == HIGH) { // if activated by other Arduino
        unsigned long currentMillis = millis();
          if((diodeAState == HIGH) && (currentMillis - previousMillisA >= pulse))
              {
                diodeAState = LOW;  // Turn it off
                previousMillisA = currentMillis;  // Remember the time
                digitalWrite(diodesPinA, diodeAState);  // Update the actual LED
                delayBetweenPulsesA = random(delayFrom,delayTo); // random number from 1 to 12
              }
              else if ((diodeAState == LOW) && (currentMillis - previousMillisA >= delayBetweenPulsesA))
              {
                diodeAState = HIGH;  // turn it on
                previousMillisA = currentMillis;   // Remember the time
                digitalWrite(diodesPinA, diodeAState);    // Update the actual LED
                pulsesCounter++;
              }
              else if((diodeBState == HIGH) && (currentMillis - previousMillisB >= pulse))
              {
                diodeBState = LOW;  // Turn it off
                previousMillisB = currentMillis;  // Remember the time
                digitalWrite(diodesPinB, diodeBState);  // Update the actual LED
                delayBetweenPulsesB = random(delayFrom,delayTo); // random number from 1 to 12
              }
              else if ((diodeBState == LOW) && (currentMillis - previousMillisB >= delayBetweenPulsesB))
              {
                diodeBState = HIGH;  // turn it on
                previousMillisB = currentMillis;   // Remember the time
                digitalWrite(diodesPinB, diodeBState);    // Update the actual LED
              }
          }
        modeOfOperation = 4;
  } else if (modeOfOperation == 2){
    if (digitalRead(activationPin) == HIGH){ // if activated
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
      digitalWrite(diodesPinA, HIGH);   // turn the diodes on

      delay(pulse);

      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
      digitalWrite(diodesPinA, LOW);   // turn the diodes off

      delayBetweenPulsesA = random(delayFrom,delayTo); // random number from 1 to 12
      delay(delayBetweenPulsesA);
    }
  }
}
