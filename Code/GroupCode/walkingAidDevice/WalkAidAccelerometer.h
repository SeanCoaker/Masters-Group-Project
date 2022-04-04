#include <SparkFun_ADXL345.h>

class WalkAidAccelerometer {

private:

	double singleTapThresholdValue;
	double singleTapThresholdDuration;
	double doubleTapLatency;
	double doubleTapWindow;

	bool stepDetected;
	long lastStepTime;
	unsigned long period;

	ADXL345 adxl;

public:

	WalkAidAccelerometer();

	void init(double singleTapThresholdValue, double singleTapThresholdDuration
		, double doubleTapLatency, double doubleTapWindow, unsigned long period);
	bool checkForStep();
	bool isStepDetected();
	long getLastStepTime();
	unsigned long getPeriod();

};