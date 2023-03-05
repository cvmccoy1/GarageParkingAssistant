#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "sensor.h"
#include "display.h"

#define DHT_PIN 2     // D0
#define DHTTYPE DHT22 // DHT 22  (AM2302)

#define SPEED_OF_SOUND 343.0 // in m/s

// create an instance of the temperature/humidity sensor
DHT_Unified _sensor(DHT_PIN, DHTTYPE);

void InitializeTemperatureAndHumidityDevice()
{
    _sensor.begin(); // start the temperature/humidity sensor
    Serial.println(F("DHT22 Unified Sensor Setup"));
    // Print temperature sensor details.
    sensor_t sensor;
    _sensor.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("°C"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("°C"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    _sensor.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("%"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("%"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("%"));
    Serial.print(F("Minimum Delay: "));
    Serial.println(sensor.min_delay);
    Serial.println(F("ms"));
    Serial.println(F("------------------------------------"));
}

float CalculateSpeedOfSound()
{
    // Get temperature event and print its value.
    float speedOfSound = SPEED_OF_SOUND;
    sensors_event_t event;
    static int temperature = 0;
    static int humidity = 0;

    bool isDhtWorking = true;

    _sensor.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
        isDhtWorking = false;
    }
    else
    {
        temperature = event.temperature;
        PrintfLine(0, PSTR("Temperature: %3d\337C"), temperature);
        Serial.print(F("Temperature: "));
        Serial.print(temperature);
        Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    _sensor.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println(F("Error reading humidity!"));
        isDhtWorking = false;
    }
    else
    {
        humidity = event.relative_humidity;
        PrintfLine(1, PSTR("Humidity: %3d%%"), humidity);
        Serial.print(F("Humidity: "));
        Serial.print(humidity);
        Serial.println(F("%"));
    }
    // Calculate speed of sound in m/s
    if (isDhtWorking)
    {
        speedOfSound = 331.4 + (0.6 * temperature) + (0.0124 * humidity);
    }
    return speedOfSound;
}