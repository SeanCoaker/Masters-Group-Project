#include "WalkAidAccelerometer.h"

WalkAidAccelerometer::WalkAidAccelerometer() {}

void WalkAidAccelerometer::init(double singleTapThresholdValue, double singleTapThresholdDuration
		, double doubleTapLatency, double doubleTapWindow, unsigned long period) {

	this->singleTapThresholdValue = singleTapThresholdValue;
	this->singleTapThresholdDuration = singleTapThresholdDuration;
	this->doubleTapLatency = doubleTapLatency;
	this->doubleTapWindow = doubleTapWindow;
	this->period = period;
	this->adxl = ADXL345();
	this->stepDetected = false;
	this->lastStepTime = millis();

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

bool WalkAidAccelerometer::checkForStep() {

	bool result = false;
	byte interrupts = adxl.getInterruptSource();
	if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
        delay(250);

        if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
        	this->stepDetected = true;
        	this->lastStepTime = millis();
        	Serial.println("Tap Detected");
        	result = true;
        }

        delay(250);
	}

	return result;

}

bool WalkAidAccelerometer::isStepDetected() {

	return this->stepDetected;

}

long WalkAidAccelerometer::getLastStepTime() {

	return this->lastStepTime;

}

unsigned long WalkAidAccelerometer::getPeriod() {

	return this->period;

}