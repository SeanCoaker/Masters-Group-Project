#include <SdFat.h>
#include <esp_now.h>
#include <WiFi.h>
#include <TinyPICO.h>
#include <SparkFun_ADXL345.h>

#include "SPIFFS.h"
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

TinyPICO tp = TinyPICO();

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

SdFat sd;
String message;

ADXL345 adxl = ADXL345();

bool reminder = false;

bool stepDetected = false;
long lastStepTime = millis();
const unsigned long period = 5000;


void IRAM_ATTR ISR() {
	Serial.println("Reached");
	byte interrupts = adxl.getInterruptSource();
	if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {

        if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {

        }
	}
}


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
  memcpy(&message, data, sizeof(message));
  Serial.println(message);
  
  flash();
  reminder = true;
}


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
	Serial.begin(115200);
  	tp.DotStar_SetPixelColor(0,128,0);

  	initAudio();
  	initComms();
  	initADXL();
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


void initComms() {
	// Set device as a Wi-Fi Station
  	WiFi.mode(WIFI_STA);
  	Serial.println(WiFi.macAddress());

  	// Init ESP-NOW
  	if (esp_now_init() != ESP_OK) {
    	Serial.println("Error initializing ESP-NOW");
    	return;
  	}
  
  	// Once ESPNow is successfully Init, we will register for recv CB to
  	// get recv packer info
  	esp_now_register_recv_cb(OnDataRecv);
}


void initADXL() {
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

  // pinMode(4, INPUT_PULLUP);
  // adxl.setInterruptLevelBit(false);
  // attachInterrupt(digitalPinToInterrupt(4), ISR, HIGH);
}


void playAudio() {
	file = new AudioFileSourceSPIFFS("/reminder.mp3");
  	id3 = new AudioFileSourceID3(file);
  	id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  	out = new AudioOutputI2S();
  	out->SetPinout(27,26,25);
  	mp3 = new AudioGeneratorMP3();

	Serial.println("Play Audio");
	mp3->begin(id3, out);
	while (mp3->loop());
	mp3->stop();
}


void flash() {
    tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
    delay(500);
    tp.DotStar_SetBrightness(0);
    tp.DotStar_Show();
}


bool checkForStep() {
	bool result = false;
	byte interrupts = adxl.getInterruptSource();
	if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
        delay(250);

        if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
        	stepDetected = true;
        	lastStepTime = millis();
        	Serial.println("Tap Detected");
        	result = true;
        }

        delay(250);
	}

	return result;
}


void loop() {
	bool isPlayAudio = false;
	bool checkFuture = false;

	if (reminder) {
		long currentMillis = millis();

		if (stepDetected) {
			if (currentMillis - lastStepTime >= period) {
				checkFuture = true;
			}
		} else {
			checkFuture = true;
		}

		if (checkFuture) {
			Serial.printf("Checking for future");
			long eventStart = millis();
			bool futureStepDetected = false;
			while (millis() - eventStart <= period && !futureStepDetected) {
				futureStepDetected = checkForStep();
			}

			if (!futureStepDetected) {
				isPlayAudio = true;
			}
		}

		reminder = false;
	}

	if (isPlayAudio) {
		playAudio();
	}

	checkForStep();
}