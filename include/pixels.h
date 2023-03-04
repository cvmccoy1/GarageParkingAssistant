#pragma once

#include <FastLED.h>

void InitializePixelLeds(unsigned long startDistance, unsigned long stopDistance);
void SetPixelLeds(unsigned long distance);
void SetLedColors(int lastLed, CRGB color);
void ResetDistances(unsigned long startDistance, unsigned long stopDistance);