void setup() {

	// Initialise serial monitor
	Serial.begin(115200);

	// Set the colour of the LED to green
	tp.DotStar_SetPixelColor(0,128,0);
	tp.DotStar_SetBrightness(0);
    tp.DotStar_Show();

	adxl.powerOn();
    adxl.setRangeSetting(4);

	// Set activity detection to false to allow focus only on tap detection.
    adxl.setActivityX(0);
    adxl.setActivityY(0);
    adxl.setActivityZ(0);

    // Set inactivity detection to false to allow focus only on tap detection.
    adxl.setInactivityX(0);
    adxl.setInactivityY(0);
    adxl.setInactivityZ(0);

    // Set tap detection to true only for Z axis (gravity).
    adxl.setTapDetectionOnX(0);
    adxl.setTapDetectionOnY(0);
    adxl.setTapDetectionOnZ(1);

    // Set threshold for tap detection.
    adxl.setTapThreshold(20); //62.5mg per increment
    // Detects how long accel value should be above tap threshold for it to count as a tap.
    adxl.setTapDuration(15); //625us per increment

    // Sets how long after single tap is detected until a double tap can be detected.
    adxl.setDoubleTapLatency(10);
    // Sets how long is given for a double tap to be detected.
    adxl.setDoubleTapWindow(30);

    // Sets which interrupts the ADXL can call. In this case, only single tap and double tap.
    adxl.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_FREE_FALL_BIT,  0);
    adxl.setInterrupt(ADXL345_INT_ACTIVITY_BIT,   0);
    adxl.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 0);
}