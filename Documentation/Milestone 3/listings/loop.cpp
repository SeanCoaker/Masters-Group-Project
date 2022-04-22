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

		if (!isVibrate) {
			audio.play();
		} else {
			comms.sendVibrateMessage();
		}
	}

	accel.checkForStep();
}