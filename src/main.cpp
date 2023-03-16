#include <Arduino.h>
#include <util/atomic.h>

#include "sonar.h"
#include "sensor.h"
#include "display.h"
#include "pixels.h"
#include "buttons.h"
#include "storedData.h"

// defining the pins
#define HEARTBEAT_LED_PIN LED_BUILTIN // D10 (Pin 13)

#define CALCULATE_HEARTBEAT_INTERVAL       1 * 4 // Second (1 Second * 4Hz)
#define CALCULATE_SPEED_OF_SOUND_INTERVAL 60 * 4 // One Minute (60 Seconds * 4Hz)

#define CELSIUS_TO_FAHRENHEIT(temperature) (int)(((double)temperature * 9.0 / 5.0) + 32.5)

// defining variables
static int temperature;
static int humidity;
volatile bool updateSpeedOfSound = true;
volatile bool updateDistances = true;
float _speedOfSound;
// The Stored Data Manager (EEPROM Settings)
StoredDataManager *_storedDataManager = nullptr;

// Forward referenced functions
void Initialize4HzTimerInterrupt();

void DisplayCurrentDistance(unsigned long distance[])
{
  char szF[NUMBER_OF_SONARS][6];
  for (int index = 0; index < NUMBER_OF_SONARS; index++)
  {
    double distanceInFeet = (double)(distance[index]) / 30.48;
    dtostrf(distanceInFeet, 4, 2, szF[index]);
  }
  PrintfLine(ROW1, PSTR("D1:%s'  D2:%s'"), szF[0], szF[1]);
}

void DisplayStartDistance(unsigned long distance[])
{
  char szF[NUMBER_OF_SONARS][6];
  for (int index = 0; index < NUMBER_OF_SONARS; index++)
  {
    double distanceInFeet = (double)(distance[index]) / 30.48;
    dtostrf(distanceInFeet, 4, 2, szF[index]);
  }
  PrintfLine(ROW2, PSTR("S1:%s'  S2:%s'"), szF[0], szF[1]);
}

void DisplayStopDistance(unsigned long distance[])
{
  char szF[NUMBER_OF_SONARS][6];
  for (int index = 0; index < NUMBER_OF_SONARS; index++)
  {
    double distanceInFeet = (double)(distance[index]) / 30.48;
    dtostrf(distanceInFeet, 4, 2, szF[index]);
  }
  PrintfLine(ROW3, PSTR("E1:%s'  E2:%s'"), szF[0], szF[1]);
}

void DisplayTemperatureAndHumidity(int temperature, int humidity)
{
  PrintfLine(ROW4, PSTR("TEMP:%3.1d\337   HUM:%3.1d%%"), CELSIUS_TO_FAHRENHEIT(temperature), humidity);
}

void setup()
{
  Serial.begin(9600); // start the serial communication
  _storedDataManager = new StoredDataManager();
  InitializeDisplay();
  InitializeSonarDevice();
  InitializeTemperatureAndHumidityDevice();
  InitializePixelLeds();
  Initialize4HzTimerInterrupt();
  InitializeButtons(_storedDataManager);
}

void loop()
{
  static float speedOfSound;
  if (updateSpeedOfSound)
  {
    updateSpeedOfSound = false;
    speedOfSound = CalculateSpeedOfSound(&temperature, &humidity);
    DisplayTemperatureAndHumidity(temperature, humidity);
  }
  if (updateDistances)
  {
    updateDistances = false;
    unsigned long currentDistance[NUMBER_OF_SONARS];
    unsigned long startDistance[NUMBER_OF_SONARS];
    unsigned long stopDistance[NUMBER_OF_SONARS];
    for (int index = 0; index < NUMBER_OF_SONARS; index++)
    {
      currentDistance[index] = CalculateSonicDistance(index, speedOfSound);
      Serial.print(F("Current Distance"));
      Serial.print(index);
      Serial.print(F(": "));
      Serial.print(currentDistance[index]);
      Serial.println(F("cm"));

      SetCurrentDistance(index, currentDistance[index]);
      startDistance[index] = _storedDataManager->getStartDistance(index);
      stopDistance[index] = _storedDataManager->getStopDistance(index);
    }
    //  Prints the distance on the Display
    DisplayCurrentDistance(currentDistance);
    DisplayStartDistance(startDistance);
    DisplayStopDistance(stopDistance);
    SetPixelLeds(currentDistance, startDistance, stopDistance);
  }
}

void Initialize4HzTimerInterrupt()
{
  pinMode(HEARTBEAT_LED_PIN, OUTPUT);  // set up the Heartbeat LED
  cli();                               // Disable interrupts while setting up registers
  TCCR1A = 0;                          // Reset entire TCCR1A to 0
  TCCR1B = 0;                          // Reset entire TCCR1B to 0
  TCNT1 = 0;                           // initialize counter value to 0
  TCCR1B |= (1 << WGM12);              // turn on CTC (Clear timer on compare match) mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);             // Enable compare match mode
  // Set Compare Match Register = clock frequency / prescaler / interrupt frequency - 1 (cause its zero based)
  OCR1A = 3906; // 16MHz / 1024 / 4Hz - 1 (must be <65536)
  sei();         // Enable back the interrupts
}

// 4Hz Second Timer Interrupt Service Routine (ISR)
ISR(TIMER1_COMPA_vect)
{
  // Blink heartbeat LED every interrupt
  static bool heartBeatState = true;
  static int heartbeatCounter = CALCULATE_HEARTBEAT_INTERVAL;
  static int calculateSpeedOfSoundCounter = CALCULATE_SPEED_OF_SOUND_INTERVAL;

  if (--heartbeatCounter <= 0)
  {
    heartBeatState = !heartBeatState;                // Invert LED state
    digitalWrite(HEARTBEAT_LED_PIN, heartBeatState); // Write new state to the LED
    heartbeatCounter = CALCULATE_HEARTBEAT_INTERVAL;
  }

  if (--calculateSpeedOfSoundCounter <= 0)
  {
    updateSpeedOfSound = true;
    calculateSpeedOfSoundCounter = CALCULATE_SPEED_OF_SOUND_INTERVAL;
  }
  updateDistances = true;
}