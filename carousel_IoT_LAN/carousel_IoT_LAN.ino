/**
 * \file carousel.ino
 * \brief The Arduino MKR100 module of the carousel projec
 * 
 * The software design approach of this program is of a satus machine where the change of
 * some of the control statuses (e.g. the PIR presence detection) changes the status of the
 * other related parameters (wheel rotation, light intensity, music starting)
 * 
 * \note This version acts autonomously based on the PIR status and will start the carousle
 * \ also when receivin a command by MQTT.
 * 
 * \date May 2019
 * \author Enrico Miglino <balearicdynamics@gmail.com>
 * \version 1.2
 * 
 */

#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <MQTT.h>
#include <WiFi101.h>

#include "globals.h"
#include "carouselsecrets.h"
#include "statemachine.h"
#include "structs.h"

#ifdef _DEBUG
#include "Streaming.h"
#endif

const char ssid[]        = SECRET_SSID;
const char pass[]        = SECRET_PASS;
const char mqttServer[] = SECRET_BROKER;
//! Working on a local network we don't use key, device name and secrect key
//! as well as certificates
const char key[] = " ";
const char secret[] = " ";  ///< No protection in LAN (see key note)
const char device[] = " ";  ///< No device in LAN (see key not)

//! WiFi client used for the LAN connection
WiFiClient wifiClient;
//! MQTT client to connect the protocol to the broker
MQTTClient mqttClient;

//! Create an instance of the state machine class
StateMachine carousel;

//! Setup and initialization
void setup() {
#ifdef _DEBUG
    Serial.begin(115200);
#endif
  carousel.initHardware();
  carousel.initStatus();
} // Setup

//! Main loop method
void loop() {
  // Always check if the WiFi is connected
  // and eventually reconnect
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  // ... as well as the MQTT brokcer
  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // If a remote command has been sent, ignore the local process
  // and process it then continue normally
  carousel.mqttCheckStatus();
  
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
    if(carousel.isPir()) {
    }
  } // Movement detection

  // Update the hardware components, accordingly with 
  // the machine status
  carousel.updateHardware();
} // Main loop

// ======================================== IoT functions

//! Try to connect to the WiFi and retry after a delay if connection is not possible
void connectWiFi() {
#ifdef _DEBUG
  Serial << "Attempting to connect to SSID: " << ssid << " ";
#endif

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
#ifdef _DEBUG
    Serial << ".";
#endif
    delay(CONN_DELAY);
  }
#ifdef _DEBUG
  Serial << endl << "You're connected to the network" << endl;
#endif
}

void connectMQTT() {
#ifdef _DEBUG
  Serial << "Attempting to MQTT broker: " << mqttServer << " ";
#endif

  //! Start the client on the local LAN
  mqttClient.begin(mqttServer, MQTT_BROKER_PORT, wifiClient);

  //! Connect the client to the server broker
  while (!mqttClient.connect(mqttServer, MQTT_BROKER_PORT)) {
    // failed, retry
#ifdef _DEBUG
    Serial << ".";
#endif
    delay(CONN_DELAY);
  }

#ifdef _DEBUG
  Serial << endl << "MQTT broker connection established" << endl;
#endif

  // Acrtivate the message callback
  mqttClient.onMessage(onMessageReceived);

  // subscribe to a topic
  mqttClient.subscribe(MQTT_CLIENT_SUBSCRIBER);
}

//! Message reeived callback function
void onMessageReceived(String &topic, String &payload) {
#ifdef _DEBUG
  Serial << "Received the message " << topic <<
  ": " << payload << endl;
#endif

  // Validate the topic
  if(topic.equals(MQTT_CLIENT_SUBSCRIBER) ) {
    // Set the correct command ID (if recognized)
    if(payload.equals(MQTT_LIGTHS) ) {
      carousel.mqttSetMqtt(true);
      carousel.mqttSetCommand(MQTTCMD_LIGTHS);
    } // Cmd lights
    if(payload.equals(MQTT_MUSIC) ) {
      carousel.mqttSetMqtt(true);
      carousel.mqttSetCommand(MQTTCMD_MUSIC);
    } // Cmd music
    if(payload.equals(MQTT_RUN) ) {
      carousel.mqttSetMqtt(true);
      carousel.mqttSetCommand(MQTTCMD_RUN);
    } // Cmd run
  } // Topic validated
}
