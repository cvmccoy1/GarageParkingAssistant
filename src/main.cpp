#include <Arduino.h>
#include <FastLED.h>
#include <NewPing.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// defining the pins
#define DHT_PIN 2     // D0
#define LED_PIN 7     // D4
#define TRIGGER_PIN 9 // D6
#define ECHO_PIN 10   // D7
// #define LCD_DATA_PIN 23   // SDA/A4/D18
// #define LCD_CLOCK_PIN 24  // SCL/A5/D19

#define NUM_LEDS 15 // The number of LEDs
#define OFF CRGB(0, 0, 0)
#define GREEN CRGB(0, 255, 0)
#define RED CRGB(255, 0, 0)

#define MAX_DISTANCE 400                             // Maximum distance we want to measure (in centimeters).
#define NUMBER_OF_SAMPLES 15                         // the number of samples to take
#define SPEED_OF_SOUND 0.0343                        // in cm per microseconds
#define DISTANCE(time) ((time * SPEED_OF_SOUND) / 2) // Divide by 2 because we are measuring the duration the sound wave needs to travel to the object and bounce back

#define LCD_ROWS 2
#define LCD_COLUMNS 16

#define DHTTYPE DHT22 // DHT 22  (AM2302)

// defining variables
CRGB leds[NUM_LEDS];
unsigned long startdistance = MAX_DISTANCE; // distance from sensor to begin scan as car pulls in(CENTIMETERS)
unsigned long stopdistance = 115;           // parking position from sensor (CENTIMETERS)
unsigned long incrementdistance = ((startdistance - stopdistance) / NUM_LEDS);
unsigned long delayMS;

void setLedColors(int lastLed, CRGB color);

// create an instance of the ultrasonic distance sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
// create an instance of the temperature/humidity sensor
DHT_Unified dht(DHT_PIN, DHTTYPE);
// create an instance of LCD display: address = 0x27 and configure for a 16 character and 2 line display
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

void setup()
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); // set up the LED strip
  dht.begin();                                           // start the temperature/humidity sensor
  lcd.init();                                            // initialize the lcd
  lcd.clear();                                           // clear the lcd display
  lcd.backlight();                                       // open the backlight
  Serial.begin(9600);                                    // start the serial communication

  // Initialize temperature/humidity device.
  Serial.println(F("DHT22 Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
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
  dht.humidity().getSensor(&sensor);
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
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop()
{
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  int temperature, humidity;
  float speedOfSound, distance, duration;

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    temperature = event.temperature;
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    humidity = event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println(F("%"));
  }
  speedOfSound = 331.4 + (0.6 * temperature) + (0.0124 * humidity); // Calculate speed of sound in m/s

  duration = sonar.ping_median(NUMBER_OF_SAMPLES); // returns duration in microseconds
  duration /= 1000000;                             // Convert microseconds to seconds
  distance = (speedOfSound * duration) / 2;        // Divide by 2 because we are measuring the duration the sound wave needs to travel to the object and bounce back
  distance = distance * 100;                       // meters to centimeters

  // Prints the distance on the Display
  lcd.setCursor(0, 0);     // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Distance: "); // Prints string "Distance" on the LCD
  lcd.print(distance);     // Prints the distance value from the sensor
  lcd.print(" cm");
  // Prints the distance on the Serial Monitor
  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.println(F("cm"));

  if (distance >= startdistance)
  {
    // Greater than start distance -- set all LEDS off
    setLedColors(NUM_LEDS, OFF);
  }
  if (distance <= stopdistance)
  {
    // Less than or equal to the stop distance -- set all LEDS to red
    setLedColors(NUM_LEDS, RED);
  }
  else
  {
    for (int i = 1; i <= NUM_LEDS; i++)
    {
      if (distance <= stopdistance + (incrementdistance * i))
      {
        setLedColors(i, GREEN);
        break;
      }
    }
  }
}

void setLedColors(int lastLed, CRGB color)
{
  for (int i = 0; i < lastLed; i++)
  {
    leds[i] = color;
  }
  for (int i = lastLed; i < NUM_LEDS; i++)
  {
    leds[i] = OFF;
  }
  FastLED.show();
  delay(50);
}
