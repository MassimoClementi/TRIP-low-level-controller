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

void DataExchangeSerial::Update(){
  // ...
}
