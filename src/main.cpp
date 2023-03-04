#include <Arduino.h>
#include <util/atomic.h>

#include "sonic.h"
#include "sensor.h"
#include "display.h"
#include "pixels.h"

// defining the pins
#define HEARTBEAT_LED_PIN LED_BUILTIN // D10 (Pin 13)

#define CALCULATE_SPEED_OF_SOUND_INTERVAL 60 // One Minute (60 Seconds)

// defining variables
volatile float speedOfSound;

// Forward referenced functions
void InitializeOneSecondTimerInterrupt();

void setup()
{
  Serial.begin(9600);                 // start the serial communication
  pinMode(HEARTBEAT_LED_PIN, OUTPUT); // set up the Heartbeat LED
  InitializeTemperatureAndHumidityDevice();
  speedOfSound = CalculateSpeedOfSound(); // Calculate the initial speedOfSound
  InitializePixelLeds(MAX_DISTANCE, 115);
  InitializeDisplay();
  InitializeOneSecondTimerInterrupt();
}

void loop()
{
  float sos;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    sos = speedOfSound;
  }
  unsigned long distance = CalculateSonicDistance(sos);
  // Prints the distance on the Display
  PrintfLine(2, PSTR("Distance: %3dcm"), distance);

  SetPixelLeds(distance);
}

void InitializeOneSecondTimerInterrupt()
{
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
    float sos = CalculateSpeedOfSound();
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      speedOfSound = sos;
    }
    calculateSpeedOfSoundCounter = CALCULATE_SPEED_OF_SOUND_INTERVAL;
  }
}