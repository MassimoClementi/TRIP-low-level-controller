/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "DCMotor.h"

DCMotor::DCMotor(int pinNumberHBridgeA, int pinNumberHBridgeB, int pinNumberControl) : DCMotorAbstract() {
  _pinNumberHBridgeA = pinNumberHBridgeA;
  _pinNumberHBridgeB = pinNumberHBridgeB;
  _pinNumberControl = pinNumberControl;

  // Configure pins as outputs
  pinMode(_pinNumberHBridgeA, OUTPUT);
  pinMode(_pinNumberHBridgeB, OUTPUT);
  pinMode(_pinNumberControl, OUTPUT);

  UpdateOutputValue();
}

DCMotor::~DCMotor(){
  
}

void DCMotor::UpdateOutputValue(){

  // Set motor directionality
  bool isRotationForward = _rotationDirection == FORWARD;
  digitalWrite(_pinNumberHBridgeA, isRotationForward);
  digitalWrite(_pinNumberHBridgeB, not isRotationForward);

  // Set motor speed
  int currOutputValue = int(_currSpeedPercent * _maxOutputValue);
  analogWrite(_pinNumberControl, currOutputValue);
}
