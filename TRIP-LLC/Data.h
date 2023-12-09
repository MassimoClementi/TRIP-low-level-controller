/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#ifndef __DATA_H__
#define __DATA_H__

struct EncoderMeasurement {
   unsigned long  instant_ms;
   double rpm;
};

struct Command {
   String instruction;
   double arg1;
   double arg2;
};

// Defined as Adafruit-Motor-Shield-Library
enum RotationDirection{
  FORWARD = 1,
  BACKWARD = 2
};

// 10 + 8 + 1 bytes ~ 20 bytes
struct ParameterVariable{
  char paramName[10];
  double paramValue = 0.0;
  bool isInitialized = false;
};

#endif
