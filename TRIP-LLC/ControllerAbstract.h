/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-09
 */ 

#ifndef __CONTROLLER_ABSTRACT_H__
#define __CONTROLLER_ABSTRACT_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "Data.h"

/*
 * Class that embeds attributes and methods to define
 * a closed-loop feedback controller
  */
class ControllerAbstract
{
protected:
    // Enable or disable the controller
    bool _isEnabled;

    // Setpoint, desired physical output target
    double _referenceInput;

    // Feedback, measurement of actual physical output
    double _measuredOutput;

    // Control, input to the system
    double _controlInput;
    double _controlInputLimitMin;
    double _controlInputLimitMax;

    virtual void ComputeControlInput(double controlError) = 0;

public:
    ControllerAbstract(double controlInputLimitMin, double controlInputLimitMax);
    ~ControllerAbstract();
    
    void SetTarget(const double target);

    void SetEnabled(const bool isEnabled);

    void SetMeasuredOutput(const double measuredOutput);

    Signal<const double> EUpdateControlInput;

};

#endif
