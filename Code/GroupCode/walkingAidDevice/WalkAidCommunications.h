/**
 * @file WalkAidCommunications.h
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief A class to handle communication between the walk aid device and the wearable device.
 * @version 1.0
 * @date 10-04-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <WiFi.h>
#include <esp_now.h>

class WalkAidCommunications {

private:

	//! Whether a reminder message has been received.
	bool reminder;

public:
	
	/**
	 * @brief Construct a new Walk Aid Communications object.
	 * 
	 */
	WalkAidCommunications();

	/**
	 * @brief Initialise communication to the wearable device.
	 * 
	 */
	void init();

	/**
	 * @brief Get whether a reminder has been received or not.
	 * 
	 * @return bool reminder.
	 */
	bool getReminder();

	/**
	 * @brief Set whether a reminder has been received or not.
	 * 
	 * @param reminder true if reminder received, false otherwise.
	 */
	void setReminder(bool reminder);

};