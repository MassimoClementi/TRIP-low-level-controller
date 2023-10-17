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

#include <AFMotor.h>

/*
 * Class that embeds attributes and methods to define
 * a rotative direct current motor, commanding its speed and direction
 * by using adafruit-based arduino motor shield
  */
class DCMotor : public DCMotorAbstract
{
private:   
  const double _maxOutputValue = 255;
  int _DCMotorID;

  AF_DCMotor* _motor = nullptr;

public:
    DCMotor(int DCMotorID);
    ~DCMotor();

    void UpdateOutputValue();
};

#endif
