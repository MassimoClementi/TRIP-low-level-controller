/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

// Libraries imports
#include <MicroQt.h>
using namespace MicroQt;
#import "Data.h"
#import "DataExchangeSerial.h"
#import "DCMotor_TB6612FNG.h"
#import "RotaryEncoder.h"

// Global variables
DataExchangeSerial* dataExchangeSerial = nullptr;
DCMotor_TB6612FNG* dcMotor = nullptr;
RotaryEncoder* rotaryEncoder = nullptr;

void setup() {
  dataExchangeSerial = new DataExchangeSerial(9600, 50);
  dataExchangeSerial->SendMessage("Hello world from TRIP-LLC!");
  dataExchangeSerial->ECommandReceived.connect(&OnCommandReceived);

  dcMotor = new DCMotor_TB6612FNG(8, 9, 5, 12);

  // Both quadrature signals use interrupt pins
  rotaryEncoder = new RotaryEncoder(2, 4, 1630, 300);
  rotaryEncoder->EMeasurement.connect(&OnEncoderMeasurement);

  // Startup the motor
  for(int i = 0; i < 100; i++){
    dcMotor->SetSpeedPercent(i / 100.0);
    // dataExchangeSerial->SendMessage(String(dcMotor->GetSpeedPercent()));
    delay(10);
  }
  
  eventLoop.setLogIntervalMs(10000);
  dataExchangeSerial->SendMessage("Board configuration completed");
}

void loop() {
  // The loop function manages all events and timers
  eventLoop.exec();
}

/*
 * Events callback functions
 */

void OnEncoderMeasurement(const EncoderMeasurement encoderMeasurement){
  dataExchangeSerial->SendMessage("Encoder measurement at instant " + String(encoderMeasurement.instant_ms) + 
                                  " with RPMs " + String(encoderMeasurement.rpm));
}

void OnCommandReceived(const Command command){
  dataExchangeSerial->SendMessage("Command received: " + String(command.instruction) + 
                                  " | " + String(command.arg1) + " | " + String(command.arg2));
  // Process specific commands logic
  if(command.instruction == "MSET"){
    dcMotor->SetSpeedPercent(command.arg1);
  }

}
 
