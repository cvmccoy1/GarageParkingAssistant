#pragma once

#include "main.h"

#define EEPROM_ADDRESS 0

#define MAXIMUM_DISTANCE 500
#define DEFAULT_START_DISTANCE 400
#define DEFAULT_STOP_DISTANCE 115

struct StoredData
{
    int startDistance[NUMBER_OF_SONARS];
    int stopDistance[NUMBER_OF_SONARS];
};

class StoredDataManager
{
public:
    StoredDataManager();
    void Save();

    int getStartDistance(int index);
    int getStopDistance(int index);

    void setStartDistance(int index, int distance);
    void setStopDistance(int index, int distance);

private:
    StoredData _storedData;
    bool _isDirty = false;
    void validateStoredData();
};