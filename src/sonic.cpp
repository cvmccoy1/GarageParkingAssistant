#include <Arduino.h>
#include <NewPing.h>

#include "sonic.h"
#include "main.h"

#define TRIGGER_PIN 9 // D6
#define ECHO_PIN 10   // D7

#define NUMBER_OF_ULTRASONIC_SAMPLES 10 // the number of ultrasonic samples to take

// create an instance of the ultrasonic distance sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned long CalculateSonicDistance(float speedOfSound)
{
    float distance, duration;

    duration = sonar.ping_median(NUMBER_OF_ULTRASONIC_SAMPLES); // returns duration in microseconds
    duration /= 1000000;                                        // Convert microseconds to seconds
    distance = (speedOfSound * duration) / 2;                   // Divide by 2 because we are measuring the duration the sound wave needs to travel to the object and bounce back
    distance = distance * 100;                            // meters to centimeters

    // Prints the distance on the Serial Monitor
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(F("cm"));

    return (unsigned long)(distance + 0.5);
}