/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-09
 */ 

#include "ControllerAbstract.h"

ControllerAbstract::ControllerAbstract(){
  _referenceInput = 0;
  _measuredOutput = 0;
  _controlInput = 0;
}

ControllerAbstract::~ControllerAbstract(){
  
}

void ControllerAbstract::SetTarget(const double target){
  _referenceInput = target;
}

void ControllerAbstract::SetMeasuredOutput(const double measuredOutput){
  _measuredOutput = measuredOutput;

  // As the output is measured, perform update of all variables
  
  // First compute the control error
  double controlError = _referenceInput - _measuredOutput;

  // Then update the control input
  ComputeControlInput(controlError);

  // Finally, broadcast successful update
  EUpdateControlInput(_controlInput);
}
