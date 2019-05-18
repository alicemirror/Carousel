/**
 * \file stateMachine.cpp
 * \briuef Carousel state machine methods
 * 
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version V 1.0.1
 * \date May 2019
 */

#include "statemachine.h"

void StateMachine::initStatus() {
  m_Status.pir = false;
  m_Status.wheel = 0;
  m_Status.light = LOW_LIGHT;
  m_Status.timerStart = millis();
}

void StateMachine::checkPirStatus() {
  // Check for motion
  if(digitalRead(PIR_PIN)) {
    // Motion detected, trigger the mp3 player
    digitalWrite(MUSIC_TRIGGER_PIN, LOW);
    m_Status.timerStart = millis();
    setPir(true);
  }
}

void StateMachine::endCarousel() {
  // Disable the pir status
  setPir(false);
  // Reset the mp3 player trigger
  digitalWrite(MUSIC_TRIGGER_PIN, HIGH);
}

// -------- Getters and setters

void StateMachine::setPir(boolean s) {
  m_Status.pir = s;
}

void StateMachine::setWheelSpeed(int s) {
  m_Status.wheel = s;
}

void StateMachine::setLightIntensity(int i) {
  m_Status.light = i;
}

int StateMachine::getElapsed() {
  return int( (millis() - m_Status.timerStart) / 1000);
}

boolean StateMachine::isPir() {
  return m_Status.pir;
}

int StateMachine::getWheel() {
  return m_Status.wheel;
}

int StateMachine::getLight() {
  return m_Status.light;
}
