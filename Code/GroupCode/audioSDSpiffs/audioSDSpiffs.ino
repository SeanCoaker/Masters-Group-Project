#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
// #include <FS.h>
#include <SdFat.h>

SdFat sd;

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
  WiFi.mode(WIFI_OFF); 
  Serial.begin(115200);
  delay(1000);
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
  beginAudio();
}

void beginAudio() {
  Serial.println("Careless Whisper plays...");
  file = new AudioFileSourceSPIFFS("/reminder.mp3");
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  Serial.println(id3->getSize());
  out = new AudioOutputI2S();
  out->SetPinout(27,26,25);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
}

void loop() {
  if (mp3->isRunning()) {
  	if (!mp3->loop()) mp3->stop();
  } else {
  	Serial.printf("MP3 done\n");
    delay(1000);
  }
}