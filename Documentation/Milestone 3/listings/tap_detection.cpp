bool result = false;

byte interrupts = adxl.getInterruptSource();
if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
    delay(250);

    if (!adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
        result = addStepToCounter();
    }

    delay(250);
}

return result;
