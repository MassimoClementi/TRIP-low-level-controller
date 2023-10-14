/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#ifndef __DC_MOTOR_ABSTRACT_H__
#define __DC_MOTOR_ABSTRACT_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "Data.h"

/*
 * Class that embeds attributes and methods to define
 * a rotative direct current motor
  */
class DCMotorAbstract
{
protected:   
    double _currSpeedPercent;
    RotationDirection _rotationDirection;

public:
    DCMotorAbstract();
    ~DCMotorAbstract();
    
    void SetSpeedPercent(const double speedPercent);
    void SetRotationDirection(const RotationDirection rotationDirection);
    double GetSpeedPercent();

    virtual void UpdateOutputValue() = 0;
};

#endif
