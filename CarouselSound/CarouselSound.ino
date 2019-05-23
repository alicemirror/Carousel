/**
 * \file CarouselSound.ino
 * \brief Arduino Nano mp3 player dedicated to create the soundtrack (from original live recordings)
 * of the Carousel cage project.
 * 
 * Due problems to control seriously the DF mp3 player with Arduino MKR1000 a dedicated custom mp3
 * player ased on this board has been designed. It is controlled by a trigger pin by the MKR1000
 * that on request only send the input to start the play operation. The soundtrack management sequence
 * is automatically started on-demand (by the triggered pin) and executes the piece from the sequence,  
 * changing it everytime for about 60 seconds  including dissolve in and out. 
 * 
 * @note The player uses the same serial used also to program the Nano via USB to when a new program should
 * be uploaded the serial wires should be disconnected. For more details on the DFPlayerMini Arduino library 
 * please read the GitHub (sources docmentation includes more detailed information than the wiki). Read the 
 * DFRobotDFPlayerMini.h header file fo the constant definitions to set all the parameters.\n
 * Using Arduino Mega or Mega2560 platforms you can set the player serial to one different than the USB serial
 * e.g. Serial1
 * 
 * @warning If you copy the files on the microSD card with a Mac OS remember to launch from the terminal
 * the command dot_clean /Volumes/<SDVolumeName> before removind the microSD card. This deletes the dot files
 * automatically generated by the OSX that create noisy silent playing of undesired ghost files.
 * 
 * @note The mp3 files should be named in a numerical sequence 001.mp3 ... xxx.mp3. Files can be organized in folders
 * named in sequence 01 .. nn and every folder can contain a max of 255 files.
 * The DF PLayer support microSD cards up to 32 Gb. For more details read the datasheet in the Doc folder of the
 * repository.
 * 
 * This source and the other components of the carousel project are available on GitHub pages at the address
 * <https://alicemirror.girhub.io/Carousel
 * 
 * @author Enico Miglino <balearicdynamics@gmail.com>
 * @license LGPL 3.0
 * @versione 1.0.0
 * @date May 2019
 */

#include "globals.h"

//! Instance of the DF mp player class
DFRobotDFPlayerMini libDFPlayer;
//! Player control structure
SoundControl playerControl;

// =============================================================
//                    Player Functions
// =============================================================

//! Initialize the player start parameters
void initPlayer() {
  //Set serial communictaion time out 500ms
  libDFPlayer.setTimeOut(SERIAL_TIMEOUT); 
  //----Set device we uset----
  libDFPlayer.reset();     //Reset the module
  delay(COMMAND_DELAY);
  //----Set volume----
  libDFPlayer.volume(playerControl.volume);  //Set volume value (0~30).
  delay(COMMAND_DELAY);
  //----Set Equalization----
  libDFPlayer.EQ(EQUALIZATION);
  delay(COMMAND_DELAY);
  //----Set device we uset----
  libDFPlayer.outputDevice(MP3_DEVICE);
  delay(COMMAND_DELAY);
}

/**
 * Read the dynamic max volume target and update the sructure
 */
void getDynamicVolume() {
  int analogVol = analogRead(ANALOG_FADE_PIN);
  // Map the analog value to the absolute max volume
  // on a scale from 0 to 1024
  playerControl.maxVolume = map(analogVol, 0, 1023, START_VOLUME, MAX_VOLUME);
}

// Check if the dynamics volume has changed. If so, update the
// value accordingly
void checkDynamicVolume() {
  int analogVol = analogRead(ANALOG_FADE_PIN);
  // Map the analog value to the absolute max volume
  // on a scale from 0 to 1024
  int mappedVolume = map(analogVol, 0, 1023, START_VOLUME, MAX_VOLUME);
  // If volume has changed update the volme of the player
  if(mappedVolume > playerControl.maxVolume) {
    // Volume shoud grow
    fadeVolume(FADE_IN);
  } else {
    // Volume shoud go lower
    fadeVolume(FADE_OUT);
  }
}

/**
 * Fade in or out the volume, accordingly with the parameter, in the min and max volume range
 * 
 * @param inout If inout is > 0 the volume is fade in else it is fadeout
 */
void fadeVolume(int inout) {
  int j;

  getDynamicVolume();
  
  if (inout == FADE_IN) {
    // fade from current volume to max
    for(j = playerControl.volume; j <= playerControl.maxVolume; j++) {
      playerControl.volume = j;
      libDFPlayer.volume(playerControl.volume);
    }
  }
  else {
    // fade from current volume to min
    for(j = playerControl.volume; j >= START_VOLUME; j--) {
      playerControl.volume = j;
      libDFPlayer.volume(playerControl.volume);
    }
  }
    
}

/**
 * Check the status of the player trigger and return the corresponding boolean status
 */
void isTrigger() {
  if(digitalRead(TRIGGER_PIN) == LOW) {
    digitalWrite(TRIGGER_LED, HIGH);
    playerControl.trigger = true;  
  }
  else {
    digitalWrite(TRIGGER_LED, LOW);
    playerControl.trigger = false;  
  }
}

// =============================================================
//                    setup() & loop()
// =============================================================

//! Initialization function
void setup() {
  Serial.begin(SERIAL_SPEED);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(TRIGGER_LED, OUTPUT);

  libDFPlayer.begin(Serial);
  delay(1000);

  // Wait untile the Arduino serial connected to the player
  if (!libDFPlayer.begin(Serial)) {
    while(true);
  }

  initPlayer();
}

/**
 * Main loop
 */
void loop()
{
  // Check the start/stop trigger
  isTrigger();

  // The trigger is active
  if(playerControl.trigger == true) {
    // Check if is already playing
    if(playerControl.isPlaying == false) {
      // Update the song
      playerControl.currentSong++;
      if(playerControl.currentSong > MAX_SONGS) {
        // Reset the song counter
        playerControl.currentSong = 1;
      } // Updated player status and song
      playerControl.isPlaying = true;
      // Start playing with a fadein the new song and loop
      // until the trigger is not reset
      libDFPlayer.loop(playerControl.currentSong);
      fadeVolume(FADE_IN);
    } // Playing started
//    else { // Not used
//      // Is already playing, check if the target volume has changed
//      checkDynamicVolume();
//    }
  } // Trigger is set
  else {
    // Trigger is not active, check if is playing and fade out the song
    if(playerControl.isPlaying == true)
    // Fade out the song
    fadeVolume(FADE_OUT);
    // Put the player in pause
    libDFPlayer.pause();
    playerControl.isPlaying = false;
  } // Playing stopped
}

void printDetail(uint8_t type, int value){
  
}
