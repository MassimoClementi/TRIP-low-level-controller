/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-09
 */ 

#include "ControllerStep.h"


ControllerStep::ControllerStep() : ControllerAbstract(){
  
}

ControllerStep::~ControllerStep(){
  
}

void ControllerStep::ComputeControlInput(double controlError){
  _controlInput = _controlInput + controlError * 0.01;
}
