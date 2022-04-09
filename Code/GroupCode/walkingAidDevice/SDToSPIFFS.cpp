/**
 * @file SDToSPIFFS.cpp
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief A file to handle the transferring audio files from the 
 * microSD card to the SPIFFS storage space.
 * @version 1.0
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "SDToSPIFFS.h"

/**
 * @brief Construct a new SDToSPIFFS object.
 * 
 */
SDToSPIFFS::SDToSPIFFS() {}

/**
 * @brief Initialises the object ready for transferring a file from SD to SPIFFS.
 * Begins by setting the SD and SPIFFS path variables for use later. Then initialises
 * connection to SPIFFS and the SD Card.
 * 
 * @param SDPath 
 * @param SPIFFSPath 
 */
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

/**
 * @brief Handles the loading of a file from SD card to SPIFFS.
 * The SD file is opened and the SPIFFS file is created and opened.
 * Then the SD card file is copied 512 bytes at a time to the SPIFFS file.
 * 
 */
void SDToSPIFFS::loadSDFileToSPIFFS() {

	FsFile sourceFile = sd.open(this->SDPath);
  	File destFile = SPIFFS.open(this->SPIFFSPath, FILE_WRITE);
  	static uint8_t buf[512];
  	while(sourceFile.read(buf, 512)) {
    	destFile.write(buf, 512);
  	}
  	destFile.close();
  	sourceFile.close();
  	sd.end();

}

/**
 * @brief Returns the SPIFFS path set in the init function.
 * 
 * @return const char* SPIFFS path.
 */
const char *SDToSPIFFS::getSPIFFSPath() {

	return this->SPIFFSPath;

}