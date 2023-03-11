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

#define CALCULATE_SPEED_OF_SOUND_INTERVAL 60 // One Minute (60 Seconds)

#define CELSIUS_TO_FAHRENHEIT(temperature) (int)(((double)temperature * 9.0 / 5.0) + 32.5)

// defining variables
static int temperature;
static int humidity;
volatile bool updateSpeedOfSound = true;
float _speedOfSound;
// The Stored Data Manager (EEPROM Settings)
StoredDataManager *_storedDataManager = nullptr;

// Forward referenced functions
void InitializeOneSecondTimerInterrupt();

void DisplayCurrentDistance(unsigned long distance[])
{
  int x[NUM_DIGITAL_PINS][2];
  for (int index = 0; index < NUMBER_OF_SONARS; index++)
  {
    int distanceInFeet = (int)((double)distance[index] / 0.3048 + 0.5);
    x[index][0] = distanceInFeet / 100;
    x[index][1] = distanceInFeet % 100;
  }
  PrintfLine(ROW1, PSTR("D1:%2.2d.%02.2d'  D2:%2.2d.%02.2d'"),  x[0][0], x[0][1], x[1][0], x[1][1]);
}

void DisplayStartDistance(unsigned long distance[])
{
  int x[NUM_DIGITAL_PINS][2];
  for (int index = 0; index < NUMBER_OF_SONARS; index++)
  {
    int distanceInFeet = (int)((double)(distance[index]) / 0.3048 + 0.5);
    x[index][0] = distanceInFeet / 100;
    x[index][1] = distanceInFeet % 100;
  }
  PrintfLine(ROW2, PSTR("S1:%2.2d.%02.2d'  S2:%2.2d.%02.2d'"), x[0][0], x[0][1], x[1][0], x[1][1]);
}

void DisplayStopDistance(unsigned long distance[])
{
  int x[NUM_DIGITAL_PINS][2];
  for (int index = 0; index < NUMBER_OF_SONARS; index++)
  {
    int distanceInFeet = (int)((double)distance[index] / 0.3048 + 0.5);
    x[index][0] = distanceInFeet / 100;
    x[index][1] = distanceInFeet % 100;
  }
  PrintfLine(ROW3, PSTR("E1:%2.2d.%02.2d'  E2:%2.2d.%02.2d'"), x[0][0], x[0][1], x[1][0], x[1][1]);
}

void DisplayTemperatureAndHumidity(int temperature, int humidity)
{
  PrintfLine(ROW4, PSTR("TEMP:%3.1d\337F  HUM:%3.1d%%"), CELSIUS_TO_FAHRENHEIT(temperature), humidity);
}

void setup()
{
  Serial.begin(9600); // start the serial communication
  _storedDataManager = new StoredDataManager();
  InitializeDisplay();
  InitializeSonarDevice();
  InitializeTemperatureAndHumidityDevice();
  InitializePixelLeds();
  InitializeOneSecondTimerInterrupt();
  InitializeButtons(_storedDataManager);
}

void loop()
{
  float speedOfSound;
  if (updateSpeedOfSound)
  {
    updateSpeedOfSound = false;
    speedOfSound = CalculateSpeedOfSound(&temperature, &humidity);
    DisplayTemperatureAndHumidity(temperature, humidity);
  }
  delay(200);
  unsigned long currentDistance[NUMBER_OF_SONARS];
  unsigned long startDistance[NUMBER_OF_SONARS];
  unsigned long stopDistance[NUMBER_OF_SONARS];
  for (int index = 0; index < NUMBER_OF_SONARS; index++)
  {
    currentDistance[index] = CalculateSonicDistance(index, speedOfSound);
    Serial.print("Current Distance");
    Serial.print(index);
    Serial.print(": ");
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

void InitializeOneSecondTimerInterrupt()
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
  OCR1A = 15624; // 16MHz / 1024 / 1Hz - 1 (must be <65536)
  sei();         // Enable back the interrupts
}

// One Second Timer Interrupt Service Routine (ISR)
ISR(TIMER1_COMPA_vect)
{
  // Blink heartbeat LED every interrupt
  static bool heartBeatState = true;
  static int calculateSpeedOfSoundCounter = CALCULATE_SPEED_OF_SOUND_INTERVAL;

  heartBeatState = !heartBeatState;                // Invert LED state
  digitalWrite(HEARTBEAT_LED_PIN, heartBeatState); // Write new state to the LED

  if (--calculateSpeedOfSoundCounter <= 0)
  {
    updateSpeedOfSound = true;
    calculateSpeedOfSoundCounter = CALCULATE_SPEED_OF_SOUND_INTERVAL;
  }
}