/**
 * \file stateMachine.h
 * \briuef Carousel state machine methods
 * 
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version V 1.0.1
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

  //! Machine status
  MachineStatus m_Status;

  public:
  /**
   * Return the time elapsed (in seconds) after the last rime reading. Time is 
   * read when the PIR status changes due a motion detection. The internal calculations
   * are done with unsigned long milliseconds while the result is in int seconds
   * 
   * @return the elapsed time in seconds
   */
  int getElapsed();

  /**
   * Return true or false accordingly with the current pir status. After motion has been deteced
   * the sensor status persists for CAROUSEL_CYCLE seconds
   * 
   * @return the sensor status
   */
  boolean isPir();

  /**
   * Get the current wheel speed
   */
  int getWheel();

  /**
   * Get the current light level
   */
  int getLight();

  /**
   * Set the pir status. Logical value depending on the last hardware read
   */
  void setPir(boolean s);

  /**
   * Set the current wheel speed
   */
  void setWheelSpeed(int s);

  /**
   * Set the current light intensity
   */
  void setLightIntensity(int i);

  /**
   * Check if PIR has detected a motion. If true, the mp3 player is triggered
   * and the timer counter is initialized.
   */
  void checkPirStatus();

  /**
   * Disable the pir status and stop the carousel
   */
  void endCarousel();
  
  /**
   * Initialize the machine status parameters atn boot
   */
  void initStatus();
  
};

 #endif
