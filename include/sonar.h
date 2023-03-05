#pragma once

#define MAX_DISTANCE 400 // maximum distance we want to measure (in centimeters).

void InitializeTemperatureAndHumidityDevice();
unsigned long CalculateSonicDistance(float speedOfSound);