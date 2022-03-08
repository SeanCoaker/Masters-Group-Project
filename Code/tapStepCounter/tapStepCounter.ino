#include <Wire.h>
#include <TinyPICO.h>
#include <SparkFun_ADXL345.h>

TinyPICO tp = TinyPICO();
ADXL345 adxl = ADXL345(5);

int steps = 0;

void setup() {
	Serial.begin(115200);
	adxl.powerOn();
    adxl.setRangeSetting(4);

	//look of activity movement on this axes - 1 == on; 0 == off
    adxl.setActivityX(0);
    adxl.setActivityY(0);
    adxl.setActivityZ(0);

    //look of inactivity movement on this axes - 1 == on; 0 == off
    adxl.setInactivityX(0);
    adxl.setInactivityY(0);
    adxl.setInactivityZ(0);

    //look of tap movement on this axes - 1 == on; 0 == off
    adxl.setTapDetectionOnX(0);
    adxl.setTapDetectionOnY(0);
    adxl.setTapDetectionOnZ(1);

    //set values for what is a tap, and what is a double tap (0-255)
    adxl.setTapThreshold(20); //62.5mg per increment
    adxl.setTapDuration(15); //625us per increment

    adxl.setDoubleTapLatency(10);
    adxl.setDoubleTapWindow(30);

    adxl.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_FREE_FALL_BIT,  0);
    adxl.setInterrupt(ADXL345_INT_ACTIVITY_BIT,   0);
    adxl.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 0);
}

void loop() {
	byte interrupts = adxl.getInterruptSource();
	if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
        delay(250);

        if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
            steps++;
            Serial.print("Steps: ");
            Serial.println(steps);
        }

        delay(250);
	}
}