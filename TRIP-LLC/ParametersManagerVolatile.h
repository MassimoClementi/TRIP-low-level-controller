/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-12-27
 */ 

#ifndef __PARAMETERS_MANAGER_VOLATILE_H__
#define __PARAMETERS_MANAGER_VOLATILE_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "Data.h"
#include "ParametersManagerAbstract.h"

/*
 * Class that embeds attributes and methods to define
 * a parameter manager which is volatile and does not mantain
 * the modified value after reboot
 * This may be preferred if the EEPROM memory has to be not used
  */
class ParametersManagerVolatile : public ParametersManagerAbstract
{
protected:
  bool StoreParameters();
  bool StoreParameter(int paramIndex);
  bool RestoreParameters();
  
public:
    ParametersManagerVolatile();
    ~ParametersManagerVolatile();
};

#endif
