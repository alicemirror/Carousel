/**
 * \file globals.h
 * \brief Program constants and static definitions
 */

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#ifndef _GLOBALS
#define _GLOBALS

#define TRIGGER_PIN 2   ///< Pin that trigger the playing of a song
#define TRIGGER_LED 13  ///< LED to monitor the state of the trigger

#define START_VOLUME 3 ///< Initial volume for the fade in/out when a new song starts
/**
 * The maximum volume level is proportional. It is the absolue limits that can be reached but
 * maybe lower, depending on the settings of the trimmer connected to the analog port to regulate
 * the effective playing volume (depending on the noise of the environmnet)
 */
#define MAX_VOLUME 80

#define MAX_SONGS 13                    ///< Max number of songs, starting counting from 1
#define EQUALIZATION DFPLAYER_EQ_POP    ///< The equalization kind
#define MP3_DEVICE DFPLAYER_DEVICE_SD   ///< if used an USB instead, change this value accordingly
#define SERIAL_SPEED 9600               ///< Communication speed (see DFPlayer datasheet)
#define SERIAL_TIMEOUT 500              ///< Player communication timeour (ms)
#define COMMAND_DELAY 800               ///< Delay betwee commands (ms)

#define ANALOG_FADE_PIN 0               ///< Analog port to read the max value for the fade in/out
#define FADE_IN 1                       ///< Volume fade in direction
#define FADE_OUT -1                     ///< Volume fade out direction

/**
 * Sound control parameters and flags to manage the different playing states
 * The structure is initializd with the defaults and initial values on boot
 */
typedef struct {
  int currentSong = 0;              ///< The song that is playing or the last played, initially no song
  int volume = START_VOLUME;        ///< The current playing volume
  int maxVolume = START_VOLUME;    ///< The duynamic max volume level
  boolean trigger = false;          ///< The trigger status
  boolean isPlaying = false;      ///< Current playing status (condition the trigger behavior)
} SoundControl;

 #endif
