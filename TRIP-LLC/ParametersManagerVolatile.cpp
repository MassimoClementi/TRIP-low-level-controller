/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-27
 */ 

#include "ParametersManagerVolatile.h"

ParametersManagerVolatile::ParametersManagerVolatile() : ParametersManagerAbstract() {
  InitializeVariables();
}

ParametersManagerVolatile::~ParametersManagerVolatile(){
  
}

bool ParametersManagerVolatile::StoreParameters(){
  // No operation needs to be performed
  return true;
}

bool ParametersManagerVolatile::StoreParameter(uint8_t paramIndex){
  // No operation needs to be performed
  return true;
}

bool ParametersManagerVolatile::RestoreParameters(){
  // No operation needs to be performed
  return true;
}
