/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-26
 */ 

#include "ParametersManagerEEPROM.h"

ParametersManagerEEPROM::ParametersManagerEEPROM() : ParametersManagerAbstract() {
  InitializeVariables();
}

ParametersManagerEEPROM::~ParametersManagerEEPROM(){
  
}

bool ParametersManagerEEPROM::StoreParameters(){
  // Sequentially store all parameters to the internal EEPROM
  for(uint8_t i = 0; i < _numVariables; i++){
    StoreParameter(i);
  }
  return true;
}

bool ParametersManagerEEPROM::StoreParameter(uint8_t paramIndex){
  // Store a single parameter to the internal EEPROM
  // Serial.println("WARNING: AN ACCESS TO EEPROM IS NECESSARY!");
  EEPROM.put(
    paramIndex * sizeof(ParameterVariable),
    _parameters[paramIndex]
  );
  return true;
}

bool ParametersManagerEEPROM::RestoreParameters(){
  // Load all parameters from internal EEPROM
  for(uint8_t i = 0; i < _numVariables; i++){
    EEPROM.get(
      i * sizeof(ParameterVariable),
      _parameters[i]
    );
  }
  return true;
}
