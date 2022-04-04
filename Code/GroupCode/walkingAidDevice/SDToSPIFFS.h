#include <SdFat.h>
#include "SPIFFS.h"

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

	const char *SDPath;
	const char *SPIFFSPath;

	SdFat sd;

public:

	SDToSPIFFS();
	void init(const char *SDPath, const char *SPIFFSPath);
	void loadSDFileToSPIFFS();
	const char *getSPIFFSPath();

};