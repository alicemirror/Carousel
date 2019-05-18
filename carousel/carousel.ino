/**
 * \file RotatingBalls.ino
 * \brief The Arduino MKR100 version of the rotating boar with
 * lights and music.
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
  pinMode(MUSIC_TRIGGER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

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
  } // Movement detection
}

boolean isMotion() {
  if(digitalRead(PIR_PIN) == HIGH) {
    return true;
  } else {
    return false;
  }
}
