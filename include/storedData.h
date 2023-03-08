#pragma once

#define EEPROM_ADDRESS 0

#define MAXIMUM_DISTANCE 500
#define DEFAULT_START_DISTANCE 400
#define DEFAULT_STOP_DISTANCE 115

struct StoredData
{
    int startDistance;
    int stopDistance;
};

class StoredDataManager
{
public:
    StoredDataManager();
    void Save();

    int getStartDistance();
    int getStopDistance();

    void setStartDistance(int distance);
    void setStopDistance(int distance);

private:
    StoredData _storedData;
    bool _isDirty = false;
    void validateStoredData();
};