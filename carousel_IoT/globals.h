/**
 * \file const.h
 * \brief constants and profile defaults for the RataingBalls + Lights and Music
 * 
 * \date May 2019
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0.1
 * 
 */

#ifndef _GLOBALS
#define _GLOBALS

// ========================================== Hardware settings

#define LIGHTSERVO_1_PIN 0    //! Light servo
#define LIGHTSERVO_2_PIN 1    //! Light servo
#define LIGHTSERVO_3_PIN 2    //! Light servo
#define LIGHTSERVO_4_PIN 8    //! Light servo
#define WHEEL_SERVO_PIN 3     //! Continuous servo rotating wheel

#define LIGHT_1_PIN 4    //! Light PWM control
#define LIGHT_2_PIN 5    //! Light PWM control
#define LIGHT_3_PIN 6    //! Light PWM control
#define LIGHT_4_PIN 7    //! Light PWM control

#define NUMSERVOS 5          //! Total number of servos to manage them in an array
#define NUMLIGHTS 4         //! Total number of lights

#define MUSIC_TRIGGER_PIN 11   ///< Start the music until the signal is low
#define PIR_PIN 9       //! PIR sensor input

// ========================================== Default values

#define LIGHT1 0    ///< Servo index in the pin array
#define LIGHT2 1    ///< Servo index in the pin array
#define LIGHT3 2    ///< Servo index in the pin array
#define LIGHT4 3    ///< Servo index in the pin array
#define WHEEL 4     ///< Servo index in the pin array

#define CLOCKWISE 1         ///< Clockwise roation increment
#define COUNTERCLOCKWISE -1 ///< Counterclockwise roation increment
#define WHEEL_CAROUSEL 110 ///< Wheel rotating servo speed
#define WHEEL_STOP 95       ///< Wheel ratating servo stopped
#define MIN_ANGLE 10        ///< Minimum servo angle
#define MAX_ANGLE 90       ///< Maximum servo angle
#define LOW_LIGHT 5        ///< Light intensity when the system is in standby
#define HIGH_LIGHT 200      ///< Light intensity when the system has been activated
#define PIR_ENABLED 1       ///< PIR sensor pin when a presence is detected
#define PIR_DISABLED 0      ///< PIR sensor pint when no presence is detected
#define CAROUSEL_CYCLE 40   ///< motion, lights and music duration (sec) ENABLED BY PIR
#define SERVO_CYCLE 25      ///< Light servo rotation delaty every 1 angle step

#endif
