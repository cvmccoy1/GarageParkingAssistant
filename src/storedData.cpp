#include <Arduino.h>
#include <EEPROM.h>
#include "storedData.h"

StoredDataManager::StoredDataManager()
{
    EEPROM.get(EEPROM_ADDRESS, _storedData);
    validateStoredData();
    _isDirty = false;
};

void StoredDataManager::Save()
{
    Serial.print("Save():");
    if (_isDirty)
    {
        EEPROM.put(EEPROM_ADDRESS, _storedData);
        _isDirty = false;
    }
    Serial.println();
}

int StoredDataManager::getStartDistance(int index) { return _storedData.startDistance[index]; }
int StoredDataManager::getStopDistance(int index) { return _storedData.stopDistance[index]; }

void StoredDataManager::setStartDistance(int index, int distance)
{
    if (_storedData.startDistance[index] != distance)
    {
        _storedData.startDistance[index] = distance;
        _isDirty = true;
    }
}

void StoredDataManager::setStopDistance(int index, int distance)
{
    if (_storedData.stopDistance[index] != distance)
    {
        _storedData.stopDistance[index] = distance;
        _isDirty = true;
    }
}

// Validates the Setup Data values read from the EEPROM.
// They should only be invalid until the first write to the EEPROM.
void StoredDataManager::validateStoredData()
{
    for (int index = 0; index < NUMBER_OF_SONARS; index++)
    {
        if (_storedData.startDistance[index] < 0 || _storedData.startDistance[index] > MAXIMUM_DISTANCE)
        {
            _storedData.startDistance[index] = DEFAULT_START_DISTANCE;
        }
        if (_storedData.stopDistance[index] < 0 || _storedData.stopDistance[index] >= _storedData.startDistance[index])
        {
            _storedData.stopDistance[index] = DEFAULT_STOP_DISTANCE;
        }
    }
}