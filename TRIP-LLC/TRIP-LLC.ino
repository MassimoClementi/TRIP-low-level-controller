/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

// Libraries imports
#include <MicroQt.h>
using namespace MicroQt;
#import "BoardTypes.h"
#import "Data.h"
#import "DataExchangeSerial.h"
#import "DCMotor_TB6612FNG.h"
#import "RotaryEncoder.h"
#import "ControllerStep.h"
#import "ControllerPID.h"
#if defined(TRIP_LLC_PARAMETERS_MANAGER_SUPPORTED)
#import "ParametersManagerEEPROM.h"
#endif

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

// Software configuration
#define GENERIC_VERBOSE_LEVEL 1
#define DATA_EXCHANGE_VERBOSE_LEVEL GENERIC_VERBOSE_LEVEL
#if GENERIC_VERBOSE_LEVEL > 0
#define MICRO_QT_LOGGING_MS 30000
#else
#define MICRO_QT_LOGGING_MS 0
#endif

// Board parameters, with defaults
double M1_ENC_IMP = 1630;
double M2_ENC_IMP = 1630;
double M1_ENC_TIN = 300;
double M2_ENC_TIN = 300;
double M1_CON_KP = 0.02;
double M2_CON_KP = 0.02;

// Global variables
DataExchangeAbstract* dataExchange = nullptr;
DCMotorAbstract* dcMotors[NUM_MOTORS];
RotaryEncoderAbstract* rotaryEncoders[NUM_MOTORS];
ControllerAbstract* controllers[NUM_MOTORS];
#if defined(TRIP_LLC_PARAMETERS_MANAGER_SUPPORTED)
ParametersManagerAbstract* parametersManager = nullptr;
#endif

void setup() {
  dataExchange = new DataExchangeSerial(9600, DATA_EXCHANGE_VERBOSE_LEVEL, 50);
  #if GENERIC_VERBOSE_LEVEL > 0
    dataExchange->SendMessage("Hello world from TRIP-LLC!");
    dataExchange->SendMessage(("Board type: " + String(BOARD)).c_str());
  #endif
  dataExchange->ECommandReceived.connect(&OnCommandReceived);

  #if defined(TRIP_LLC_PARAMETERS_MANAGER_SUPPORTED)
  #if GENERIC_VERBOSE_LEVEL > 0
  dataExchange->SendMessage("Parameters manager available");
  #endif
  parametersManager = new ParametersManagerEEPROM();
  M1_ENC_IMP = parametersManager->GetVariable("M1_ENC_IMP")->paramValue;
  M2_ENC_IMP = parametersManager->GetVariable("M2_ENC_IMP")->paramValue;
  M1_ENC_TIN = parametersManager->GetVariable("M1_ENC_TIN")->paramValue;
  M2_ENC_TIN = parametersManager->GetVariable("M2_ENC_TIN")->paramValue;
  M1_CON_KP = parametersManager->GetVariable("M1_CON_KP")->paramValue;
  M2_CON_KP = parametersManager->GetVariable("M2_CON_KP")->paramValue;
  #else
  #if GENERIC_VERBOSE_LEVEL > 0
  dataExchange->SendMessage("Parameters manager not supported by current board");
  #endif
  #endif

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
  
  eventLoop.setLogIntervalMs(MICRO_QT_LOGGING_MS);
  #if GENERIC_VERBOSE_LEVEL > 0
  dataExchange->SendMessage("TRIP-LLC configuration completed");
  #endif
}

void loop() {
  // The loop function manages all events and timers
  eventLoop.exec();
}

/*
 * Events callback functions and utilities
 */

void OnEncoder1Measurement(const EncoderMeasurement* encoderMeasurement){
    OnEncoderMeasurement(encoderMeasurement, 0);
}

void OnEncoder2Measurement(const EncoderMeasurement* encoderMeasurement){
    OnEncoderMeasurement(encoderMeasurement, 1);
}

void OnEncoderMeasurement(const EncoderMeasurement* encoderMeasurement, uint8_t encoderNumber){
    // dataExchange->SendEncoderMeasurement(encoderMeasurement, encoderNumber);
    controllers[encoderNumber]->SetMeasuredOutput(encoderMeasurement->rpm);
}

void OnController1UpdateControlInput(const double controlInput){
  OnControllerUpdateControlInput(controlInput, 0);
}

void OnController2UpdateControlInput(const double controlInput){
  OnControllerUpdateControlInput(controlInput, 1);
}

void OnControllerUpdateControlInput(const double controlInput, uint8_t controllerNumber){
  // dataExchange->SendMessage(("CON" + String(controllerNumber) + ", INPUT SET " + String(controlInput)).c_str());
  dcMotors[controllerNumber]->SetSpeedPercent(controlInput);
}

void OnCommandReceived(const Command* command){
  // The following commands are motor-agnostic
  // Therefore no value checks need to be performed
  if(strcmp(command->instruction, "ENC") == 0){
    // Share last measurement of all encoders
    for(uint8_t enc_idx = 0; enc_idx < NUM_MOTORS; enc_idx++){
      dataExchange->SendEncoderMeasurement(
        rotaryEncoders[enc_idx]->GetLastMeasurement(),
        enc_idx);
    }
    return;
  }

  #if defined(TRIP_LLC_PARAMETERS_MANAGER_SUPPORTED)
  if(strcmp(command->instruction, "PSET") == 0){
    // Set the provided parameter to the given value
    dataExchange->SendMessage(("Setting parameter " + String(command->arg1) + " to value " + String(command->arg2)).c_str());
    bool res = parametersManager->SetVariable(command->arg1, command->arg2);
    if(res == true){
      dataExchange->SendMessage("Parameter value updated successfully");
    } else {
      dataExchange->SendMessage("Parameter value update FAIL");
    }
    return;
  }
  if(strcmp(command->instruction, "PLIST") == 0){
    // Provide the list of all parameters
    dataExchange->SendMessage("== BOARD PARAMETERS ==");
    dataExchange->SendMessage("Name | Value | Instantiation status");
    for(uint8_t i = 0; i < parametersManager->GetNumVariables(); i++){
      dataExchange->SendMessage(parametersManager->GetParameterDescription(i).c_str());
    }
    dataExchange->SendMessage("== End of table ==");
    return;
  }
  if(strcmp(command->instruction, "PERASE") == 0){
    // Reset all parameters stored to EEPROM
    // This is a non-ordinary operation to perform
    dataExchange->SendMessage("Erasing board parameters stored to EEPROM...");
    parametersManager->ResetAll();
    dataExchange->SendMessage("Done.");
    return;
  }
  #endif

  // The following commands are motor-specific
  // It is then necessary to assert beforehand that the index
  // of the motor of interest is valid
  uint8_t selectedItem = atoi(command->arg1);
  if(selectedItem < 0 or selectedItem >= NUM_MOTORS){
    return;
  }
  if(strcmp(command->instruction, "MSET") == 0){
    // Set directly motor percent speed
    controllers[selectedItem]->SetEnabled(false);
    dcMotors[selectedItem]->SetSpeedPercent(command->arg2);
    return;
  }
  if(strcmp(command->instruction, "CSET") == 0){
    // Define new absolute speed setpoint for motor controller
    controllers[selectedItem]->SetTarget(command->arg2);
    controllers[selectedItem]->SetEnabled(true);
    return;
  }
}
 
