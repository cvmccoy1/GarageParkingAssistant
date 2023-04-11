#include <Arduino.h>
#include <NewPing.h>

#include "sonar.h"
#include "main.h"

#define TRIGGER1_PIN 4 // D4
#define ECHO1_PIN 5    // D5
#define TRIGGER2_PIN 6 // D6
#define ECHO2_PIN 7    // D7

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
    int triggerPin = TRIGGER1_PIN + (index * NUMBER_OF_SONARS);
    int echoPin = ECHO1_PIN + (index * NUMBER_OF_SONARS);
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(5);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    unsigned long duration = pulseIn(echoPin, HIGH);
    //Serial.print("Duration");
    //Serial.print(index);
    //Serial.print(": ");
    //Serial.println(duration);

    // SpeedOfSound (m/s) * duration (us) / 1,000,000 (us/s) * 100 (cm/m) / 2 (number of trips--to and back) + 0.5 (to round up)
    return ((speedOfSound * duration / 20000) + 0.5); // in centimeters
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