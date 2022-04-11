/**
 * @file wearableDevice.ino
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief The main sketch for handling the wearable device.
 * @version 1.0
 * @date 11-04-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "WearableAccelerometer.h"
#include "WearableCommunications.h"

//! The MAC address of the walking aid device.
uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xA1, 0xA0, 0x84};

WearableAccelerometer accel;
WearableCommunications comms;

/**
 * @brief Handles the initialisation of the accelerometer and the communications objects and is the Arduino setup function.
 * 
 */
void setup() {
	Serial.begin(115200);
  accel.init(20, 15, 10, 30, 10000);
  comms.init("Reminder", broadcastAddress);
}

/**
 * @brief Calls the poll accelerometer function and calls to send a message to the walking aid device if the poll function returns true. Acts as the Arduino loop function.
 * 
 */
void loop() {

  if (accel.poll()) {
    comms.sendReminderMessage();
  }

}