#include "DebugLED.h"

DebugLED::DebugLED() {
    tp = TinyPICO();
}

void DebugLED::poweredOn() {
    tp.DotStar_SetPixelColor(0,128,0);
    tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
    delay(1000);
    tp.DotStar_SetBrightness(0);
    tp.DotStar_Show();
}

void DebugLED::initError() {
    tp.DotStar_SetPixelColor(128,0,0);
    tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
}

void DebugLED::peerSetupError() {
    tp.DotStar_SetPixelColor(255,165,0);
    tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
}