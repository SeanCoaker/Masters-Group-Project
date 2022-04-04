#include "SDToSPIFFS.h"

SDToSPIFFS::SDToSPIFFS() {}

void SDToSPIFFS::init(const char *SDPath, const char *SPIFFSPath) {

	this->SDPath = SDPath;
	this->SPIFFSPath = SPIFFSPath;

	SPIFFS.begin(5);
  	delay(1000);
  	while (!sd.begin(SD_CONFIG)){
    	sd.initErrorHalt(&Serial);
    	delay(1000);
  	}
  	Serial.println("SD Card Initialised");

}

void SDToSPIFFS::loadSDFileToSPIFFS() {

	FsFile sourceFile = sd.open(this->SDPath);
  	File destFile = SPIFFS.open(this->SPIFFSPath, FILE_WRITE);
  	static uint8_t buf[512];
  	while( sourceFile.read( buf, 512) ) {
    	destFile.write( buf, 512 );
  	}
  	destFile.close();
  	sourceFile.close();
  	sd.end();

}

const char *SDToSPIFFS::getSPIFFSPath() {

	return this->SPIFFSPath;

}