/**
 * @file WearableAccelerometer.h
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief A class to handle all the walking detection in the wearable device.
 * @version 1.0
 * @date 11-04-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <SparkFun_ADXL345.h>

class WearableAccelerometer {

private:

	//! Threshold for single tap to be detected.
	double singleTapThresholdValue;
	//! Duration for single tap to be above threshold to be detected.
	double singleTapThresholdDuration;
	//! Time between a tap event and when the reading for a double tap starts.
	double doubleTapLatency;
	//! Defines how long a double tap is checked for.
	double doubleTapWindow;

	//! Accelerometer object.
	ADXL345 adxl;

	//! Keeps track of when the first step was taken.
	unsigned long startMillis;
	//! Stores the time of the current step being taken.
	unsigned long currentMillis;
	//! The period of time that we should check for 5 steps.
	unsigned long period;
	//! Stores if the timer has been started or not.
	bool stepTimerStarted;
	//! Acts as the step counter.
	int steps;

public:

	/**
	 * @brief Construct a new Wearable Accelerometer object.
	 * 
	 */
	WearableAccelerometer();

	/**
	 * @brief Initialises the accelerometer with the specified values in parameter fields.
	 * 
	 * @param singleTapThresholdValue Threshold for single tap to be detected.
	 * @param singleTapThresholdDuration Duration that tap must be above threshold to be detected.
	 * @param doubleTapLatency Time after single tap until double tap is measured.
	 * @param doubleTapWindow How long double tap is checked for.
	 * @param period Time period to check for steps before and after receiving message.
	 */
	void init(double singleTapThresholdValue, double singleTapThresholdDuration
		, double doubleTapLatency, double doubleTapWindow, unsigned long period);

	/**
	 * @brief Polls the accelerometer to see if a single-tap has been detected. Calls the addStepToCounter function if it has.
	 * 
	 * @return The result of addStepToCounter.
	 */
	bool poll();

	/**
	 * @brief Increments the step counter. If the it has been longer than the set period of time since the first step, then reset the time of the first step and reset the step counter. If not, and the step counter = 5 reset the step counter and time of first step, and return true.
	 * 
	 * @return true if 5 steps were detected within period of time, false otherwise.
	 */
	bool addStepToCounter();
	
};