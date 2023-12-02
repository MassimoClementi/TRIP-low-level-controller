/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-29
 */ 

#include "ControllerPID.h"

ControllerPID::ControllerPID(double controlInputLimitMin, double controlInputLimitMax, double kp, double ki, double kd) : ControllerAbstract(controlInputLimitMin, controlInputLimitMax){
  _kp = kp;
  _ki = ki;
  _kd = kd;
  _pidOutput = 0;
  
  _myPID = new PID(&_measuredOutput, &_pidOutput, &_referenceInput, _kp, _ki, _kd, DIRECT);
  _myPID->SetOutputLimits(_pidLimitMin, _pidLimitMax);
  _myPID->SetMode(AUTOMATIC);
}

ControllerPID::~ControllerPID(){
  
}

void ControllerPID::ComputeControlInput(double controlError){

  // Try to compute the control input delta by using PID
  bool res = _myPID->Compute();

  // If successful, then update the control input value
  if(res == true){
    _controlInput = _controlInput + _pidOutput;
  }
}
