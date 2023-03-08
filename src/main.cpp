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

void DisplayCurrentDistance(unsigned long distance)
{
  int distanceInFeet = (int)((double)distance / 0.3048 + 0.5);
  int x = distanceInFeet / 100;
  int y = distanceInFeet % 100;
  PrintfLine(ROW1, PSTR("DISTANCE:    %2d.%02dft"), x, y);
}

void DisplayStartDistance(unsigned long distance)
{
  int distanceInFeet = (int)((double)distance / 0.3048 + 0.5);
  int x = distanceInFeet / 100;
  int y = distanceInFeet % 100;
  PrintfLine(ROW2, PSTR("START DISTANCE:%2d.%02d"), x, y);
}

void DisplayStopDistance(unsigned long distance)
{
  int distanceInFeet = (int)((double)distance / 0.3048 + 0.5);
  int x = distanceInFeet / 100;
  int y = distanceInFeet % 100;
  PrintfLine(ROW3, PSTR("STOP DISTANCE: %2d.%02d"), x, y);
}

void DisplayTemperatureAndHumidity(int temperature, int humidity)
{
  PrintfLine(ROW4, PSTR("TEMP:%3.1d\337F HUM:%3.1d%%"), CELSIUS_TO_FAHRENHEIT(temperature), humidity);
}

void setup()
{
  Serial.begin(9600); // start the serial communication
  _storedDataManager = new StoredDataManager();
  InitializeDisplay();
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
  unsigned long currentDistance = CalculateSonicDistance(speedOfSound);
  unsigned long startDistance;
  unsigned long stopDistance;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    SetCurrentDistance(currentDistance);
    startDistance = _storedDataManager->getStartDistance();
    stopDistance = _storedDataManager->getStopDistance();
  }
  // Serial.print(F("Distance: "));
  // Serial.print(distance);
  // Serial.println(F("cm"));
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
