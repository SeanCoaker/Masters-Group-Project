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


//! A copy of the message received from the walking aid device.
String message;
//! The MAC address of the walking aid device.
uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xA1, 0xA0, 0x84};

WearableAccelerometer accel;
WearableCommunications comms;

TinyPICO tp = TinyPICO();

/**
 * @brief Callback function to be executed when data is received from a vibrate message.
 * 
 * @param mac MAC address of the device that sent the vibrate message.
 * @param data Data of the vibrate message.
 * @param len Length of the vibrate message.
 */
void OnDataRecv(const uint8_t * mac, const uint8_t * data, int len) {
  memcpy(&message, data, sizeof(message));
  Serial.println(message);
  tp.DotStar_SetPixelColor(230, 230, 250);
  tp.DotStar_SetBrightness(128);
  tp.DotStar_Show();
  delay(1000);
  tp.DotStar_SetBrightness(0);
  tp.DotStar_Show();
}

/**
 * @brief Handles the initialisation of the accelerometer and the communications objects and is the Arduino setup function.
 * 
 */
void setup() {
	Serial.begin(115200);
  accel.init(20, 15, 10, 30, 10000);
  comms.init("Reminder", broadcastAddress);
  // Sets callback on data received.
  esp_now_register_recv_cb(OnDataRecv);
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