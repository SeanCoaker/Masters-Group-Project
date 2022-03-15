void loop() {

    // Receives data on the interrupts fired.
	byte interrupts = adxl.getInterruptSource();

    // Only runs code if single tap is detected.
	if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
        delay(250);

        // Only increments step counter if double tap is not detected.
        if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
        	addStepToCounter();
        }

        delay(250);
	}
}