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
    boolean isRotating;        ///< Wheel status
    int light;                 ///< The current light intensity
    int servoPos[NUMLIGHTS];   ///< Last positon of the light rotating servos
    /** 
     * Rotation direction of type "A". 
     * 
     * To rotate the light crossing the colors here are used to directions
     * initially set to the opposite starting points and with opposite
     * increments (+1 / -2)
     */
    int rotationDirA;          
    /** 
     * Rotation direction of type "A". 
     * 
     * To rotate the light crossing the colors here are used to directions
     * initially set to the opposite starting points and with opposite
     * increments (+1 / -2)
     */
    int rotationDirB;
    /**
     * Reading of the timer when the PIR status has been detected
     * It is reset everytime the pir status is read positive
     */
    unsigned long timerStart;
    /**
     * Laset millis read to temporize the servo lights rotation
     * continuously. The value is checked/updated everytime the 
     * updateHardware() is called.
     */
    unsigned long timerServo;
} MachineStatus;

#endif
