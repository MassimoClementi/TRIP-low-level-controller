/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "DCMotorAFShield.h"

DCMotorAFShield::DCMotorAFShield(int DCMotorID) : DCMotorAbstract() {
  _DCMotorID = DCMotorID;

  _motor = new AF_DCMotor(_DCMotorID);

  UpdateOutputValue();
}

DCMotorAFShield::~DCMotorAFShield(){
  
}

void DCMotorAFShield::UpdateOutputValue(){

  // Set motor directionality
  bool isRotationForward = _currSpeedPercent > 0;
  if(isRotationForward)
  {
    _motor->run(FORWARD);
  } 
  else
  {
    _motor->run(BACKWARD);
  }

  // Set motor speed
  int currOutputValue = int(abs(_currSpeedPercent) * _maxOutputValue);
  _motor->setSpeed(currOutputValue);
}
