#include <Arduino.h>
#include <NewPing.h>

#include "sonar.h"
#include "main.h"

#define TRIGGER1_PIN 9  // D9
#define ECHO1_PIN 10    // D10
#define TRIGGER2_PIN 11 // D11
#define ECHO2_PIN 12    // D12

void InitializeSonarDevice()
{
    // Define inputs and outputs
    pinMode(TRIGGER1_PIN, OUTPUT);
    pinMode(ECHO1_PIN, INPUT);
    pinMode(TRIGGER2_PIN, OUTPUT);
    pinMode(ECHO2_PIN, INPUT);
}

unsigned long CalculateSonicDistance(int index, float speedOfSound)
{
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(TRIGGER1_PIN + index, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIGGER1_PIN + index, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER1_PIN + index, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    unsigned long duration = pulseIn(ECHO1_PIN, HIGH);
    return ((speedOfSound * duration * 10000.0) / 2.0 + 0.5); // Divide by 2 because we are measuring the duration the sound wave needs to travel to the object and bounce back
}

/*
#define NUMBER_OF_ULTRASONIC_SAMPLES 10 // the number of ultrasonic samples to take

// create an instance of the ultrasonic distance sensor
NewPing _sonar(TRIGGER_PIN, ECHO_PIN);

unsigned long CalculateSonicDistance(float speedOfSound)
{
    float distance, duration;

    duration = _sonar.ping_median(NUMBER_OF_ULTRASONIC_SAMPLES); // returns duration in microseconds
    duration /= 1000000;                                         // Convert microseconds to seconds
    distance = (speedOfSound * duration) / 2;                    // Divide by 2 because we are measuring the duration the sound wave needs to travel to the object and bounce back
    distance = distance * 100;                                   // meters to centimeters

    // Prints the distance on the Serial Monitor
    //Serial.print(F("Distance: "));
    //Serial.print(distance);
    //Serial.println(F("cm"));

    return (unsigned long)(distance + 0.5);
}
*/