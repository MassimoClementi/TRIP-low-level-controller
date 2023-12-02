/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-09
 */ 

#include "ControllerStep.h"


ControllerStep::ControllerStep(double controlInputLimitMin, double controlInputLimitMax, double kProportional) : ControllerAbstract(controlInputLimitMin, controlInputLimitMax){
  _kProportional = kProportional;
}

ControllerStep::~ControllerStep(){
  
}

void ControllerStep::ComputeControlInput(double controlError){
  _controlInput = _controlInput + controlError * _kProportional;
}
