


#include "statemachine.h"

void StateMachine::initStatus() {
  m_Status.music = false;
  m_Status.pir = false;
  m_Status.wheel = false;
  m_Status.light = LOW_LIGHT;
  m_Status.song = 1;
}
