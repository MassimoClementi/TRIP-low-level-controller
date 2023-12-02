/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-29
 */ 

#ifndef __CONTROLLER_PID_H__
#define __CONTROLLER_PID_H__

#include <MicroQt.h>
using namespace MicroQt;

#include <PID_v1.h>

#include "ControllerAbstract.h"

/*
 * Class that embeds attributes and methods to define
 * a PID-based closed-loop feedback controller
  */
class ControllerPID : public ControllerAbstract
{
protected:
    double _kp, _ki, _kd;
    double _pidOutput;

    const double _pidLimitMin = -1.0;
    const double _pidLimitMax = 1.0;
    
    PID* _myPID = nullptr;

    void ComputeControlInput(double controlError);

public:
    ControllerPID(double controlInputLimitMin, double controlInputLimitMax, double kp, double ki, double kd);
    ~ControllerPID();
};

#endif
