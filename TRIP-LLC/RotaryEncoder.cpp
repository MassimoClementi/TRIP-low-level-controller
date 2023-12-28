/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-20
 */ 

#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(uint8_t pinEncoderA, uint8_t pinEncoderB, unsigned long numImpulsesPerFullRotation, int updateInterval_ms) : RotaryEncoderAbstract(numImpulsesPerFullRotation, updateInterval_ms){
  _pinEncoderA = pinEncoderA;
  _pinEncoderB = pinEncoderB;

  _encoder = new Encoder(_pinEncoderA, _pinEncoderB);
  _encoder->write(0);

    // Configure fast reading thread
    readingTimer.setIntervalMs(0);
    readingTimer.setSingleShot(false);
    readingTimer.sglTimeout.connect([this](){this->GetImpulses();});
    readingTimer.start();    
}

RotaryEncoder::~RotaryEncoder(){
  
}


void RotaryEncoder::GetImpulses(){
  long impulses = _encoder->read();
  _encoder->write(0);
  _currNumImpulses += impulses;
}
