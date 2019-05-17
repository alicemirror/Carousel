/**
 * \file struct.h
 * \brief Data structures to control the status of the parameters
 * 
 * \date May 2019
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0
 * 
 */

#include "Arduino.h"

#ifndef _SRUCTS
#define _STRUCTS

//! Structure defining the status flags of the machine
typedef struct {
    boolean music;                      ///< The status of the mp3 player
    boolean pir;                        ///< The status of the PIR sensor
    boolean wheel;                      ///< The status of the rotating wheel
    int light;                 ///< The current light intensity
    int song;                     ///< The number of the current played song
} MachineStatus;

#endif
