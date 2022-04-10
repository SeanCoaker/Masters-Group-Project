/**
 * @file SDToSPIFFS.h
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief A file to handle the transferring audio files from the 
 * microSD card to the SPIFFS storage space.
 * @version 1.0
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <SdFat.h>
#include "SPIFFS.h"

// Clock Select Pin to the SD Card Module
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


class SDToSPIFFS {

private:

	//! Audio file to be transferred.
	const char *SDPath;
	//! Name to give the audio file being saved in SPIFFS.
	const char *SPIFFSPath;

	//! SD Card object.
	SdFat sd;

public:

	/**
	 * @brief Construct a new SDToSPIFFS object.
	 * 
	 */
	SDToSPIFFS();
	
	/**
	 * @brief Initialises the object ready for the 
	 * transferring of files from SD card to SPIFFs.
	 * 
	 * @param SDPath The path to the SD card file.
	 * @param SPIFFSPath The file name to give the new SPIFFS file.
	 */
	void init(const char *SDPath, const char *SPIFFSPath);

	/**
	 * @brief Handles the loading of the SD card file to SPIFFS.
	 * 
	 */
	void loadSDFileToSPIFFS(int test);

	/**
	 * @brief Returns the path to the SPIFFS file.
	 * 
	 * @return const char* SPIFFS file path.
	 */
	const char *getSPIFFSPath();

};