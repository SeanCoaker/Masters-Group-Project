/**
 * @file WalkAidAccelerometer.h
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief A class to handle duties of the accelerometer within this system.
 * @version 1.0
 * @date 10-04-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <SparkFun_ADXL345.h>

class WalkAidAccelerometer {

private:

	//! Threshold for single tap to be detected.
	double singleTapThresholdValue;
	//! Duration for single tap to be above threshold to be detected.
	double singleTapThresholdDuration;
	//! Time between a tap event and when the reading for a double tap starts.
	double doubleTapLatency;
	//! Defines how long a double tap is checked for.
	double doubleTapWindow;

	//! Defines if a step has been detected.
	bool stepDetected;
	//! The last time a step was detected.
	long lastStepTime;
	//! The period to check previously and in future for detected steps when a message is received.
	unsigned long period;

	//! Accelerometer object.
	ADXL345 adxl;

public:

	/**
	 * @brief Construct a new Walk Aid Accelerometer object.
	 * 
	 */
	WalkAidAccelerometer();

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
	 * @brief Checks if a tap event was detected by the accelerometer.
	 * 
	 * @return true if tap is detected, false otherwise.
	 */
	bool checkForStep();

	/**
	 * @brief Returns the value of stepDetected.
	 * 
	 * @return bool stepDetected
	 */
	bool isStepDetected();

	/**
	 * @brief Get the time the last step was taken.
	 * 
	 * @return long lastStepTime.
	 */
	long getLastStepTime();

	/**
	 * @brief Get the developer's specified period to check for steps.
	 * @return unsigned long period.
	 */
	unsigned long getPeriod();

};
