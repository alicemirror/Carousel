/**
 * \file stateMachine.h
 * \briuef Carousel state machine methods. Integrates remote commands via MQTT protocol
 * 
 * This version of the StateMachine class include the optoins of remote commands
 * They can be used in any way by the program but these are sort of "dmoe" funcionts with
 * shorter duration. Their typical usage is to show the features of the carousel when they
 * are managed from remote. Remote control acts through the MQTT protocol.\n
 * For better understanding the remote command methods have the prefix "mqtt" 
 * 
 * \note the remote commands interact with the state of the mqtt flag in the state machine
 * structure MachineStatus (flag mqtt) and automatically disable the PIR sensor effect.
 * When a remote command is received the mqtt flag is enabled, and inhibits the PIR sensor
 * until the remote command seqence has not been completed. The local behavior controller
 * by the PIR sensor and the mqtt remote commands are mutually exclusive.
 * 
 * 
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version V 1.0.2
 * \date May 2019
 */

#ifndef _STATEMACHINE
#define _STATEMACHINE

#include <Servo.h>
#include "globals.h"
#include "structs.h"

//! The state machine class control the behavior of all the hardware and the
//! logic of movements accordingly with the PIR sensor
class StateMachine {
  private:
  //! Array of the servo pins
  //! Note that the first four indexes are normal servos while the fifth is a rotating servo
  int servoPin[NUMSERVOS] = { LIGHTSERVO_1_PIN, LIGHTSERVO_2_PIN, LIGHTSERVO_3_PIN, LIGHTSERVO_4_PIN, WHEEL_SERVO_PIN };
  
  //! Array of the light pins
  //! Light intensity is controlled through PWM so the pins are selected accordingly
  int lightPin[NUMLIGHTS] = { LIGHT_1_PIN, LIGHT_2_PIN, LIGHT_3_PIN, LIGHT_4_PIN };

  //! Array of the servo library instances (one every servo)
  Servo servos[NUMSERVOS];

  //! Machine status
  MachineStatus m_Status;

  /**
   * Set the speed of the wheel (rotating servo) accordingly with the
   * status of the machine.
   */
  void setWheelRotation();

  /**
   * Update the light intensity, accordingly with
   * the machine status light value
   */
  void setLightIntensity();

  /**
   * Check if the delaty between two 1 Deg light servo rotation
   * has passed. If the delay has been reached the servo(s) are moved
   * by one step and the time counter is updated.
   */
  void servoLightTimeToMove();

  /**
   * Update the ligth servos position by one step
   */
  void stepLightServo();

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
   * Return true or false accordingly with the remote command status (mqtt flagh).
   * 
   * @return the mqtt remote command flag status
   */
  boolean mqttIsMqtt();

  /**
   * Get the current wheel speed
   */
  int getWheel();

  /**
   * Get the current light level
   */
  int getLight();

  /**
   * Set the pir status. Logical value depends on the last hardware read
   */
  void setPir(boolean s);

  /**
   * Set the remote command mqtt flag status.
   */
  void mqttSetMqtt(boolean s);

  /**
   * Set the current wheel speed
   */
  void setWheelSpeed(int s);

  /**
   * Set the current light intensity
   * 
   * @param i light intensity
   */
  void setLight(int i);

  /**
   * Check if PIR has detected a motion. If true, the mp3 player is triggered
   * and the timer counter is initialized.
   * 
   * \Note To make easy the program main loop control, the PIR status take into
   * account of the mqtt remote command flag status.
   */
  void checkPirStatus();

  /**
   * Check if a remote command (via the mqtt protocol) has been received. 
   * If true, the specific features are executed accordingly with the
   * mqtt received message.
   */
  void mqttCheckStatus();

  /**
   * When a remote command via mqtt is received (via the callback function)
   * the normal status of the operation is interrupted the next loop cycle.
   * 
   * \note The MQTT callback function only set the parameters in the MachineState
   * structure. If the mqtt flag is set also a command is passed to the satus
   * structure, to be executed the next loop cycle the check MQTT status is
   * called.
   */
  void mqttExecCommand();

  /**
   * Set the command ID to the machine status structure
   * 
   * @param cmd The command ID
   */
  void mqttSetCommand(int cmd);

  /**
   * Executes a series of musics on the player starting from the last music played.
   * 
   * The mp3 player is a different board whoe behavior is triggered by a pin
   * so the easy action to do it trigger the pin multiple times. Every time
   * a music start/end on the mp3 player if moves to the next in its own
   * internal list. That's all.
   */
  void mqttCmdMusic();

  /**
   * Shows the lights (high intensity) for a limited period of time
   * 
   * @param to The amount of seconds the light should be shown
   */
  void mqttCmdLights(int to);

  /**
   * Power on the lights
   */
  void mqttCmdLights();

  /**
   * Executes a complete sequence: power on the lights, plays some music then goes off.
   */
  void mqttCmdRun();

  /**
   * Disable the pir status and stop the carousel
   */
  void endCarousel();

  /**
   * Disable the mqtt remote command flah and stop the carosel
   */
  void mqttEndCarousel();
  
  /**
   * Initialize the machine status parameters atn boot
   */
  void initStatus();

  /**
   * Initialize the hardware pins and set the hardware servos and 
   * lights to the initial state
   */
  void initHardware();

  /**
   * Update the hardware components (servos, lilghts) accordingly
   * with the status of the machine
   */
  void updateHardware();
};

 #endif
