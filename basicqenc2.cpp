
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
#define bodyEncoderLeftIntA 0
#define bodyEncoderLeftIntB 1
#define bodyEncoderLeftFunctionA bodyEncoderLeftCounterA
#define bodyEncoderLeftFunctionB bodyEncoderLeftCounterB
#define bodyEncoderLeftPinA 2 // A pin the interrupt pin
#define bodyEncoderLeftPinB 3 // B pin the interrupt pin

#define bodyEncoderLeftSignal bodyEncoderSignal
#define bodyEncoderLeftInputSignal bodyEncoderInputSignal

// encoder the rotation direction
boolean bodyEncoderLeftDirection; 
byte bodyEncoderLeftPinLast;

// encoder pulses
volatile signed long bodyEncoderLeftTotalPulses = 0;


void bodyEncoderLeftCounterA() {

  // look for a low-to-high on channel A
  if (digitalRead(bodyEncoderLeftPinA) == HIGH) {
    // check channel B to see which way encoder is turning
    if (digitalRead(bodyEncoderLeftPinB) == LOW) {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses - 1;         // CW
    } else {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses + 1;         // CCW
    }
  } else {
    // its low-to-high-to-low on channel A
    // check channel B to see which way encoder is turning
    if (digitalRead(bodyEncoderLeftPinB) == HIGH) {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses - 1;          // CW
    } else {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses + 1;          // CCW
    }
  }
}


void bodyEncoderLeftCounterB() {

  // look for a low-to-high on channel B
  if (digitalRead(bodyEncoderLeftPinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(bodyEncoderLeftPinA) == HIGH) {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses - 1;         // CW
    }
    else {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses + 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B
  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(bodyEncoderLeftPinA) == LOW) {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses - 1;          // CW
    }
    else {
      bodyEncoderLeftTotalPulses = bodyEncoderLeftTotalPulses + 1;          // CCW
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize LED digital pin as an output.
  pinMode(LED_PIN, OUTPUT);

  pinMode(bodyEncoderLeftPinA,INPUT_PULLUP);
  pinMode(bodyEncoderLeftPinB,INPUT_PULLUP);

  bodyEncoderLeftDirection = true; //default -> Forward 
  attachInterrupt(bodyEncoderLeftIntA, bodyEncoderLeftFunctionA, bodyEncoderLeftSignal);
  attachInterrupt(bodyEncoderLeftIntB, bodyEncoderLeftFunctionB, bodyEncoderLeftSignal);
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

