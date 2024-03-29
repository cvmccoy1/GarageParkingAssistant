

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "sensor.h"
#include "display.h"

#define DHT_PIN A3   // D5
#define DHTTYPE DHT22 // DHT 22  (AM2302)

#define DEFAULT_SPEED_OF_SOUND 343.0 // in m/s
#define SPEED_OF_SOUND_CALCULATED(temperature, humidity) (331.4 + (0.6 * temperature) + (0.0124 * humidity))

// create an instance of the temperature/humidity sensor
// DHT_Unified _sensor(DHT_PIN, DHTTYPE);
DHT_Unified _sensor(DHT_PIN, DHTTYPE);

void InitializeTemperatureAndHumidityDevice()
{
    _sensor.begin(); // start the temperature/humidity sensor
    delay(2000);
    //Serial.println(F("DHT22 Unified Sensor Setup"));
    // Print temperature sensor details.
    sensor_t sensor;
    _sensor.temperature().getSensor(&sensor);
    //Serial.println(F("------------------------------------"));
    //Serial.println(F("Temperature Sensor"));
    //Serial.print(F("Sensor Type: "));
    //Serial.println(sensor.name);
    //Serial.print(F("Driver Ver:  "));
    //Serial.println(sensor.version);
    //Serial.print(F("Unique ID:   "));
    //Serial.println(sensor.sensor_id);
    //Serial.print(F("Max Value:   "));
    //Serial.print(sensor.max_value);
    //Serial.println(F("°C"));
    //Serial.print(F("Min Value:   "));
    //Serial.print(sensor.min_value);
    //Serial.println(F("°C"));
    //Serial.print(F("Resolution:  "));
    //Serial.print(sensor.resolution);
    //Serial.println(F("°C"));
    //Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    _sensor.humidity().getSensor(&sensor);
    //Serial.println(F("Humidity Sensor"));
    //Serial.print(F("Sensor Type: "));
    //Serial.println(sensor.name);
    //Serial.print(F("Driver Ver:  "));
    //Serial.println(sensor.version);
    //Serial.print(F("Unique ID:   "));
    //Serial.println(sensor.sensor_id);
    //Serial.print(F("Max Value:   "));
    //Serial.print(sensor.max_value);
    //Serial.println(F("%"));
    //Serial.print(F("Min Value:   "));
    //Serial.print(sensor.min_value);
    //Serial.println(F("%"));
    //Serial.print(F("Resolution:  "));
    //Serial.print(sensor.resolution);
    //Serial.println(F("%"));
    //Serial.print(F("Minimum Delay: "));
    //Serial.print((double(sensor.min_delay) / 1000000.0));
    //Serial.println(F(" sec"));
    //Serial.println(F("------------------------------------"));
}

float CalculateSpeedOfSound(int *temperature, int *humidity)
{
    sensors_event_t event;
    // Get temperature event and retrieve its value.
    _sensor.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        //Serial.println(F("Error reading temperature!"));
        *temperature = 25;
    }
    else
    {
        *temperature = event.temperature + 0.5;
        //Serial.print(F("Temperature: "));
        //Serial.print(*temperature);
        //Serial.println(F("°C"));
    }
    // Get humidity event and retrieve its value.
    _sensor.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        //Serial.println(F("Error reading humidity!"));
        *humidity = 50;
    }
    else
    {
        *humidity = event.relative_humidity + 0.5;
        //Serial.print(F("Humidity: "));
        //Serial.print(*humidity);
        //Serial.println(F("%"));
    }
    float speedOfSound = DEFAULT_SPEED_OF_SOUND;
    // Calculate speed of sound in m/s
    if (!isnan(*temperature) && !isnan(*humidity))
        speedOfSound = SPEED_OF_SOUND_CALCULATED(*temperature, *humidity);

    //Serial.print(F("Speed of Sound: "));
    //Serial.print(speedOfSound);
    //Serial.println(F("m/s"));
    return speedOfSound;
}