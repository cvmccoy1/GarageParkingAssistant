#pragma once

extern float gSpeedOfSound;

void InitializeTemperatureAndHumidityDevice();
float CalculateSpeedOfSound(int *temperature, int *humidity);