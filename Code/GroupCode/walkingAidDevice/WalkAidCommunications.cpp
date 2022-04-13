#include "WalkAidCommunications.h"

WalkAidCommunications::WalkAidCommunications() {}

void WalkAidCommunications::init(String message, uint8_t broadcastAddress[]) {

	this->reminder = false;
	this->message = message;
	this->broadcastAddress = broadcastAddress;

	// Set device as a Wi-Fi Station
  	WiFi.mode(WIFI_STA);
  	Serial.println(WiFi.macAddress());

  	// Init ESP-NOW
  	if (esp_now_init() != ESP_OK) {
		this->debugLED.initError();
    	Serial.println("Error initializing ESP-NOW");
    	return;
  	}

	// Once ESPNow is successfully Init, we will register for Send CB to
  	// get the status of Trasnmitted packet
  	esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status) {
  		Serial.print("\r\nLast Packet Send Status:\t");
  		Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  	});
  
  	// Register peer
  	esp_now_peer_info_t peerInfo;
  	memset(&peerInfo, 0, sizeof(peerInfo));
  	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  	peerInfo.channel = 0;  
  	peerInfo.encrypt = false;
  
  	// Add peer        
  	if (esp_now_add_peer(&peerInfo) != ESP_OK){
    	Serial.println("Failed to add peer");
		this->debugLED.peerSetupError();
    	return;
  	}

	this->debugLED.poweredOn();

}

bool WalkAidCommunications::getReminder() {

	return this->reminder;

}
	
void WalkAidCommunications::setReminder(bool reminder) {

	this->reminder = reminder;

}

void WalkAidCommunications::sendVibrateMessage() {

	// Send message via ESP-NOW
  	esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
   
  	if (result == ESP_OK) {
    	Serial.println("Vibrate message sent with success");
  	} else {
    	Serial.println("Error sending the vibrate message");
  	}

}