#include <Arduino.h>
#include <util/atomic.h>

#include "main.h"
#include "buttons.h"
#include "display.h"

#define START_DISTANCE_BUTTON 2 // D2
#define STOP_DISTANCE_BUTTON 3  // D3
#define DISTANCE_INDEX_SWITCH 4 // D4

StoredDataManager *_localStoredDataManager = nullptr;
volatile unsigned long _currentDistance[NUMBER_OF_SONARS];

void StartDistanceInterrupt();
void StopDistanceInterrupt();

void InitializeButtons(StoredDataManager *storedDataManager)
{
    _localStoredDataManager = storedDataManager;

    pinMode(START_DISTANCE_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(START_DISTANCE_BUTTON), StartDistanceInterrupt, FALLING);

    pinMode(STOP_DISTANCE_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(STOP_DISTANCE_BUTTON), StopDistanceInterrupt, FALLING);

    pinMode(DISTANCE_INDEX_SWITCH, INPUT_PULLUP);
}

void StartDistanceInterrupt()
{
    if (digitalRead(START_DISTANCE_BUTTON) == 0)
    {
        unsigned distance;
        int index = digitalRead(DISTANCE_INDEX_SWITCH);
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            distance = _currentDistance[index];
            _localStoredDataManager->setStartDistance(index, distance);
            _localStoredDataManager->Save();
        }
        Serial.print(">>>> Start Distance");
        Serial.print(index);
        Serial.print(": ");
        Serial.println(distance);
    }
}

void StopDistanceInterrupt()
{
    if (digitalRead(STOP_DISTANCE_BUTTON) == 0)
    {
        unsigned distance;
        int index = digitalRead(DISTANCE_INDEX_SWITCH);
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            distance = _currentDistance[index];
            _localStoredDataManager->setStopDistance(index, distance);
            _localStoredDataManager->Save();
        }
        Serial.print(">>>> Stop Distance");
        Serial.print(index);
        Serial.print(": ");
        Serial.println(distance);
    }
}

void SetCurrentDistance(int index, unsigned long distance)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        _currentDistance[index] = distance;
    }
}
