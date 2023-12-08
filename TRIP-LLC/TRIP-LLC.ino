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
#import "ControllerStep.h"
#import "ControllerPID.h"

// Hardware configuration
#define NUM_MOTORS 2
#define PIN_ENC1_Q1 2
#define PIN_ENC1_Q2 4
#define PIN_ENC2_Q1 3
#define PIN_ENC2_Q2 7
#define PIN_MOTOR1_IN1 8
#define PIN_MOTOR1_IN2 9
#define PIN_MOTOR1_PWM 5
#define PIN_MOTOR2_IN1 10
#define PIN_MOTOR2_IN2 11
#define PIN_MOTOR2_PWM 6
#define PIN_MOTORS_STBY 12

// Global variables
DataExchangeAbstract* dataExchange = nullptr;
DCMotorAbstract* dcMotors[NUM_MOTORS];
RotaryEncoderAbstract* rotaryEncoders[NUM_MOTORS];
ControllerAbstract* controllers[NUM_MOTORS];

void setup() {
  dataExchange = new DataExchangeSerial(9600, 50);
  dataExchange->SendMessage("Hello world from TRIP-LLC!");
  dataExchange->ECommandReceived.connect(&OnCommandReceived);

  dcMotors[0] = new DCMotor_TB6612FNG(PIN_MOTOR1_IN1, PIN_MOTOR1_IN2, PIN_MOTOR1_PWM, PIN_MOTORS_STBY);
  dcMotors[1] = new DCMotor_TB6612FNG(PIN_MOTOR2_IN1, PIN_MOTOR2_IN2, PIN_MOTOR2_PWM, PIN_MOTORS_STBY);

  rotaryEncoders[0] = new RotaryEncoder(PIN_ENC1_Q1, PIN_ENC1_Q2, 1630, 300);
  rotaryEncoders[0]->EMeasurement.connect(&OnEncoder1Measurement);
  rotaryEncoders[1] = new RotaryEncoder(PIN_ENC2_Q1, PIN_ENC2_Q2, 1630, 300);
  rotaryEncoders[1]->EMeasurement.connect(&OnEncoder2Measurement);

  controllers[0] = new ControllerStep(-1.0, 1.0, 0.02);
  //controllers[0] = new ControllerPID(-1.0, 1.0, 0.02, 0.07, 0.001);
  controllers[0]->EUpdateControlInput.connect(&OnController1UpdateControlInput);
  controllers[1] = new ControllerStep(-1.0, 1.0, 0.01);
  controllers[1]->EUpdateControlInput.connect(&OnController2UpdateControlInput);
  
  eventLoop.setLogIntervalMs(10000);
  dataExchange->SendMessage("TRIP-LLC configuration completed");
}

void loop() {
  // The loop function manages all events and timers
  eventLoop.exec();
}


/*
 * Events callback functions and utilities
 */

void OnEncoder1Measurement(const EncoderMeasurement encoderMeasurement){
    OnEncoderMeasurement(encoderMeasurement, 0);
}

void OnEncoder2Measurement(const EncoderMeasurement encoderMeasurement){
    OnEncoderMeasurement(encoderMeasurement, 1);
}

void OnEncoderMeasurement(const EncoderMeasurement encoderMeasurement, int encoderNumber){
    // dataExchange->SendEncoderMeasurement(encoderMeasurement, encoderNumber);
    controllers[encoderNumber]->SetMeasuredOutput(encoderMeasurement.rpm);
}

void OnController1UpdateControlInput(const double controlInput){
  OnControllerUpdateControlInput(controlInput, 0);
}

void OnController2UpdateControlInput(const double controlInput){
  OnControllerUpdateControlInput(controlInput, 1);
}

void OnControllerUpdateControlInput(const double controlInput, int controllerNumber){
  dataExchange->SendMessage("CON" + String(controllerNumber) + ", INPUT SET " + String(controlInput));
  dcMotors[controllerNumber]->SetSpeedPercent(controlInput);
}

void OnCommandReceived(const Command command){
  // The following commands are motor-agnostic
  // Therefore no value checks need to be performed
  if(command.instruction == "ENC"){
    // Share last measurement of all encoders
    for(int enc_idx = 0; enc_idx < NUM_MOTORS; enc_idx++){
      dataExchange->SendEncoderMeasurement(
        rotaryEncoders[enc_idx]->GetLastMeasurement(),
        enc_idx);
    }
    return;
  }

  // The following commands are motor-specific
  // It is then necessary to assert beforehand that the index
  // of the motor of interest is valid
  int selectedItem = int(command.arg1);
  if(selectedItem < 0 or selectedItem >= NUM_MOTORS){
    return;
  }
  if(command.instruction == "MSET"){
    // Set directly motor percent speed
    controllers[selectedItem]->SetEnabled(false);
    dcMotors[selectedItem]->SetSpeedPercent(command.arg2);
    return;
  }
  if(command.instruction == "CSET"){
    // Define new absolute speed setpoint for motor controller
    controllers[selectedItem]->SetTarget(command.arg2);
    controllers[selectedItem]->SetEnabled(true);
    return;
  }
}
 
