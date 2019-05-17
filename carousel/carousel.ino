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

//! Setup and initialization
void setup() {
  pinMode(MUSIC_TRIGGER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

}

void loop() {
  if(digitalRead(PIR_PIN)) {
    digitalWrite(MUSIC_TRIGGER_PIN, LOW);
  } else {
    digitalWrite(MUSIC_TRIGGER_PIN, HIGH);
  }
}

boolean isMotion() {
  if(digitalRead(PIR_PIN) == HIGH) {
    return true;
  } else {
    return false;
  }
}
