/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-20
 */ 

#include "RotaryEncoderAbstract.h"

RotaryEncoderAbstract::RotaryEncoderAbstract(unsigned long numImpulsesPerFullRotation, int updateInterval_ms) : ThreadObject(updateInterval_ms){
  _numImpulsesPerFullRotation = numImpulsesPerFullRotation;
  _lastMeasurement.instant_ms = millis();
  _lastMeasurement.rpm = 0.0;
}

RotaryEncoderAbstract::~RotaryEncoderAbstract(){
  
}

EncoderMeasurement RotaryEncoderAbstract::GetLastMeasurement(){
  return _lastMeasurement;
}

void RotaryEncoderAbstract::Update(){

  // Compute speed based on current impulses count
  /*
   * Note that the used encoder library for quadrature rotary encoders returns 
   * the inpulses count with 4X resolution. This has to be taken into account
   * during the current RPM computation
   */
  unsigned long current_ms = millis();
  unsigned long delta_ms = current_ms - _lastMeasurement.instant_ms;
  double delta_s = double(delta_ms / 1000.0);
  double rotations = double(_currNumImpulses / (4.0 * _numImpulsesPerFullRotation));
  double rpm = 60.0 * rotations / delta_s;

  // Update last measurement
  _lastMeasurement.instant_ms = current_ms;
  _lastMeasurement.rpm = rpm;

  // Reset impulses count
  _currNumImpulses = 0;

  // Broadcast measurement
  EMeasurement(_lastMeasurement);
  
}
