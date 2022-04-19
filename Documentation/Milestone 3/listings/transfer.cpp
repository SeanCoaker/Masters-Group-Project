void SDToSPIFFS::loadSDFileToSPIFFS() {

	FsFile sourceFile = sd.open(this->SDPath);
	if (sourceFile) {
		File destFile = SPIFFS.open(this->SPIFFSPath, FILE_WRITE);
		static uint8_t buf[512];
		while(sourceFile.read(buf, 512)) {
			destFile.write(buf, 512);
		}
		destFile.close();
		sourceFile.close();
	} 
  	
	sd.end();
}