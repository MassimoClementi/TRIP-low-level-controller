/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-20
 */ 

#ifndef __ROTARY_ENCODER_ABSTRACT_H__
#define __ROTARY_ENCODER_ABSTRACT_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "ThreadObject.h"
#include "Data.h"

/*
 * Class that embeds attributes and methods to instante
 * a rotary encoder with internal polling thread
  */
class RotaryEncoderAbstract : public ThreadObject
{
private:
protected:
    unsigned long _numImpulsesPerFullRotation;
    long _currNumImpulses;
    EncoderMeasurement _lastMeasurement;
    void Update();
public:
    RotaryEncoderAbstract(unsigned long numImpulsesPerFullRotation, int updateInterval_ms);
    ~RotaryEncoderAbstract();

    Signal<const EncoderMeasurement> EMeasurement;
};

#endif
