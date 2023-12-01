/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-29
 */ 

#include "ControllerPID.h"

ControllerPID::ControllerPID(double kp, double ki, double kd) : ControllerAbstract(){
  _kp = kp;
  _ki = ki;
  _kd = kd;

  _myPID = new PID(&_measuredOutput, &_controlInput, &_referenceInput, _kp, _ki, _kd, DIRECT);
  _myPID->SetMode(AUTOMATIC);
}

ControllerPID::~ControllerPID(){
  
}

void ControllerPID::ComputeControlInput(double controlError){
  _myPID->Compute();
}
