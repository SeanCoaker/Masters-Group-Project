void loop() {

	readAverageAccel(xCurrent, yCurrent, zCurrent);

	float acc = convertToAccel(xCurrent, yCurrent, zCurrent);
	delay(250);

	//Next Acceleration
	readAverageAccel(xNext, yNext, zNext);

	float acc2 = convertToAccel(xNext, yNext, zNext);

	if (acc2 - acc > stepThreshold) {
		addStepToCounter();
	}

	delay(400);
}

// Gets the acceleration for 3 axes
float convertToAccel(float x, float y, float z) {

	return sqrt((square(x - xCalib)) + 
	               (square(y - yCalib)) + 
	               (square(z - zCalib)));

}

// Returns the square value of val
float square(float val) {
	return val * val;
}

