/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-09
 */ 

#include "ControllerStep.h"


ControllerStep::ControllerStep(double controlInputLimitMin, double controlInputLimitMax) : ControllerAbstract(controlInputLimitMin, controlInputLimitMax){
  
}

ControllerStep::~ControllerStep(){
  
}

void ControllerStep::ComputeControlInput(double controlError){
  _controlInput = _controlInput + controlError * 0.01;
}
