/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-09
 */ 

#include "ParametersManagerAbstract.h"

ParametersManagerAbstract::ParametersManagerAbstract(){
  
}

ParametersManagerAbstract::~ParametersManagerAbstract(){

}

uint8_t ParametersManagerAbstract::GetNumVariables(){
  return _numVariables;
}

bool ParametersManagerAbstract::SetVariableIfNotExists(char* paramName, double paramValue){
  // Try to get the parameter, initialize it if does not already exist
  // Return false if parameter already existed, true otherwise
  if(GetVariable(paramName) == nullptr){
    SetVariable(paramName, paramValue, true);
    return true;
  }
  return false;
}

void ParametersManagerAbstract::InitializeVariables(){
  // Try get each variable, if not then create with default

  if(RestoreParameters() == false){
    return;
  }

  SetVariableIfNotExists("M1_ENC_IMP", 1630);
  SetVariableIfNotExists("M2_ENC_IMP", 1630);
  
  SetVariableIfNotExists("M1_ENC_TIN", 300);
  SetVariableIfNotExists("M2_ENC_TIN", 300);
  
  SetVariableIfNotExists("M1_CON_KP",  0.02);
  SetVariableIfNotExists("M2_CON_KP",  0.02);

  return;
}

ParameterVariable* ParametersManagerAbstract::GetVariable(char* paramName){
  // Search in the variables the one with the same unique name identifier
  // Once found, then get the related value

  if(RestoreParameters() == false){
    return nullptr;
  }

  int matchIdx = _numVariables;   // initialize index as non-valid value

  // Search for first occcurence in parameter list
  // that matches the provided name
  for(uint8_t i = 0; i < _numVariables; i++){
    if(_parameters[i].isInitialized == 0){
      // Skip uninitialized variables
      continue;
    }
    if(strcmp(_parameters[i].paramName, paramName) == 0){
      matchIdx = i;
      break;
    }
  }

  // Return matched parameter, if found
  if(matchIdx != _numVariables){
    return &(_parameters[matchIdx]);
  }
  else
  {
    return nullptr;  
  }
  
}

bool ParametersManagerAbstract::SetVariable(char* paramName, double paramValue, bool enableCreateNew = false){
  // Search in the parameters list if a variable with the same unique name identifier does exists
  // If it is present, then update its value and initialization
  // If not present and creation of new variable is enabled, select the first uninitialized position, if available

  if(RestoreParameters() == false){
    return false;
  }

  // Initialize indexes as non-valid values
  uint8_t matchIdxNotInitialized = _numVariables;
  uint8_t matchIdxFound = _numVariables;

  // Search for first occcurence in parameter list
  // that matches the provided name
  for(uint8_t i = 0; i < _numVariables; i++){
    if(matchIdxNotInitialized == _numVariables and _parameters[i].isInitialized == 0){
      // Save first occurence of uninitialized variable
      matchIdxNotInitialized = i;
    }
    if(strcmp(_parameters[i].paramName, paramName) == 0){
      matchIdxFound = i;
      break;
    }
  }

  // Evaluate search results
  uint8_t setIndex = _numVariables;
  if(matchIdxFound != _numVariables){
    setIndex = matchIdxFound;
  }
  else if(enableCreateNew == true and matchIdxNotInitialized != _numVariables){
    setIndex = matchIdxNotInitialized;
  }
  else
  {
    return false;
  }

  // Save parameter at the selected position
  strcpy(_parameters[setIndex].paramName, paramName);
  _parameters[setIndex].paramValue = paramValue;
  _parameters[setIndex].isInitialized = true;
  return StoreParameter(setIndex);
}

bool ParametersManagerAbstract::RemoveVariable(uint8_t paramIndex){
  // Set the parameter at the given index as empty
  _parameters[paramIndex].paramName[0] = '\0';
  _parameters[paramIndex].paramValue = 0.0;
  _parameters[paramIndex].isInitialized = false;
  return true;
}

bool ParametersManagerAbstract::ResetAll(){
  // Reset all parameter list to empty and store changes
  for(uint8_t i = 0; i < _numVariables; i++){
    RemoveVariable(i);
  }
  return StoreParameters();
}

String ParametersManagerAbstract::GetParameterDescription(uint8_t paramIdex){
  // Get formatted parameter name, value and initialization status
  bool isInitialized = _parameters[paramIdex].isInitialized > 0;
  if(isInitialized == true){
    // If initialized, return all details
    return String(_parameters[paramIdex].paramName) + " | " + 
         String(_parameters[paramIdex].paramValue) + " | " +  
         String(_parameters[paramIdex].isInitialized);
  } else {
    // Display non-initialized parameters as placeholders
    return "(non-initialized)";
  }
  
}
