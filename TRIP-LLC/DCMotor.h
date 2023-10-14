/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "Data.h"
#include "DCMotorAbstract.h"

/*
 * Class that embeds attributes and methods to define
 * a rotative direct current motor, commanding its speed and direction
 * by using arduino motor shield
  */
class DCMotor : public DCMotorAbstract
{
private:   
  const double _maxOutputValue = 255;

  int _pinNumberHBridgeA, _pinNumberHBridgeB, _pinNumberControl;

public:
    DCMotor(int pinNumberHBridgeA, int pinNumberHBridgeB, int pinNumberControl);
    ~DCMotor();

    void UpdateOutputValue();
};

#endif
