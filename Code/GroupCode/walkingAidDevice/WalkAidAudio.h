#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "SDToSPIFFS.h"

class WalkAidAudio {

private:

	AudioGeneratorMP3 *mp3;
	AudioFileSourceSPIFFS *file;
	AudioOutputI2S *out;
	AudioFileSourceID3 *id3;

	SDToSPIFFS s2s;

public:

	WalkAidAudio();
	void init(const char *SDPath, const char *SPIFFSPath);
	void play();

};