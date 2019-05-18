/**
 * \file struct.h
 * \brief Data structures to control the status of the parameters
 * 
 * \date May 2019
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0.1
 * 
 */

#include "Arduino.h"

#ifndef _SRUCTS
#define _STRUCTS

//! Structure defining the status flags of the machine
typedef struct {
    boolean music;             ///< The status of the mp3 player
    boolean pir;               ///< The status of the PIR sensor
    int wheel;                 ///< The rotating wheel speed
    int light;                 ///< The current light intensity
    /**
     * Reading of the timer when the PIR status has been detected
     * It is reset everytime the pir status is read positive
     */
    unsigned long timerStart;
} MachineStatus;

#endif
