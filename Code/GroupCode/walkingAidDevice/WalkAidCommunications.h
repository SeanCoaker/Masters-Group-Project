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
#include "DebugLED.h"

class WalkAidCommunications {

private:

	//! Whether a reminder message has been received.
	bool reminder;
	//! The message to send to the peer.
	String message;
	//! The MAC address of the peer.
	uint8_t *broadcastAddress;

	DebugLED debugLED;

public:
	
	/**
	 * @brief Construct a new Walk Aid Communications object.
	 * 
	 */
	WalkAidCommunications();

	/**
	 * @brief Initialise communication to the wearable device.
	 * 
	 * @param message The message to send to the wearable device.
	 * @param broadcastAddress The MAC address of the wearable device.
	 */
	void init(String message, uint8_t broadcastAddress[]);

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

	/**
	 * @brief Sends a message to the wearable device to vibrate.
	 * 
	 */
	void sendVibrateMessage();

};