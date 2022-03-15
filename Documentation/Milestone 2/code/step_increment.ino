void addStepToCounter() {
	steps++;
	currentMillis = millis();
	unsigned long timerVal = currentMillis - startMillis;

	if (!stepTimerStarted) {
				
		stepTimerStarted = true;
		startMillis = millis();

	} else {

		if (timerVal <= period) {
			if (steps >= 5) {

				// Send reminder to patient
				flashLED();
				Serial.print("REMINDER FIRED:\t");
				Serial.println(timerVal);

				steps = 0;
				stepTimerStarted = false;

			}
		} else {

			Serial.print("Timer Reset:\t");
			Serial.println(timerVal);
			steps = 0;
			stepTimerStarted = false;

		}

	}
			
  Serial.print("Steps: ");
  Serial.println(steps);
}