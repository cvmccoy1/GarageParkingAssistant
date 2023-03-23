#include <Arduino.h>
#include <NewPing.h>

#include "main.h"
#include "pixels.h"

#define PIXEL_LEDS1_PIN A1
#define PIXEL_LEDS2_PIN A2 

#define NUMBER_OF_PIXEL_LEDS 15 // The number of LEDs
#define OFF CRGB(0, 0, 0)
#define GREEN CRGB(0, 255, 0)
#define YELLOW CRGB(255, 255, 0)
#define RED CRGB(255, 0, 0)

CRGB _pixelLeds[NUMBER_OF_SONARS][NUMBER_OF_PIXEL_LEDS];

void SetLedColors(int index, int lastLed, CRGB color);

void InitializePixelLeds()
{
    FastLED.addLeds<WS2812, PIXEL_LEDS1_PIN, GRB>(_pixelLeds[0], NUMBER_OF_PIXEL_LEDS); // set up the LED strip1
    SetLedColors(0, NUMBER_OF_PIXEL_LEDS, OFF);                                         // set all LEDS off
    FastLED.addLeds<WS2812, PIXEL_LEDS2_PIN, GRB>(_pixelLeds[1], NUMBER_OF_PIXEL_LEDS); // set up the LED strip2
    SetLedColors(1, NUMBER_OF_PIXEL_LEDS, OFF);                                         // set all LEDS off
}

void SetPixelLeds(unsigned long currentDistance[], unsigned long startDistance[], unsigned long stopDistance[])
{
    for (int index = 0; index < NUMBER_OF_SONARS; index++)
    {
        Serial.print("SetPixelLeds: Strip ");
        Serial.print(index);
        Serial.print(", Current Distance = ");
        Serial.print(currentDistance[index]);
        Serial.print(", Start = ");
        Serial.print(startDistance[index]);
        Serial.print(", Stop = ");
        Serial.println(stopDistance[index]);
        if (currentDistance[index] >= startDistance[index])
        {
            // Greater than start distance -- set all LEDS off
            Serial.print("Strip ");
            Serial.print(index);
            Serial.println(": Greater than start distance -- set all LEDS off");
            SetLedColors(index, NUMBER_OF_PIXEL_LEDS, OFF);
        }
        else if (currentDistance[index] <= stopDistance[index])
        {
            // Less than or equal to the stop distance -- set all LEDS to red
            Serial.print("Strip ");
            Serial.print(index);
            Serial.println(": Less than or equal to the stop distance -- set all LEDS to red");
            SetLedColors(index, NUMBER_OF_PIXEL_LEDS, RED);
        }
        else
        {
            unsigned long incrementalDistance = ((startDistance[index] - stopDistance[index]) / NUMBER_OF_PIXEL_LEDS);
            // The distance is somewhere between the start and stop values.
            // Depending on where the distance is, set LED colors accordingly.
            for (int led = 1; led <= NUMBER_OF_PIXEL_LEDS; led++)
            {
                if (currentDistance[index] <= stopDistance[index] + (incrementalDistance * led))
                {
                    Serial.print("Strip ");
                    Serial.print(index);
                    Serial.print(": The distance is somewhere between the start and stop values: Last LED ");
                    Serial.println(led);
                    SetLedColors(index, led, GREEN);
                    break;
                }
            }
        }
    }
}

void SetLedColors(int index, int lastLed, CRGB color)
{
    Serial.print("SetLedColors: ");
    // Set the specificed number of lEDs to the specified color
    for (int i = 0; i < lastLed; i++)
    {
        _pixelLeds[index][i] = color;
        Serial.print(color);
        Serial.print(" ");
    }
    // Turn off the remaining LEDs, if any
    for (int i = lastLed; i < NUMBER_OF_PIXEL_LEDS; i++)
    {
        _pixelLeds[index][i] = OFF;
        Serial.print("Off ");
    }
    Serial.println();
    FastLED.show();
    delay(50);
}
