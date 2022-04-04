#include <SparkFun_ADXL345.h>

class WearableAccelerometer {

private:

	double singleTapThresholdValue;
	double singleTapThresholdDuration;
	double doubleTapLatency;
	double doubleTapWindow;

	ADXL345 adxl;

	unsigned long startMillis;
	unsigned long currentMillis;
	unsigned long period;
	bool stepTimerStarted;
	int steps;

public:

	WearableAccelerometer();

	void init(double singleTapThresholdValue, double singleTapThresholdDuration
		, double doubleTapLatency, double doubleTapWindow, unsigned long period);
	bool poll();
	bool addStepToCounter();
	
};