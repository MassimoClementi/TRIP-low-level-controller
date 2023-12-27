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
#import "ParametersManagerEEPROM.h"

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
ParametersManagerAbstract* parametersManager;

void setup() {
  dataExchange = new DataExchangeSerial(9600, 50);
  dataExchange->SendMessage("Hello world from TRIP-LLC!");
  dataExchange->ECommandReceived.connect(&OnCommandReceived);

  parametersManager = new ParametersManagerEEPROM();
  double M1_ENC_IMP = parametersManager->GetVariable("M1_ENC_IMP")->paramValue;
  double M2_ENC_IMP = parametersManager->GetVariable("M2_ENC_IMP")->paramValue;
  double M1_ENC_TIN = parametersManager->GetVariable("M1_ENC_TIN")->paramValue;
  double M2_ENC_TIN = parametersManager->GetVariable("M2_ENC_TIN")->paramValue;
  double M1_CON_KP = parametersManager->GetVariable("M1_CON_KP")->paramValue;
  double M2_CON_KP = parametersManager->GetVariable("M2_CON_KP")->paramValue;

  dcMotors[0] = new DCMotor_TB6612FNG(PIN_MOTOR1_IN1, PIN_MOTOR1_IN2, PIN_MOTOR1_PWM, PIN_MOTORS_STBY);
  dcMotors[1] = new DCMotor_TB6612FNG(PIN_MOTOR2_IN1, PIN_MOTOR2_IN2, PIN_MOTOR2_PWM, PIN_MOTORS_STBY);

  rotaryEncoders[0] = new RotaryEncoder(PIN_ENC1_Q1, PIN_ENC1_Q2, M1_ENC_IMP, M1_ENC_TIN);
  rotaryEncoders[0]->EMeasurement.connect(&OnEncoder1Measurement);
  rotaryEncoders[1] = new RotaryEncoder(PIN_ENC2_Q1, PIN_ENC2_Q2, M2_ENC_IMP, M2_ENC_TIN);
  rotaryEncoders[1]->EMeasurement.connect(&OnEncoder2Measurement);

  controllers[0] = new ControllerStep(-1.0, 1.0, M1_CON_KP);
  //controllers[0] = new ControllerPID(-1.0, 1.0, 0.02, 0.07, 0.001);
  controllers[0]->EUpdateControlInput.connect(&OnController1UpdateControlInput);
  controllers[1] = new ControllerStep(-1.0, 1.0, M2_CON_KP);
  controllers[1]->EUpdateControlInput.connect(&OnController2UpdateControlInput);
  
  eventLoop.setLogIntervalMs(30000);
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
  // dataExchange->SendMessage("CON" + String(controllerNumber) + ", INPUT SET " + String(controlInput));
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
  if(command.instruction == "PSET"){
    // Set the provided parameter to the given value
    dataExchange->SendMessage("Setting parameter " + command.arg1 + " to value " + command.arg2);
    bool res = parametersManager->SetVariable(command.arg1.c_str(), command.arg2);
    if(res == true){
      dataExchange->SendMessage("Parameter value updated successfully");
    } else {
      dataExchange->SendMessage("Parameter value update FAIL");
    }
    return;
  }
  if(command.instruction == "PLIST"){
    // Provide the list of all parameters
    dataExchange->SendMessage("== EEPROM PARAMETERS CONTENT ==");
    dataExchange->SendMessage("Name | Value | Instantiation status");
    for(int i = 0; i < parametersManager->GetNumVariables(); i++){
      dataExchange->SendMessage(parametersManager->GetParameterDescription(i));
    }
    dataExchange->SendMessage("== End of table ==");
    return;
  }
  if(command.instruction == "PERASE"){
    // Reset all parameters stored to EEPROM
    // This is a non-ordinary operation to perform
    parametersManager->ResetAll();
    dataExchange->SendMessage("EEPROM resetted");
    return;
  }

  // The following commands are motor-specific
  // It is then necessary to assert beforehand that the index
  // of the motor of interest is valid
  int selectedItem = atoi(command.arg1.c_str());
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
 
