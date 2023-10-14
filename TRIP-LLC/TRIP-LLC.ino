/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

// Libraries imports
#include <MicroQt.h>
using namespace MicroQt;
#import "DataExchangeSerial.h"

// Global variables
DataExchangeSerial* dataExchangeSerial = nullptr;

void setup() {
  dataExchangeSerial = new DataExchangeSerial(9600, 50);
  dataExchangeSerial->SendMessage("Hello world from TRIP-LLC!");
  eventLoop.setLogIntervalMs(10000);
}

void loop() {
  // The loop function manages all events and timers
  eventLoop.exec();
}
