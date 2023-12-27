/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-09
 */ 

#ifndef __PARAMETERS_MANAGER_ABSTRACT_H__
#define __PARAMETERS_MANAGER_ABSTRACT_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "Data.h"

/*
 * Class that embeds attributes and methods to define
 * a persistent parameter manager
  */
class ParametersManagerAbstract
{
protected:
  static const int _numVariables = 10;
  ParameterVariable _parameters[_numVariables];

  void InitializeVariables();
  bool SetVariableIfNotExists(char* paramName, double paramValue);

  virtual bool StoreParameters() = 0;
  virtual bool StoreParameter(int paramIndex) = 0;
  virtual bool RestoreParameters() = 0;
  
public:
    ParametersManagerAbstract();
    ~ParametersManagerAbstract();

    ParameterVariable* GetVariable(char* paramName);
    bool SetVariable(char* paramName, double paramValue, bool enableCreateNew = false);
    bool RemoveVariable(int paramIndex);

    int GetNumVariables();
    String GetParameterDescription(int paramIndex);
    
    bool ResetAll();
};

#endif
