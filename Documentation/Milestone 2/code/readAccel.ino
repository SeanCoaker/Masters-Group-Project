void readAverageAccel(float &xRef, float &yRef, float &zRef) {

	float xtot = 0, ytot = 0, ztot = 0;

	for (int i = 0; i < 50; i++) {
		int x, y, z;
		adxl.readAccel(&x, &y, &z);

		// Converts accel reading to unit of G
		float xa = x * 0.004;
		xtot += xa;

		// Converts accel reading to unit of G
		float ya = y * 0.004;
		ytot += ya;

		// Converts accel reading to unit of G
		float za = z * 0.004;
		ztot += za;
	}

	xRef = xtot / 50; 
	yRef = ytot / 50;
	zRef = ztot / 50;

}