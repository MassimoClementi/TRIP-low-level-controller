/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#ifndef __DATA_EXCHANGE_SERIAL_H__
#define __DATA_EXCHANGE_SERIAL_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "DataExchangeAbstract.h"
#include "Data.h"

/*
 * Class that embeds attributes and methods to instante
 * a serial data exchange object with internal thread
  */
class DataExchangeSerial : public DataExchangeAbstract
{
private:
    int _baudRate;
    bool _isReading = false;
    const char* _message = nullptr;

    double GetIntToExtSpeedConversionFactor();
    void Update();

public:
    DataExchangeSerial(int baudRate, int updateInterval_ms);
    ~DataExchangeSerial();

    void SendMessage(const String message);
    void SendEncoderMeasurement(const EncoderMeasurement encoderMeasurement, const int encoderNumber);

};

#endif
