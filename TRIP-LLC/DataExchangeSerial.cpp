/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "DataExchangeSerial.h"

DataExchangeSerial::DataExchangeSerial(int baudRate, int updateInterval_ms) : DataExchangeAbstract(updateInterval_ms){
  _baudRate = baudRate;
  Serial.begin(_baudRate);
}

DataExchangeSerial::~DataExchangeSerial()
{
}

void DataExchangeSerial::SendMessage(const String message){
  Serial.println(message);
}

void DataExchangeSerial::SendEncoderMeasurement(const EncoderMeasurement encoderMeasurement, const int encoderNumber){
  SendMessage("ENC" + String(encoderNumber) + 
              ", RPM: " + String(ConvertSpeed_FromIntToExt(encoderMeasurement.rpm)) + 
              ", T: " + String(encoderMeasurement.instant_ms));
}

double DataExchangeSerial::GetIntToExtSpeedConversionFactor(){
  return 1.0;
}

void DataExchangeSerial::Update(){
  if (_isReading == false && Serial.available() > 0){
    _isReading = true;

    // Read message on serial until end-of-line character
    SendMessage("Receiving message...");
    String currMessage = Serial.readStringUntil('\n');
    currMessage.trim();
    SendMessage("Message received: " + currMessage);
    _message = currMessage.c_str();
    SendMessage("C message is: " + String(_message));

    // Parse command by splitting by separator 
    char * strtokIndx;
    strtokIndx = strtok(_message,",");
    _lastCommand.instruction = String(strtokIndx);    // get instruction as string
    strtokIndx = strtok(NULL, ",");
    _lastCommand.arg1 = double(atof(strtokIndx));     // convert arg1 to double
    strtokIndx = strtok(NULL, ",");
    _lastCommand.arg2 = ConvertSpeed_FromExtToInt(double(atof(strtokIndx)));     // convert arg2 to double

    // Log formatted received command
    SendMessage("Command received: " + String(_lastCommand.instruction) + 
                                  " | " + String(_lastCommand.arg1) + " | " + String(_lastCommand.arg2));

    // Share command
    ECommandReceived(_lastCommand);

    SendMessage("Resetting receive flag...");
    _isReading = false;
  }
}
