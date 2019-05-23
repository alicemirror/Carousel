/**
 * \file RotatingBalls.ino
 * \brief The Arduino MKR100 module of the carousel projec
 * 
 * The software design approach of this program is of a satus machine where the change of
 * some of the control statuses (e.g. the PIR presence detection) changes the status of the
 * other related parameters (wheel rotation, light intensity, music starting)
 * 
 * \date May 2019
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.0
 * 
 */
#include "statemachine.h"

// Create an instance of the state machine class
StateMachine carousel;

//! Setup and initialization
void setup() {
  carousel.initHardware();
  carousel.initStatus();
}

//! Main loop method
void loop() {
  if(carousel.isPir() ) {
    // Sensor PIR has been activated, check if
    // it is time to disable it
    if(carousel.getElapsed() > CAROUSEL_CYCLE) {
      // The cycle should stop
      carousel.endCarousel();
    } // Time elapsed
  } // Pir is active
  else {
    // Check if there is a movement detection
    carousel.checkPirStatus();
    // If Pir status is true, the music is starting and we shoud
    // start the other stuff
  } // Movement detection

  // Update the hardware components, accordingly with 
  // the machine status
  carousel.updateHardware();
}
