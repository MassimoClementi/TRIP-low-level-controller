/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-26
 */ 

#ifndef __PARAMETERS_MANAGER_EEPROM_H__
#define __PARAMETERS_MANAGER_EEPROM_H__

#include <MicroQt.h>
using namespace MicroQt;

#include <EEPROM.h>

#include "Data.h"
#include "ParametersManagerAbstract.h"

/*
 * Class that embeds attributes and methods to define
 * a persistent parameter manager using EEPROM memory
  */
class ParametersManagerEEPROM : public ParametersManagerAbstract
{
protected:
  bool StoreParameters();
  bool StoreParameter(uint8_t paramIndex);
  bool RestoreParameters();
  
public:
    ParametersManagerEEPROM();
    ~ParametersManagerEEPROM();
};

#endif
