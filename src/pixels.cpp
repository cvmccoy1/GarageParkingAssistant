#include <Arduino.h>
#include <NewPing.h>

#include "pixels.h"

#define PIXEL_LEDS_PIN 7 // D4

#define NUMBER_OF_PIXEL_LEDS 15 // The number of LEDs
#define OFF CRGB(0, 0, 0)
#define GREEN CRGB(0, 255, 0)
#define YELLOW CRGB(255, 255, 0)
#define RED CRGB(255, 0, 0)

CRGB _pixelLeds[NUMBER_OF_PIXEL_LEDS];

void InitializePixelLeds()
{
    FastLED.addLeds<WS2812, PIXEL_LEDS_PIN, GRB>(_pixelLeds, NUMBER_OF_PIXEL_LEDS); // set up the LED strip
    SetLedColors(NUMBER_OF_PIXEL_LEDS, OFF);                                        // set all LEDS off
}

void SetPixelLeds(unsigned long currentDistance, unsigned long startDistance, unsigned long stopDistance)
{
    if (currentDistance >= startDistance)
    {
        // Greater than start distance -- set all LEDS off
        SetLedColors(NUMBER_OF_PIXEL_LEDS, OFF);
    }
    if (currentDistance <= stopDistance)
    {
        // Less than or equal to the stop distance -- set all LEDS to red
        SetLedColors(NUMBER_OF_PIXEL_LEDS, RED);
    }
    else
    {
        unsigned long incrementalDistance = ((startDistance - stopDistance) / NUMBER_OF_PIXEL_LEDS);
        // The distance is somewhere between the start and stop values.
        // Depending on where the distance is, set LED colors accordingly.
        for (int i = 1; i <= NUMBER_OF_PIXEL_LEDS; i++)
        {
            if (currentDistance <= stopDistance + (incrementalDistance * i))
            {
                SetLedColors(i, GREEN);
                break;
            }
        }
    }
}

void SetLedColors(int lastLed, CRGB color)
{
    // Set the specificed number of lEDs to the specified color
    for (int i = 0; i < lastLed; i++)
    {
        _pixelLeds[i] = color;
    }
    // Turn off the remaining LEDs, if any
    for (int i = lastLed; i < NUMBER_OF_PIXEL_LEDS; i++)
    {
        _pixelLeds[i] = OFF;
    }
    FastLED.show();
    delay(50);
}
