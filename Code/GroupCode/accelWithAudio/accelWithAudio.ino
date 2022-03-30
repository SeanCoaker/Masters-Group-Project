#include <Wire.h>
#include <TinyPICO.h>
#include <SparkFun_ADXL345.h>
#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include <SdFat.h>

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = SS;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif  // SDCARD_SS_PIN

#define SPI_CLOCK SD_SCK_MHZ(5)

#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

TinyPICO tp = TinyPICO();
ADXL345 adxl = ADXL345();

SdFat sd;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000; // 10 second period
bool stepTimerStarted = false;
int steps = 0;

bool audioPlayedFirstTime = false;


// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}


void setup() {

	// Initialise serial monitor
	Serial.begin(115200);
	Serial.println("Init");
	initAudio();
	Serial.println("Ready");

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

void initAudio() {
	SPIFFS.begin(5);
  delay(1000);
  while (!sd.begin(SD_CONFIG)){
    sd.initErrorHalt(&Serial);
    delay(1000);
  }
  Serial.println("SD Card Initialised");
  FsFile sourceFile = sd.open("/reminder/reminder.mp3");
  File destFile = SPIFFS.open("/reminder.mp3", FILE_WRITE);
  static uint8_t buf[512];
  while( sourceFile.read( buf, 512) ) {
    destFile.write( buf, 512 );
  }
  destFile.close();
  sourceFile.close();
  sd.end();
  audioLogger = &Serial;
}


void playAudio() {
	Serial.println("Play Audio");
  file = new AudioFileSourceSPIFFS("/reminder.mp3");
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  out = new AudioOutputI2S();
  out->SetPinout(27,26,25);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
  while (mp3->loop());
  mp3->stop();
}


void loop() {
	byte interrupts = adxl.getInterruptSource();
	if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
        delay(250);

        if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
        	addStepToCounter();
        }

        delay(250);
	}
}


/*
  A function that causes the LED of the TinyPICO to blink.
*/
void flash() {
    tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
    delay(500);
    tp.DotStar_SetBrightness(0);
    tp.DotStar_Show();
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
				// flash();
				playAudio();
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