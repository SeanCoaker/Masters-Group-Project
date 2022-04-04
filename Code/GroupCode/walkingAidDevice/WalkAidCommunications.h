#include <WiFi.h>
#include <esp_now.h>

class WalkAidCommunications {

private:

	bool reminder;

public:
	
	WalkAidCommunications();

	void init();
	bool getReminder();
	void setReminder(bool reminder);

};