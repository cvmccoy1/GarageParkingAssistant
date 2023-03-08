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
    if (_isDirty)
    {
        EEPROM.put(EEPROM_ADDRESS, _storedData);
        _isDirty = false;
    }
}

int StoredDataManager::getStartDistance() { return _storedData.startDistance; }
int StoredDataManager::getStopDistance()  { return _storedData.stopDistance; }

void StoredDataManager::setStartDistance(int distance)
{
    if (_storedData.startDistance != distance)
    {
        _storedData.startDistance = distance;
        _isDirty = true;
    }
}

void StoredDataManager::setStopDistance(int distance)
{
    if (_storedData.stopDistance != distance)
    {
        _storedData.stopDistance = distance;
        _isDirty = true;
    }
}

// Validates the Setup Data values read from the EEPROM.
// They should only be invalid until the first write to the EEPROM.
void StoredDataManager::validateStoredData()
{
    if (_storedData.startDistance < 0 || _storedData.startDistance > MAXIMUM_DISTANCE)
    {
        _storedData.startDistance = DEFAULT_START_DISTANCE;
    }
    if (_storedData.stopDistance < 0 || _storedData.stopDistance >= _storedData.startDistance)
    {
        _storedData.stopDistance = DEFAULT_STOP_DISTANCE;
    }
}