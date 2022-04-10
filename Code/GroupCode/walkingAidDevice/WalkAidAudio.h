/**
 * @file WalkAidAudio.h
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief A class to handle the playing of the SPIFFS audio file.
 * @version 1.0
 * @date 10-04-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "SDToSPIFFS.h"

class WalkAidAudio {

private:

	//! The generator of the mp3 player.
	AudioGeneratorMP3 *mp3;
	//! The source file of the SPIFFS audio file.
	AudioFileSourceSPIFFS *file;
	//! The output of the mp3 player.
	AudioOutputI2S *out;
	//! Metadata handler for the mp3 file.
	AudioFileSourceID3 *id3;

	//! Object to handle the SD to SPIFFS transfer.
	SDToSPIFFS s2s;

public:

	/**
	 * @brief Construct a new Walk Aid Audio object.
	 * 
	 */
	WalkAidAudio();

	/**
	 * @brief Initialises the walk aid audio system by calling the transfer of SD file to SPIFFS and generating the mp3 code. 
	 * 
	 * @param SDPath Path to the SD card audio file.
	 * @param SPIFFSPath Name of the SPIFFS audio file to be created.
	 */
	void init(const char *SDPath, const char *SPIFFSPath);

	/**
	 * @brief Plays the SPIFFS audio file.
	 * 
	 */
	void play();

};