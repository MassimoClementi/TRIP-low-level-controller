/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-09
 */ 

#include "ParametersManagerAbstract.h"

ParametersManagerAbstract::ParametersManagerAbstract(){
  
}

ParametersManagerAbstract::~ParametersManagerAbstract(){

}

int ParametersManagerAbstract::GetNumVariables(){
  return _numVariables;
}

bool ParametersManagerAbstract::SetVariableIfNotExists(char* paramName, double paramValue){
  // Try to get the parameter, initialize it if does not already exist
  // Return false if parameter already existed, true otherwise
  if(GetVariable(paramName) == nullptr){
    SetVariable(paramName, paramValue);
    return true;
  }
  return false;
}

void ParametersManagerAbstract::InitializeVariables(){
  // Try get each variable, if not then create with default

  if(RestoreParameters() == false){
    return;
  }

  for(int i = 0; i < _numVariables; i++){
    SetVariableIfNotExists(("PAR" + String(i)).c_str(), i * 100.0);
  }

  /*
  SetVariableIfNotExists("A", 10.01);
  SetVariableIfNotExists("B", 20.02);
  SetVariableIfNotExists("C", 30.03);
  */

  return;
}

ParameterVariable* ParametersManagerAbstract::GetVariable(char* paramName){
  // Search in the variables the one with the same unique name identifier
  // Once found, then get the related value

  if(RestoreParameters() == false){
    return nullptr;
  }

  int matchIdx = -1;

  // Search for first occcurence in parameter list
  // that matches the provided name
  for(int i = 0; i < _numVariables; i++){
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
  if(matchIdx != -1){
    return &(_parameters[matchIdx]);
  }
  else
  {
    return nullptr;  
  }
  
}

bool ParametersManagerAbstract::SetVariable(char* paramName, double paramValue){
  // Search in the parameters list if a variable with the same unique name identifier does exists
  // If it is present, then update its value and initialization
  // If not present, create a new variable in the first uninitialized position, if available

  if(RestoreParameters() == false){
    return false;
  }

  int matchIdxNotInitialized = -1;
  int matchIdxFound = -1;

  // Search for first occcurence in parameter list
  // that matches the provided name
  for(int i = 0; i < _numVariables; i++){
    if(matchIdxNotInitialized == -1 and _parameters[i].isInitialized == 0){
      // Save first occurence of uninitialized variable
      matchIdxNotInitialized = i;
    }
    if(strcmp(_parameters[i].paramName, paramName) == 0){
      matchIdxFound = i;
      break;
    }
  }

  // Evaluate search results
  int setIndex = -1;
  if(matchIdxFound != -1){
    setIndex = matchIdxFound;
  }
  else if(matchIdxNotInitialized != -1){
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

bool ParametersManagerAbstract::RemoveVariable(char* paramName){
  // Loop all variable until name matches
  // Then set variable as unititialized
  return false;
}

bool ParametersManagerAbstract::ResetAll(){
  // Reset all parameter list to empty and store changes
  for(int i = 0; i < _numVariables; i++){
    _parameters[i].paramName[0] = '\0';
    _parameters[i].paramValue = 0.0;
    _parameters[i].isInitialized = false;
  }
  return StoreParameters();
}

String ParametersManagerAbstract::GetParameterDescription(int paramIdex){
  // Get formatted parameter name, value and initialization status
  return String(_parameters[paramIdex].paramName) + " | " + 
         String(_parameters[paramIdex].paramValue) + " | " +  
         String(_parameters[paramIdex].isInitialized);
}
