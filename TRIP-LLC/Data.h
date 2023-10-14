/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#ifndef __DATA_H__
#define __DATA_H__

struct EncoderMeasurement {
   int  timeReference;
   double rpm;
};

struct Command {
   int  ID;
   double arg1;
   double arg2;
   double arg3;
};

enum RotationDirection{
  FORWARD,
  BACKWARD
};

#endif
