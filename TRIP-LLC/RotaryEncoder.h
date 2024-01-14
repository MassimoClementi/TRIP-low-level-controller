/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-20
 */ 

#ifndef __ROTARY_ENCODER_H__
#define __ROTARY_ENCODER_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "ThreadObject.h"
#include "Data.h"

#include "RotaryEncoderAbstract.h"

#include <Encoder.h>

/*
 * Class that embeds attributes and methods to instante
 * a phisical rotary encoder by using Encoder library
  */
class RotaryEncoder : public RotaryEncoderAbstract
{
private:
protected:
    uint8_t _pinEncoderA, _pinEncoderB;
    Encoder* _encoder = nullptr;
    const Timer readingTimer;
    void GetImpulses();
public:
    RotaryEncoder(uint8_t pinEncoderA, uint8_t pinEncoderB, unsigned long numImpulsesPerFullRotation, int updateInterval_ms);
    ~RotaryEncoder();
};

#endif
