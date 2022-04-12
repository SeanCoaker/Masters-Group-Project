/**
 * @file WearableCommunications.h
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief This class handles the instantiation of ESP-Now and 
 * sends messages to the peer when needed.
 * @version 1.0
 * @date 11-04-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <WiFi.h>
#include <esp_now.h>
#include "DebugLED.h"

class WearableCommunications {

private:

	//! The message to send to the peer.
	String message;
	//! The MAC address of the peer.
	uint8_t *broadcastAddress;
	//! An instance of DebugLED to display LED colours depending on state of system.
	DebugLED debugLED;

public:
	
	/**
	 * @brief Construct a new Wearable Communications object.
	 * 
	 */
	WearableCommunications();

	/**
	 * @brief Initialise ESP-Now and peer device info, and set on send message callback function.
	 * 
	 * @param message The message to send to the walking aid device.
	 * @param broadcastAddress The MAC address of the walking aid device.
	 */
	void init(String message, uint8_t broadcastAddress[]);

	/**
	 * @brief Send the message to the walking aid device.
	 * 
	 */
	void sendReminderMessage();

};