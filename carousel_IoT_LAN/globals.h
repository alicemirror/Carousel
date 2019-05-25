/**
 * \file const.h
 * \brief constants and profile defaults for the RataingBalls + Lights and Music
 * 
 * \date May 2019
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0.2
 * 
 */

#ifndef _GLOBALS
#define _GLOBALS

// Undef to avoid serial output of debug
#define _DEBUG

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

#define MUSIC_TRIGGER_PIN 10   ///< Start the music until the signal is low
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
#define WHEEL_STOP 90       ///< Wheel ratating servo stopped
#define MIN_ANGLE 10        ///< Minimum servo angle
#define MAX_ANGLE 90       ///< Maximum servo angle
#define LOW_LIGHT 5        ///< Light intensity when the system is in standby
#define HIGH_LIGHT 200      ///< Light intensity when the system has been activated
#define PIR_ENABLED 1       ///< PIR sensor pin when a presence is detected
#define PIR_DISABLED 0      ///< PIR sensor pint when no presence is detected
#define CAROUSEL_CYCLE 40   ///< motion, lights and music duration (sec) ENABLED BY PIR
#define SERVO_CYCLE 25      ///< Light servo rotation delaty every 1 angle step

// ========================================== IoT constants

#define MQTT_BROKER_PORT 8883 ///< Remote port to connect to the broker via MQTT protocol (standard)
#define CONN_DELAY 5000  ///< Delay (ms) while trying multiple times to connect
//! The subscriber topic for the carousel thing topic
#define MQTT_CLIENT_SUBSCRIBER "carousel/start"

#define MQTT_LIGTHS "mqtt_lights"   ///< Command to start lights
#define MQTT_MUSIC "mqtt_music"     ///< Command to start music
#define MQTT_RUN "mqtt_run"         ///< Command to run the carousel (short time)

#define MQTT_MUSIC_TIMEOUT 5                ///< Duration of a piece of music (command mqtt_music)
#define MQTT_MUSIC_PLAY_SONGS 5             ///< Number of songs played by mqtt music command
#define MQTT_LIGHTS_TIMEOUT 10              ///< Duration of the command mqtt_lights

#define MQTTCMD_LIGTHS 0X01        ///< Start lights ID
#define MQTTCMD_MUSIC 0X02         ///< Start music ID
#define MQTTCMD_RUN 0X03           ///< Run the carousel short time ID

#endif
