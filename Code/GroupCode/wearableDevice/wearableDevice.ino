#include "WearableAccelerometer.h"
#include "WearableCommunications.h"

uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xA1, 0xA0, 0x84};
WearableAccelerometer accel;
WearableCommunications comms;


void setup() {
	Serial.begin(115200);
  accel.init(20, 15, 10, 30, 10000);
  comms.init("Reminder", broadcastAddress);
}


void loop() {

  if (accel.poll()) {
    comms.sendReminderMessage();
  }

}