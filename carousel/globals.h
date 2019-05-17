/**
 * \file const.h
 * \brief constants and profile defaults for the RataingBalls + Lights and Music
 * 
 * \date May 2019
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0
 * 
 */

#ifndef _GLOBALS
#define _GLOBALS

// ========================================== Hardware settings

#define LIGHTSERVO_1_PIN 0    //! Light spot rotating servo
#define LIGHTSERVO_2_PIN 1    //! Light spot rotating servo
#define LIGHTSERVO_3_PIN 2    //! Light spot rotating servo
#define LIGHTSERVO_4_PIN 3    //! Light spot rotating servo
#define WHEEL_SERVO_PIN 4    //! Continuous servo rotating wheel
#define SPOT_LIGHTS_PIN 5    //! PWM intensity control of the spot lights
#define NUMSERVOS 5          //! Total number of servos to manage them in an array

#define MP3_TX_PIN 14   //! Serial connection to the mp3 player
#define MP3_RX_PIN 13   //! Serial connection to the mp3 player

#define PIR_PIN 9       //! PIR sensor input

// ========================================== Default values

#define WHEEL_SPEED 10      ///< Rotating servo speed
#define MIN_ANGLE 20        ///< Minimum servo angle
#define MAX_ANGLE 150       ///< Maximum servo angle
#define LOW_LIGHT 50        ///< Light intensity when the system is in standby
#define HIGH_LIGHT 200      ///< Light intensity when the system has been activated
#define PIR_ENABLED 1       ///< PIR sensor pin when a presence is detected
#define PIR_DISABLED 0      ///< PIR sensor pint when no presence is detected
#define NUM_SONGS 13        ///< Number of tracks on the mpt3 player microSD

#endif
