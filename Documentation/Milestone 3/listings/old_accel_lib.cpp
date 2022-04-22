/*************************** READ FROM I2C **************************/
/*                Start; Send Address To Read; End                  */
void ADXL345::readFromI2C(byte address, int num, byte _buff[]) {
	Wire.beginTransmission(ADXL345_DEVICE);  
	Wire.write(address);             
	Wire.endTransmission();         
	
	Wire.beginTransmission(ADXL345_DEVICE); 
	Wire.requestFrom(ADXL345_DEVICE, num);  // Request 6 Bytes
	
	int i = 0;
	while(Wire.available())					
	{ 
		_buff[i] = Wire.read();				// Receive Byte
		i++;
	}
	if(i != num){
		status = ADXL345_ERROR;
		error_code = ADXL345_READ_ERROR;
	}
	Wire.endTransmission();         	
}