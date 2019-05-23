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
  m_Status.wheel = WHEEL_STOP;
  m_Status.isRotating = false;
  m_Status.timerServo = millis();
  m_Status.rotationDirA = CLOCKWISE;
  m_Status.rotationDirB = COUNTERCLOCKWISE;
  m_Status.servoPos[LIGHT1] = MIN_ANGLE;
  m_Status.servoPos[LIGHT3] = MIN_ANGLE;
  m_Status.servoPos[LIGHT2] = MAX_ANGLE;
  m_Status.servoPos[LIGHT4] = MAX_ANGLE;
}

void StateMachine::initHardware() {
#ifdef _DEBUG
  Serial.begin(9600);
#endif
  int j;

  pinMode(MUSIC_TRIGGER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT_PULLUP);

  // Initialize the lights to the minimum value
  for(j = 0; j < NUMLIGHTS; j++) {
    pinMode(lightPin[j], OUTPUT);
    analogWrite(lightPin[j], LOW_LIGHT);
  }

  // Attach the servos to the corresponding pins
  for(j = 0; j < NUMSERVOS; j++) {
      servos[j].attach(servoPin[j]);
  }
  
  // Position the four light servos at the initial point
  servos[LIGHT1].write(m_Status.servoPos[LIGHT1]);
  servos[LIGHT2].write(m_Status.servoPos[LIGHT2]);
  servos[LIGHT3].write(m_Status.servoPos[LIGHT3]);
  servos[LIGHT4].write(m_Status.servoPos[LIGHT4]);

  // Set the wheel stopped
  servos[WHEEL].write(WHEEL_STOP);    
}

void StateMachine::updateHardware() {
  // Check for the PIR conditional hardware changes
  if( (m_Status.pir == true) && (m_Status.isRotating == false) ){
    // Should start the rotating wheel
    m_Status.isRotating = true;
    setWheelRotation();
    setLightIntensity();
  } else {
    if( (m_Status.pir == false) && (m_Status.isRotating == true) ){
      // Stop the rotating wheel
      m_Status.isRotating = false;
      setWheelRotation();
      setLightIntensity();
    } // No pir and wheel rotates
  } // Pir rotates and wheel is stopped

  // Check for the light servo rotation interval
  // end eventually moved the servos
  if(m_Status.pir == true) {
    servoLightTimeToMove();
  }
}

void StateMachine::setWheelRotation() {
  servos[WHEEL].write(m_Status.wheel);
}

void StateMachine::checkPirStatus() {
  // Check for motion
  if(digitalRead(PIR_PIN)) {
    // Motion detected, trigger the mp3 player
    // and start the timeout counter
    digitalWrite(MUSIC_TRIGGER_PIN, LOW);
    setLight(HIGH_LIGHT);
    setWheelSpeed(WHEEL_CAROUSEL);
    m_Status.timerStart = millis();
    setPir(true);
  }
  else {
    // Disable Trigger pin and set the PIR flag accordingly
    endCarousel();
  }
}

void StateMachine::endCarousel() {
  // Disable the pir status
  setPir(false);
  // Reset the mp3 player trigger and disable the other stuff
  digitalWrite(MUSIC_TRIGGER_PIN, HIGH);
  setLight(LOW_LIGHT);
  setWheelSpeed(WHEEL_STOP);
}

void StateMachine::setLightIntensity() {
  int j;
  for(j = 0; j < NUMLIGHTS; j++) {
    analogWrite(lightPin[j], m_Status.light);
  }
}

void StateMachine::servoLightTimeToMove() {
  // Check the elapsed time in milliseconds
  if( (millis() - m_Status.timerServo) >= SERVO_CYCLE) {
    // Time passes, servos should move
    stepLightServo();
    m_Status.timerServo = millis(); // Update the time reading
  }
}

void StateMachine::stepLightServo() {
  // Here we check only one increment as the second light position
  // should be the same a rotations are coupled in the order 1-3- 2-4
  m_Status.servoPos[LIGHT1] += m_Status.rotationDirA;
  m_Status.servoPos[LIGHT3] += m_Status.rotationDirA;
  if(m_Status.servoPos[LIGHT1] >= MAX_ANGLE) {
    m_Status.servoPos[LIGHT1] = MAX_ANGLE - 1;
    m_Status.servoPos[LIGHT3] = MAX_ANGLE -1 ;
    m_Status.rotationDirA = COUNTERCLOCKWISE;
  }
  if(m_Status.servoPos[LIGHT1] <= MIN_ANGLE) {
    m_Status.servoPos[LIGHT1] = MIN_ANGLE + 1;
    m_Status.servoPos[LIGHT3] = MIN_ANGLE +1 ;
    m_Status.rotationDirA = CLOCKWISE;
  }

  // Do the same on the other couple of servos  
  m_Status.servoPos[LIGHT2] += m_Status.rotationDirB;
  m_Status.servoPos[LIGHT4] += m_Status.rotationDirB;
  if(m_Status.servoPos[LIGHT2] >= MAX_ANGLE) {
    m_Status.servoPos[LIGHT2] = MAX_ANGLE - 1;
    m_Status.servoPos[LIGHT4] = MAX_ANGLE -1 ;
    m_Status.rotationDirB = COUNTERCLOCKWISE;
  }
  if(m_Status.servoPos[LIGHT2] <= MIN_ANGLE) {
    m_Status.servoPos[LIGHT2] = MIN_ANGLE + 1;
    m_Status.servoPos[LIGHT4] = MIN_ANGLE +1;
    m_Status.rotationDirB = CLOCKWISE;
  }
  // Update the servos position  
  servos[LIGHT1].write(m_Status.servoPos[LIGHT1]);
  servos[LIGHT3].write(m_Status.servoPos[LIGHT3]);
  servos[LIGHT2].write(m_Status.servoPos[LIGHT2]);
  servos[LIGHT4].write(m_Status.servoPos[LIGHT4]);
}

// -------- Getters and setters

void StateMachine::setPir(boolean s) {
  m_Status.pir = s;
}

void StateMachine::setWheelSpeed(int s) {
  m_Status.wheel = s;
}

void StateMachine::setLight(int i) {
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
