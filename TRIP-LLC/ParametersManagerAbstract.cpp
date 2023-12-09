/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-09
 */ 

#include "ParametersManagerAbstract.h"

ParametersManagerAbstract::ParametersManagerAbstract(){
  LoadParametersFromDisk();
}

ParametersManagerAbstract::~ParametersManagerAbstract(){
  SaveParametersToDisk();
}

int ParametersManagerAbstract::GetNumVariables(){
  return _numVariables;
}

void ParametersManagerAbstract::InitializeVariables(){
  // Try get each variable, if not then create with default
  return;
}

bool ParametersManagerAbstract::SaveParametersToDisk(){
  return true;
}

bool ParametersManagerAbstract::LoadParametersFromDisk(){
  return true;
}

double ParametersManagerAbstract::GetVariable(char* paramName){
  // Search in the variables the one with the same unique name identifier
  // Once found, then get the related value

  int matchIdx = -1;

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

  if(matchIdx != -1){
    return _parameters[matchIdx].paramValue;
  }
  else
  {
    return 0.0;  
  }
  
}

bool ParametersManagerAbstract::SetVariable(char* paramName, double paramValue){
  // Search in the variables if a variable with the same unique name identifier exists
  // If so, then update its value and initialization
  // If not present, if possible, create a new variable in the first uninitialized position

  int matchIdxNotInitialized = -1;
  int matchIdxFound = -1;

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

  strcpy(_parameters[setIndex].paramName, paramName);
  _parameters[setIndex].paramValue = paramValue;
  _parameters[setIndex].isInitialized = true;
  return SaveParametersToDisk();
}

bool ParametersManagerAbstract::RemoveVariable(char* paramName){
  // Loop all variable until name matches
  // Then set variable as unititialized

  return SaveParametersToDisk();
}

String ParametersManagerAbstract::GetParameterDescription(int paramIdex){
  return String(_parameters[paramIdex].paramName) + " | " + 
         String(_parameters[paramIdex].paramValue) + " | " +  
         String(_parameters[paramIdex].isInitialized);
}
