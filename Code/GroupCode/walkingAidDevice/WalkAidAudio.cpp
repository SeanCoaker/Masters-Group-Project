#include "WalkAidAudio.h"

WalkAidAudio::WalkAidAudio() {}

void WalkAidAudio::init(const char *SDPath, const char *SPIFFSPath) {

	this->s2s.init(SDPath, SPIFFSPath);
	this->s2s.loadSDFileToSPIFFS();

}

void WalkAidAudio::play() {

	this->file = new AudioFileSourceSPIFFS(this->s2s.getSPIFFSPath());
  	this->id3 = new AudioFileSourceID3(this->file);
  	this->out = new AudioOutputI2S();
  	this->out->SetPinout(27,26,25);
  	this->mp3 = new AudioGeneratorMP3();
	Serial.println("Play Audio");
	this->mp3->begin(this->id3, this->out);
	while (this->mp3->loop());
	this->mp3->stop();
}