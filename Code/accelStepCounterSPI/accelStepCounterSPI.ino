#include <Wire.h>
#include <TinyPICO.h>
#include <SparkFun_ADXL345.h>

TinyPICO tp = TinyPICO();
ADXL345 adxl = ADXL345(5);

float xCalib = 0, yCalib = 0, zCalib = 0;
float xCurrent = 0, yCurrent = 0, zCurrent = 0;
float xNext = 0, yNext = 0, zNext = 0;

double stepThreshold = 0.05;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000; // 10 second period
bool stepTimerStarted = false;
int steps = 0;


void setup() {

  	// Initialise serial monitor
	Serial.begin(115200);

	// Set the colour of the LED to green
	tp.DotStar_SetPixelColor(0,128,0);
	tp.DotStar_SetBrightness(0);
  	tp.DotStar_Show();

  	adxl.powerOn();
  	adxl.setRangeSetting(4);

  	readAverageAccel(xCalib, yCalib, zCalib);
}


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


void addStepToCounter() {
	steps++;
	currentMillis = millis();
	unsigned long timerVal = currentMillis - startMillis;

	if (!stepTimerStarted) {
				
		stepTimerStarted = true;
		startMillis = millis();

	} else {

		if (timerVal <= period) {
			if (steps >= 5) {

				// Send reminder to patient
				flashLED();
				Serial.print("REMINDER FIRED:\t");
				Serial.println(timerVal);

				steps = 0;
				stepTimerStarted = false;

			}
		} else {

			Serial.print("Timer Reset:\t");
			Serial.println(timerVal);
			steps = 0;
			stepTimerStarted = false;

		}

	}
			
  Serial.print("Steps: ");
  Serial.println(steps);
}


void readAverageAccel(float &xRef, float &yRef, float &zRef) {

	float xavg = 0, yavg = 0, zavg = 0;

	for (int i = 0; i < 50; i++) {
		int x, y, z;
		adxl.readAccel(&x, &y, &z);

		// Converts accel reading to unit of G
		float xa = x * 0.004;
		xavg += xa;

		// Converts accel reading to unit of G
		float ya = y * 0.004;
		yavg += ya;

		// Converts accel reading to unit of G
		float za = z * 0.004;
		zavg += za;
	}

	xRef = xavg / 50; 
	yRef = yavg / 50;
	zRef = zavg / 50;

}


float convertToAccel(float x, float y, float z) {

	return sqrt(((x - xCalib) * (x - xCalib)) + 
	               ((y - yCalib) * (y - yCalib)) + 
	               ((z - zCalib) * (z - zCalib)));

}


void flashLED() {
	tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
   	tp.DotStar_SetBrightness(0);
   	tp.DotStar_Show();
}