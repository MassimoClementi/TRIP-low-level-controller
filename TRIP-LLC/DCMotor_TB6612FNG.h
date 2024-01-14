/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-29
 */ 

#ifndef __DC_MOTOR_TB6612FNG_H__
#define __DC_MOTOR_TB6612FNG_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "Data.h"
#include "DCMotorAbstract.h"

/*
 * Class that embeds attributes and methods to define
 * a rotative direct current motor, commanding its speed and direction
 * by using TB6612FNG Toshiba Driver IC for Dual DC motor breakout board
  */
class DCMotor_TB6612FNG : public DCMotorAbstract
{
private:   
  const double _maxOutputValue = 255;
  uint8_t _pinIN1, _pinIN2, _pinPWM, _pinSTBY;

public:
    DCMotor_TB6612FNG(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinPWM, uint8_t pinSTBY);
    ~DCMotor_TB6612FNG();

    void SetStandbySignal(bool val);

    void UpdateOutputValue();
};

#endif
