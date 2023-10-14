/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "DCMotorAbstract.h"

DCMotorAbstract::DCMotorAbstract(){
  _currSpeedPercent = 0.0;
  _rotationDirection = FORWARD;
}

DCMotorAbstract::~DCMotorAbstract(){
  
}

void DCMotorAbstract::SetSpeedPercent(const double speedPercent){
  // Limit percentage value between 0.0 and 1.0
  _currSpeedPercent = min(max(0.0, speedPercent), 1.0);
  UpdateOutputValue();
}

void DCMotorAbstract::SetRotationDirection(const RotationDirection rotationDirection){
  _rotationDirection = rotationDirection;
  UpdateOutputValue();
}

double DCMotorAbstract::GetSpeedPercent(){
  return _currSpeedPercent;
}
