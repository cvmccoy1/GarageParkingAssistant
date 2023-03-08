#include <Arduino.h>
#include <util/atomic.h>

#include "buttons.h"
#include "display.h"

#define START_DISTANCE_BUTTON 2 // D2
#define STOP_DISTANCE_BUTTON 3  // D3

StoredDataManager *_localStoredDataManager = nullptr;
volatile unsigned long _currentDistance;



void StartDistanceInterrupt();
void StopDistanceInterrupt();

void InitializeButtons(StoredDataManager *storedDataManager)
{
    _localStoredDataManager = storedDataManager;

    pinMode(START_DISTANCE_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(START_DISTANCE_BUTTON), StartDistanceInterrupt, FALLING);

    pinMode(STOP_DISTANCE_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(STOP_DISTANCE_BUTTON), StopDistanceInterrupt, FALLING);
}

void StartDistanceInterrupt()
{
    if (digitalRead(START_DISTANCE_BUTTON) == 0)
    {
        unsigned distance;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            distance = _currentDistance;
            _localStoredDataManager->setStartDistance(distance);
            _localStoredDataManager->Save();
        }
        Serial.print(">>>> Start Distance: ");
        Serial.println(distance);
    }
}

void StopDistanceInterrupt()
{
    if (digitalRead(STOP_DISTANCE_BUTTON) == 0)
    {
        unsigned distance;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            distance = _currentDistance;
            _localStoredDataManager->setStopDistance(distance);
            _localStoredDataManager->Save();
        }
        Serial.print(">>>> Stop Distance: ");
        Serial.println(distance);
    }
}

void SetCurrentDistance(unsigned long distance)
{
    _currentDistance = distance;
}
