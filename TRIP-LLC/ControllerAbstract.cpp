/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-09
 */ 

#include "ControllerAbstract.h"

ControllerAbstract::ControllerAbstract(double controlInputLimitMin, double controlInputLimitMax){
  _referenceInput = 0;
  _measuredOutput = 0;
  _controlInput = 0;
  _controlInputLimitMin = controlInputLimitMin;
  _controlInputLimitMax = controlInputLimitMax;
  _isEnabled = false;
}

ControllerAbstract::~ControllerAbstract(){
  
}

void ControllerAbstract::SetTarget(const double target){
  _referenceInput = target;
}

void ControllerAbstract::SetEnabled(const bool isEnabled){
  _isEnabled = isEnabled;
}

void ControllerAbstract::SetMeasuredOutput(const double measuredOutput){
  _measuredOutput = measuredOutput;

  // Assert that controller is enabled
  if(not _isEnabled){
    return;
  }

  // As the output is measured, perform update of all variables
  
  // First compute the control error
  double controlError = _referenceInput - _measuredOutput;

  // Then update the control input
  ComputeControlInput(controlError);

    // Always limit the control input values between valid bouds
  _controlInput = max(min(_controlInput, _controlInputLimitMax), _controlInputLimitMin);

  // Finally, broadcast successful update
  EUpdateControlInput(_controlInput);
}
