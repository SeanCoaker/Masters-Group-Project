void setup() {

  	// Initialise serial monitor.
	Serial.begin(115200);

	// Set the colour of the LED to green.
	tp.DotStar_SetPixelColor(0,128,0);
	tp.DotStar_SetBrightness(0);
  	tp.DotStar_Show();

  	adxl.powerOn();
  	// Sets accuracy of the ADXL readings.
  	adxl.setRangeSetting(4);

  	// Reads the average acceleration of the ADXL at boot.
  	readAverageAccel(xCalib, yCalib, zCalib);
}