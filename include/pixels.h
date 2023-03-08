#pragma once

#include <FastLED.h>

void InitializePixelLeds();
void SetPixelLeds(unsigned long currentDistance, unsigned long startDistance, unsigned long stopDistance);
void SetLedColors(int lastLed, CRGB color);