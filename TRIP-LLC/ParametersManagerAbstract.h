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
  static const int _numVariables = 5;
  ParameterVariable _parameters[_numVariables];

  void InitializeVariables();
  
  bool SaveParametersToDisk();
  bool LoadParametersFromDisk();
  
public:
    ParametersManagerAbstract();
    ~ParametersManagerAbstract();

    double GetVariable(char* paramName);
    bool SetVariable(char* paramName, double paramValue);
    bool RemoveVariable(char* paramName);
    String GetParameterDescription(int paramIndex);
    int GetNumVariables();
};

#endif
