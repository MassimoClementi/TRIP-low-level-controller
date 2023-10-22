/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "DCMotor.h"

DCMotor::DCMotor(int DCMotorID) : DCMotorAbstract() {
  _DCMotorID = DCMotorID;

  _motor = new AF_DCMotor(_DCMotorID);

  UpdateOutputValue();
}

DCMotor::~DCMotor(){
  
}

void DCMotor::UpdateOutputValue(){

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
