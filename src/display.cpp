#include <Arduino.h>
#include <Wire.h>
#include <stdarg.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#include "display.h"

// These are the h/w pins for the OLED Display (for fastest operation)
#define SCLK_PIN 13 // D13
#define MOSI_PIN 11 // D11
#define DC_PIN 8    // D8
#define CS_PIN 10   // D10
#define RST_PIN 12  // D12


// Using H/W pins for faster operation
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

char _line[LCD_ROWS][LCD_COLUMNS + 1];
char _line_copy[LCD_ROWS][LCD_COLUMNS];

void display();
void updateDisplayRow(int row, uint16_t color, uint16_t bg);

void InitializeDisplay()
{
    tft.begin();
    tft.fillScreen(BLACK);
    tft.setTextSize(1);
}

void PrintLine(int row, const char *line)
{
    if (row < LCD_ROWS)
    {
        strncpy(_line[row], line, LCD_COLUMNS);
        display();
    }
    else
    {
        PrintLine(ERROR_ROW1, "Error: PrintLine()");
        PrintfLine(ERROR_ROW2, PSTR("Row %d out of range!"), row);
        PrintLine(ERROR_ROW3, "");
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
        display();
    }
    else
    {
        PrintLine(ERROR_ROW1, "Error: PrintfLine()");
        PrintfLine(ERROR_ROW2, PSTR("Row %d out of range!"), row);
        PrintLine(ERROR_ROW3, "");
    }
}

void display()
{
    for (int row = 0; row < ERROR_ROW1; row++)
    {
        updateDisplayRow(row, WHITE, BLACK);
    }
    for (int row = ERROR_ROW1; row < ERROR_ROW3; row++)
    {
        updateDisplayRow(row, RED, BLACK);
    }
}

void updateDisplayRow(int row, uint16_t color, uint16_t bg)
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
            int x = column * FONT_WIDTH;
            int y = (row * FONT_HEIGHT) + (FONT_HEIGHT - 1);
            tft.drawChar(x, y, displayChar, color, bg, FONT_SCALE);
        }
    }
}
