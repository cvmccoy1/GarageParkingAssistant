#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdarg.h>

#include "display.h"

// #define LCD_DATA_PIN 23   // SDA/A4/D18
// #define LCD_CLOCK_PIN 24  // SCL/A5/D19

#define LCD_ROWS 4
#define LCD_COLUMNS 20

// create an instance of LCD display: address = 0x27 and configure for a 16 character and 2 line display
LiquidCrystal_I2C _lcd(0x27, LCD_COLUMNS, LCD_ROWS);

char _line[LCD_ROWS][LCD_COLUMNS + 1];
char _line_copy[LCD_ROWS][LCD_COLUMNS];

void display();
void updateDisplayRow(int row);

void InitializeDisplay()
{
    _lcd.init();      // initialize the lcd
    _lcd.clear();     // clear the lcd display
    _lcd.backlight(); // open the backlight
}

void PrintLine(int row, const char *line)
{
    if (row < LCD_ROWS)
    {
        strncpy(_line[row], line, LCD_COLUMNS);
        //Serial.print(F("Serial Row "));
        //Serial.print(row);
        //Serial.print(F(": "));
        //Serial.println(&_line[row][0]);
        display();
    }
    else
    {
        Serial.println(F("PrintLine() Error: row out of range!"));
    }
}

void PrintfLine(int row, const char *format, ...)
{
    if (row < LCD_ROWS)
    {
        va_list args;
        va_start(args, format);
        vsnprintf_P(_line[row], LCD_COLUMNS + 1, format, args);
        va_end(args);
        //Serial.print(F("Serial Row"));
        //Serial.print(row);
        //Serial.print(F(": "));
        //Serial.println(&_line[row][0]);
        display();
    }
    else
    {
        Serial.println(F("PrintfLine() Error: row out of range!"));
    }
}

void display()
{
    for (int row = 0; row < LCD_ROWS; row++)
    {
        updateDisplayRow(row);
    }
}

void updateDisplayRow(int row)
{
    bool isEnd = false;
    for (int column = 0; column < LCD_COLUMNS; column++)
    {
        char displayChar = _line[row][column];
        if (displayChar == 0 || isEnd)
        {
            displayChar = ' ';
            isEnd = true;
        }
        if (displayChar != _line_copy[row][column])
        {
            _line_copy[row][column] = displayChar;
            _lcd.setCursor(column, row);
            _lcd.write(displayChar);
        }
    }
}