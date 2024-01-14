/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-29
 */ 

#include "DCMotor_TB6612FNG.h"

DCMotor_TB6612FNG::DCMotor_TB6612FNG(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinPWM, uint8_t pinSTBY) : DCMotorAbstract() {
  _pinIN1 = pinIN1;
  _pinIN2 = pinIN2;
  _pinPWM = pinPWM;
  _pinSTBY = pinSTBY;

  // Initialize output pins
  pinMode(_pinIN1, OUTPUT);
  pinMode(_pinIN2, OUTPUT);
  pinMode(_pinPWM, OUTPUT);
  pinMode(_pinSTBY, OUTPUT);

  UpdateOutputValue();
  SetStandbySignal(true);
}

DCMotor_TB6612FNG::~DCMotor_TB6612FNG(){
  
}

void DCMotor_TB6612FNG::SetStandbySignal(bool val){
    digitalWrite(_pinSTBY, val);
}

void DCMotor_TB6612FNG::UpdateOutputValue(){

  // Set motor directionality
  bool isRotationForward = _currSpeedPercent > 0;
  digitalWrite(_pinIN1, isRotationForward);
  digitalWrite(_pinIN2, not isRotationForward);

  // Set motor speed
  uint8_t currOutputValue = int(abs(_currSpeedPercent) * _maxOutputValue);
  analogWrite(_pinPWM, currOutputValue);
}
