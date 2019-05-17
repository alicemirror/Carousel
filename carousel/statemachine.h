/**
 * \file stateMachine.h
 * \briuef RotantingBalls state machine methods
 * 
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version V 1.0.0
 * \date May 2019
 */

#ifndef _STATEMACHINE
#define _STATEMACHINE

#include <Servo.h>
#include "globals.h"
#include "structs.h"

class StateMachine {
  private:
  //! Array of the servo pins
  //! Note that the first four indexes are normal servos while the fifth is a rotating servo
  int servoPins[NUMSERVOS] = { LIGHTSERVO_1_PIN, LIGHTSERVO_2_PIN, LIGHTSERVO_3_PIN, LIGHTSERVO_4_PIN, WHEEL_SERVO_PIN };
  
  //! Array of the servo library instances (one every servo)
  Servo servos[NUMSERVOS];

  public:
  //! Machine status
  MachineStatus m_Status;
  

  
  /**
   * Initialize the machine status parameters atn boot
   */
  void initStatus();
  
};

 #endif
