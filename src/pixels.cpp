#include <Arduino.h>
#include <NewPing.h>

#include "pixels.h"

#define PIXEL_LEDS_PIN 7 // D4

#define NUMBER_OF_PIXEL_LEDS 15 // The number of LEDs
#define OFF CRGB(0, 0, 0)
#define GREEN CRGB(0, 255, 0)
#define YELLOW CRGB(255, 255, 0)
#define RED CRGB(255, 0, 0)

unsigned long startdistance;   // distance from sensor to begin scan as car pulls in(CENTIMETERS)
unsigned long stopdistance;    // parking position from sensor (CENTIMETERS)
unsigned long incrementdistance = ((startdistance - stopdistance) / NUMBER_OF_PIXEL_LEDS);

CRGB leds[NUMBER_OF_PIXEL_LEDS];

void InitializePixelLeds(unsigned long startDistance, unsigned long stopDistance)
{
    startdistance = startDistance;
    stopdistance = stopDistance;
    FastLED.addLeds<WS2812, PIXEL_LEDS_PIN, GRB>(leds, NUMBER_OF_PIXEL_LEDS); // set up the LED strip
    SetLedColors(NUMBER_OF_PIXEL_LEDS, OFF);                                  // set all LEDS off
}

void SetPixelLeds(unsigned long distance)
{
    if (distance >= startdistance)
    {
        // Greater than start distance -- set all LEDS off
        SetLedColors(NUMBER_OF_PIXEL_LEDS, OFF);
    }
    if (distance <= stopdistance)
    {
        // Less than or equal to the stop distance -- set all LEDS to red
        SetLedColors(NUMBER_OF_PIXEL_LEDS, RED);
    }
    else
    {
        // The distance is somewhere between the start and stop values.
        // Depending on where the distance is, set LED colors accordingly.
        for (int i = 1; i <= NUMBER_OF_PIXEL_LEDS; i++)
        {
            if (distance <= stopdistance + (incrementdistance * i))
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
        leds[i] = color;
    }
    // Turn off the remaining LEDs, if any
    for (int i = lastLed; i < NUMBER_OF_PIXEL_LEDS; i++)
    {
        leds[i] = OFF;
    }
    FastLED.show();
    delay(50);
}

void ResetDistances(unsigned long startDistance, unsigned long stopDistance)
{
    startdistance = startDistance;
    stopdistance = stopDistance;
}