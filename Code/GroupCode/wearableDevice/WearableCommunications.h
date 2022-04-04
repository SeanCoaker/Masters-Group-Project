#include <WiFi.h>
#include <esp_now.h>

class WearableCommunications {

private:

	String message;
	uint8_t *broadcastAddress;

public:
	
	WearableCommunications();

	void init(String message, uint8_t broadcastAddress[]);
	void sendReminderMessage();

};