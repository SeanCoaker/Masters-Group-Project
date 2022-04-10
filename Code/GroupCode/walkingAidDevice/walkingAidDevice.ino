#include "WalkAidAccelerometer.h"
#include "WalkAidCommunications.h"
#include "WalkAidAudio.h"

//! A copy of the message received from the wearable device.
String message;
//! The audio controller object.
WalkAidAudio audio;
//! The communications controller object.
WalkAidCommunications comms;
//! The accelerometer controller object.
WalkAidAccelerometer accel;


/**
 * @brief Callback function to be executed when data is received from a reminder message.
 * 
 * @param mac MAC address of the device that sent the reminder.
 * @param data Data of the reminder.
 * @param len Length of the reminder.
 */
void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
  memcpy(&message, data, sizeof(message));
  Serial.println(message);
  comms.setReminder(true);
}

/**
 * @brief Handles the Arduino setup function. Initialises the accelerometer, communications, and audio.
 * 
 */
void setup() {
	  Serial.begin(115200);

    audio.init("/reminder/reminder.mp3", "/reminder.mp3");
  	comms.init();
    // Sets callback on data received.
    esp_now_register_recv_cb(OnDataRecv);
  	accel.init(20, 15, 10, 30, 10000);
}

/**
 * @brief Handles the Arduino loop function. Loops through checks for reminder messages and responds accordingly.
 * 
 */
void loop() {
	bool isPlayAudio = false;
	bool checkFuture = false;

	if (comms.getReminder()) {
		long currentMillis = millis();

		if (accel.isStepDetected()) {
			if (currentMillis - accel.getLastStepTime() >= accel.getPeriod()) {
				checkFuture = true;
			}
		} else {
			checkFuture = true;
		}

		if (checkFuture) {
			Serial.println("Checking for future");
			long eventStart = millis();
			bool futureStepDetected = false;
			while (millis() - eventStart <= accel.getPeriod() && !futureStepDetected) {
				futureStepDetected = accel.checkForStep();
			}

			if (!futureStepDetected) {
				isPlayAudio = true;
			}
		}

		comms.setReminder(false);
	}

	if (isPlayAudio) {
		audio.play();
	}

	accel.checkForStep();
}