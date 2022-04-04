#include "WalkAidAccelerometer.h"
#include "WalkAidCommunications.h"
#include "WalkAidAudio.h"

String message;
WalkAidAudio audio;
WalkAidCommunications comms;
WalkAidAccelerometer accel;


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
  memcpy(&message, data, sizeof(message));
  Serial.println(message);
  comms.setReminder(true);
}


void setup() {
	  Serial.begin(115200);

    audio.init("/reminder/reminder.mp3", "/reminder.mp3");
  	comms.init();
    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(OnDataRecv);
  	accel.init(20, 15, 10, 30, 10000);
}


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