#include "WearableAccelerometer.h"

WearableAccelerometer::WearableAccelerometer() {}

void WearableAccelerometer::init(double singleTapThresholdValue, double singleTapThresholdDuration
		, double doubleTapLatency, double doubleTapWindow, unsigned long period) {

	this->singleTapThresholdValue = singleTapThresholdValue;
	this->singleTapThresholdDuration = singleTapThresholdDuration;
	this->doubleTapLatency = doubleTapLatency;
	this->doubleTapWindow = doubleTapWindow;
	this->period = period;
	this->adxl = ADXL345();
	this->stepTimerStarted = false;
	this->steps = 0;

	adxl.powerOn();
  	adxl.setRangeSetting(4);

  	// Set activity detection to false to allow focus only on tap detection.
  	adxl.setActivityX(0);
  	adxl.setActivityY(0);
  	adxl.setActivityZ(0);

  	// Set inactivity detection to false to allow focus only on tap detection.
  	adxl.setInactivityX(0);
  	adxl.setInactivityY(0);
  	adxl.setInactivityZ(0);

  	// Set tap detection to true only for Z axis (gravity).
  	adxl.setTapDetectionOnX(0);
  	adxl.setTapDetectionOnY(0);
  	adxl.setTapDetectionOnZ(1);

  	// Set threshold for tap detection.
  	adxl.setTapThreshold(this->singleTapThresholdValue); //62.5mg per increment
  	// Detects how long accel value should be above tap threshold for it to count as a tap.
  	adxl.setTapDuration(this->singleTapThresholdDuration); //625us per increment

  	// Sets how long after single tap is detected until a double tap can be detected.
  	adxl.setDoubleTapLatency(this->doubleTapLatency);
  	// Sets how long is given for a double tap to be detected.
  	adxl.setDoubleTapWindow(this->doubleTapWindow);

  	// Sets which interrupts the ADXL can call. In this case, only single tap and double tap.
  	adxl.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 1);
  	adxl.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 1);
  	adxl.setInterrupt(ADXL345_INT_FREE_FALL_BIT,  0);
  	adxl.setInterrupt(ADXL345_INT_ACTIVITY_BIT,   0);
  	adxl.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 0);

}

bool WearableAccelerometer::poll() {

	bool result = false;

	byte interrupts = adxl.getInterruptSource();
	if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
        delay(250);

        if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
        	result = addStepToCounter();
        }

        delay(250);
	}

	return result;

}

bool WearableAccelerometer::addStepToCounter() {

	steps++;
	currentMillis = millis();
	unsigned long timerVal = currentMillis - startMillis;
	bool result = false;

	if (!stepTimerStarted) {
				
		stepTimerStarted = true;
		startMillis = millis();

	} else {

		if (timerVal <= period) {
			if (steps >= 5) {

				// Send reminder to patient
				result = true;
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

  	return result;

}

