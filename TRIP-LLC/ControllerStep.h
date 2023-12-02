/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-11-09
 */ 

#ifndef __CONTROLLER_STEP_H__
#define __CONTROLLER_STEP_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "ControllerAbstract.h"

/*
 * Class that embeds attributes and methods to define
 * a step-based closed-loop feedback controller
  */
class ControllerStep : public ControllerAbstract
{
protected:
    void ComputeControlInput(double controlError);

public:
    ControllerStep(double controlInputLimitMin, double controlInputLimitMax);
    ~ControllerStep();
};

#endif
