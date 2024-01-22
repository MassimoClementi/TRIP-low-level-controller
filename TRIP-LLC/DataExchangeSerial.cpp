/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "DataExchangeSerial.h"

DataExchangeSerial::DataExchangeSerial(int baudRate, int verboseLevel, int updateInterval_ms) : DataExchangeAbstract(updateInterval_ms){
  _baudRate = baudRate;
  _verboseLevel = verboseLevel;
  Serial.begin(_baudRate);
}

DataExchangeSerial::~DataExchangeSerial()
{
}

void DataExchangeSerial::SendMessage(const char message[96]){
  Serial.println(message);
}

void DataExchangeSerial::SendEncoderMeasurement(const EncoderMeasurement* encoderMeasurement, const uint8_t encoderNumber){
  SendMessage(("ENC" + String(encoderNumber) + 
              ", RPM: " + String(ConvertSpeed_FromIntToExt(encoderMeasurement->rpm)) + 
              ", T: " + String(encoderMeasurement->instant_ms)).c_str());
}

double DataExchangeSerial::GetIntToExtSpeedConversionFactor(){
  return 1.0;
}

void DataExchangeSerial::Update(){
  if (_isReading == false && Serial.available() > 0){
    _isReading = true;

    // Read message on serial until end-of-line character
    if(_verboseLevel > 0) { SendMessage("Receiving message..."); }
    String currMessage = Serial.readStringUntil('\n');
    currMessage.trim();
    if(_verboseLevel > 0) { SendMessage(("Message received: " + currMessage).c_str()); }
    _message = currMessage.c_str();
    // SendMessage("C message is: " + String(_message));

    // Parse command by splitting by separator 
    char * strtokIndx;
    strtokIndx = strupr(strtok(_message,","));      //  upper-case
    strcpy(_lastCommand.instruction, strtokIndx);
    strtokIndx = strtok(NULL, ",");
    strcpy(_lastCommand.arg1, strtokIndx);
    strtokIndx = strtok(NULL, ",");
    _lastCommand.arg2 = double(atof(strtokIndx));     // convert arg2 to double
    if(_lastCommand.instruction == "CSET"){
      // Only if a controller set command is received, scale value by RPM speed factor
      _lastCommand.arg2 = ConvertSpeed_FromExtToInt(_lastCommand.arg2);
    }

    // Log formatted received command
    if(_verboseLevel > 0) {
      SendMessage(("Command received: " + String(_lastCommand.instruction) + 
                                  " | " + String(_lastCommand.arg1) + " | " + String(_lastCommand.arg2)).c_str());
    }

    // Share command
    ECommandReceived(&_lastCommand);

    if(_verboseLevel > 0) { SendMessage("Resetting receive flag..."); }
    _isReading = false;
  }
}
