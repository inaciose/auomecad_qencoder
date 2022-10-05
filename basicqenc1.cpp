#include <Arduino.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN PC13
#endif

#define LED_PIN LED_BUILTIN
#define LED_TIME 1000
unsigned long ledTimer = 0;
bool blinkState = false;

#define bodyEncoderSignal CHANGE
#define bodyEncoderInputSignal INPUT_PULLUP

// encoder LeftMotor
#define bodyEncoderLeftInt 0
#define bodyEncoderLeftFunction bodyEncoderLeftCounter
#define bodyEncoderLeftPin 2 // A pin the interrupt pin
#define bodyEncoderLeftPinB 7 // B pin: the digital pin

#define bodyEncoderLeftSignal bodyEncoderSignal
#define bodyEncoderLeftInputSignal bodyEncoderInputSignal

// encoder the rotation direction
boolean bodyEncoderLeftDirection; 
byte bodyEncoderLeftPinLast;

// encoder pulses
volatile signed long bodyEncoderLeftTotalPulses = 0;


void bodyEncoderLeftCounter() {
  int Lstate = digitalRead(bodyEncoderLeftPin);
  if((bodyEncoderLeftPinLast == LOW) && Lstate==HIGH) {
    int val = digitalRead(bodyEncoderLeftPinB);
    if(val == LOW && bodyEncoderLeftDirection) {
      bodyEncoderLeftDirection = false; //Reverse
    } else if(val == HIGH && !bodyEncoderLeftDirection) {
      bodyEncoderLeftDirection = true; //Forward
    }
  }
  bodyEncoderLeftPinLast = Lstate;

  if(!bodyEncoderLeftDirection) {
    bodyEncoderLeftTotalPulses++;
  } else {
    bodyEncoderLeftTotalPulses--;
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize LED digital pin as an output.
  pinMode(LED_PIN, OUTPUT);


  pinMode(bodyEncoderLeftPin,INPUT_PULLUP);
  pinMode(bodyEncoderLeftPinB,INPUT_PULLUP);

  bodyEncoderLeftDirection = true; //default -> Forward 
  pinMode(bodyEncoderLeftPinB, bodyEncoderLeftInputSignal);
  attachInterrupt(bodyEncoderLeftInt, bodyEncoderLeftFunction, bodyEncoderLeftSignal);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(millis() > ledTimer) {
    ledTimer = millis() + LED_TIME;
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    Serial.println(bodyEncoderLeftTotalPulses);
  }

}
