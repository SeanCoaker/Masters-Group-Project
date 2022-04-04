#include "WalkAidCommunications.h"

WalkAidCommunications::WalkAidCommunications() {}

void WalkAidCommunications::init() {

	this->reminder = false;

	// Set device as a Wi-Fi Station
  	WiFi.mode(WIFI_STA);
  	Serial.println(WiFi.macAddress());

  	// Init ESP-NOW
  	if (esp_now_init() != ESP_OK) {
    	Serial.println("Error initializing ESP-NOW");
    	return;
  	}

}

bool WalkAidCommunications::getReminder() {

	return this->reminder;

}
	
void WalkAidCommunications::setReminder(bool reminder) {

	this->reminder = reminder;

}

