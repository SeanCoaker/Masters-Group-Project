#include <Wire.h>
#include <TinyPICO.h>

TinyPICO tp = TinyPICO();

float xavg=0, yavg=0, zavg=0;
float xcur=0, ycur=0, zcur=0;
float xnxt=0, ynxt=0, znxt=0;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000; // 10 second period
bool stepTimerStarted = false;
int steps = 0;


void flashLED() {
	tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
   	tp.DotStar_SetBrightness(0);
   	tp.DotStar_Show();
}

void setup() {
  	tp.DotStar_SetBrightness(0);
  	tp.DotStar_Show();
  	tp.DotStar_SetPixelColor(0,128,0);

  	adxl345_init();
  	read_av_acc();
}

void loop() {

	for (int i = 0; i < 50; i++) {
		Wire.beginTransmission(0x53);
		Wire.write(0x32); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte x0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x33); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte x1 = Wire.read();
		x1 = x1 & 0x03;

		uint16_t x = (x1 << 8) + x0;
		int16_t xf = x;

		if (xf > 511) {
			xf = xf - 1024;
		}

		float xa = xf * 0.004;
		xcur = xcur + xa;


		Wire.beginTransmission(0x53);
		Wire.write(0x34); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte y0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x35); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte y1 = Wire.read();
		y1 = y1 & 0x03;

		uint16_t y = (y1 << 8) + y0;
		int16_t yf = y;

		if (yf > 511) {
			yf = yf - 1024;
		}

		float ya = yf * 0.008;
		ycur = ycur + ya;


		Wire.beginTransmission(0x53);
		Wire.write(0x36); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte z0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x37); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte z1 = Wire.read();
		z1 = z1 & 0x03;

		uint16_t z = (z1 << 8) + z0;
		int16_t zf = z;
		if (zf > 511) {
			zf = zf - 1024;
		}

		float za = zf * 0.004;
		zcur = zcur + za;
	}

	xcur = xcur/50; 
	ycur = ycur/50;
	zcur = zcur/50;

	float acc = sqrt(((xcur - xavg) * (xcur - xavg)) + 
	               ((ycur - yavg) * (ycur - yavg)) + 
	               ((zcur - zavg) * (zcur - zavg)));
	delay(250);

	//Next Acceleration
	for (int i = 0; i < 50; i++) {
		Wire.beginTransmission(0x53);
		Wire.write(0x32); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte x0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x33); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte x1 = Wire.read();
		x1 = x1 & 0x03;

		uint16_t x = (x1 << 8) + x0;
		int16_t xf = x;

		if (xf > 511) {
			xf = xf - 1024;
		}

		float xa = xf * 0.004;
		xnxt = xnxt + xa;


		Wire.beginTransmission(0x53);
		Wire.write(0x34); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte y0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x35); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte y1 = Wire.read();
		y1 = y1 & 0x03;

		uint16_t y = (y1 << 8) + y0;
		int16_t yf = y;

		if (yf > 511) {
			yf = yf - 1024;
		}

		float ya = yf * 0.004;
		ynxt = ynxt + ya;


		Wire.beginTransmission(0x53);
		Wire.write(0x36); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte z0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x37); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte z1 = Wire.read();
		z1 = z1 & 0x03;

		uint16_t z = (z1 << 8) + z0;
		int16_t zf = z;

		if (zf > 511) {
			zf = zf - 1024;
		}

		float za = zf * 0.004;
		znxt = znxt + za;
	}
		
	xnxt = xnxt/50; 
	ynxt = ynxt/50;
	znxt = znxt/50;

	float acc2 = sqrt(((xnxt - xavg) * (xnxt - xavg)) + 
	               ((ynxt - yavg) * (ynxt - yavg)) + 
	               ((znxt - zavg) * (znxt - zavg)));

	if (acc2 - acc > 0.05) {
		addStepToCounter();
	}

	delay(400);
}

void adxl345_init(){
	Serial.begin(115200);
	Wire.begin();

	Wire.beginTransmission(0x53);
	Wire.write(0x2C); 
	Wire.write(0x08); 
	Wire.endTransmission();

	Wire.beginTransmission(0x53);
	Wire.write(0x31); 
	Wire.write(0x08); 
	Wire.endTransmission();

	Wire.beginTransmission(0x53);
	Wire.write(0x2D); 
	Wire.write(0x08); 
	Wire.endTransmission();
}

void read_av_acc(){

	for (int i = 0; i < 50; i++) {
		Wire.beginTransmission(0x53);
		Wire.write(0x32); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte x0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x33); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte x1 = Wire.read();
		x1 = x1 & 0x03;

		uint16_t x = (x1 << 8) + x0;
		int16_t xf = x;

		if (xf > 511) {
			xf = xf - 1024;
		}

		float xa = xf * 0.004;
		xavg = xavg + xa;


		Wire.beginTransmission(0x53);
		Wire.write(0x34); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte y0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x35); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte y1 = Wire.read();
		y1 = y1 & 0x03;

		uint16_t y = (y1 << 8) + y0;
		int16_t yf = y;

		if (yf > 511) {
			yf = yf - 1024;
		}

		float ya = yf * 0.004;
		yavg = yavg + ya;


		Wire.beginTransmission(0x53);
		Wire.write(0x36); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte z0 = Wire.read();

		Wire.beginTransmission(0x53);
		Wire.write(0x37); 
		Wire.endTransmission();
		Wire.requestFrom(0x53, 1);
		byte z1 = Wire.read();
		z1 = z1 & 0x03;

		uint16_t z = (z1 << 8) + z0;
		int16_t zf = z;

		if (zf > 511) {
			zf = zf - 1024;
		}

		float za = zf * 0.004;
		zavg = zavg + za;
	}

	xavg = xavg/50; 
	yavg = yavg/50;
	zavg = zavg/50;
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
