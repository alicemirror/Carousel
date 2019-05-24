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
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
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
const char broker[]      = SECRET_BROKER;
const char* certificate  = SECRET_CERTIFICATE;

//! WiFi client used for the TCP socket connection
WiFiClient wifiClient;
//! Client used for SSL/TLS together with the ECC508
BearSSLClient sslClient(wifiClient);
//! MQTT client to connect the protocol to the broker
MqttClient mqttClient(sslClient);

//! Create an instance of the state machine class
StateMachine carousel;

//! Status message struct
IoTmessage statusIoT;

//! Setup and initialization
void setup() {
#ifdef _DEBUG
    Serial.begin(115200);
#endif

  carousel.initHardware();
  carousel.initStatus();

  if (!ECCX08.begin()) {
#ifdef _DEBUG
    Serial.println("No ECCX08 present!");
#endif
    while(1);
  }

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);
  
  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);

  // Optionally set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  // but we will use the client ID from remote to publish messages
  // to this specific client
  mqttClient.setId(MQTT_CLIENT_ID);

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);
} // Setup

//! Main loop method
void loop() {
  // Always check if the WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  // ... as well as the MQTT brokcer
  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();
  
  if(carousel.isPir() ) {
    // Sensor PIR has been activated, check if
    // it is time to disable it
    if(carousel.getElapsed() > CAROUSEL_CYCLE) {
      // The cycle should stop
      carousel.endCarousel();
      // Update hte IoT status
      updateIoTStatus();
    } // Time elapsed
  } // Pir is active
  else {
    // Check if there is a movement detection
    carousel.checkPirStatus();
    // If Pir status is true, the music is starting and we shoud
    // start the other stuff
    if(carousel.isPir()) {
      // If the carousel started, take into account of the current time
      // to calulate the total playing time
      statusIoT.timerStart = millis();
    }
  } // Movement detection

  // Update the hardware components, accordingly with 
  // the machine status
  carousel.updateHardware();
} // Main loop

// ======================================== IoT functions

  /**
   * Get the current time from the WiFi module (network time)
   */
  unsigned long getTime() {
      return WiFi.getTime();
  }

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
  Serial << "Attempting to MQTT broker: " << broker << " ";
#endif

  while (!mqttClient.connect(broker, MQTT_BROKER_PORT)) {
    // failed, retry
#ifdef _DEBUG
    Serial << ".";
#endif
    delay(CONN_DELAY);
  }

#ifdef _DEBUG
  Serial << endl << "MQTT broker connection established" << endl;
#endif

  // subscribe to a topic
  mqttClient.subscribe(MQTT_CLIENT_SUBSCRIBER);
}

//! Message reeived callback function
void onMessageReceived(int messageSize) {
#ifdef _DEBUG
  Serial << "Received a message '" << mqttClient.messageTopic() <<
  "', length " << messageSize << " bytes:" << endl;
#endif

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
#ifdef _DEBUG
    Serial << (char)mqttClient.read();
#endif
  }

  // Empty the message buffer
  mqttClient.flush();
  
#ifdef _DEBUG
  Serial << endl << endl;
#endif

  publishJsonIoTStatus();
}

// ======================================== IoT status functions

//! Initialize the status structure on startu
void initIoTStatus() {
  statusIoT.detections = 0;
  statusIoT.timerStart = 0;
  statusIoT.timePlayedUntilNow = 0;
  statusIoT.wheelRotations = 0;
  statusIoT.numSpheres = 0;  
}

//! Update the status structure. Called when a cycle ends
//! Note that the values of the status are updated in incremental
//1 mode so every new update all the clues expresses the global
//! sate of the carousel from the last startup
void updateIoTStatus() {
  //! The number of ms played by the last update
  float msPlayed;
  
  // Increment the number of detections
  statusIoT.detections++;
  // Update the played minutes
  msPlayed = millis() - statusIoT.timerStart;
  // Conver the played ms in minutes and increment the played
  //! minutes counter
  statusIoT.timePlayedUntilNow += msPlayed / 60000;
  //! Recalculate hte wheel rotations based on the RPM and the duration of a cycle
  statusIoT.wheelRotations += msPlayed / 60000 * WHEEL_RPM;
  //! Recalculate the number of spheres passed
  statusIoT.numSpheres += statusIoT.wheelRotations * SPHERES_PER_ROTATION;
}

//! Create the Json formatted IoT status message to send to the broker
//! And publish MQTT
void publishJsonIoTStatus() {
  String jPublish;
  
  // Create the message
  jPublish = String("{\n'detections': " + String(statusIoT.detections) + 
                    ",\n'minutes': " + String(statusIoT.timePlayedUntilNow) + 
                    ",\nrotations': " + String(statusIoT.wheelRotations) + 
                    ",\n'spheres': " + String(statusIoT.numSpheres) + "\n}");
  mqttClient.beginMessage(MQTT_CLIENT_PUBLISHER);
  mqttClient.print(jPublish);
  mqttClient.endMessage();
}
