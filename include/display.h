#pragma once

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.
#define FONT_WIDTH 6
#define FONT_HEIGHT 8
#define FONT_SCALE 1
#define LCD_ROWS (int)(SCREEN_HEIGHT / FONT_HEIGHT) 
#define LCD_COLUMNS (int)(SCREEN_WIDTH / FONT_WIDTH)

#define ROW1 0
#define ROW2 1 
#define ROW3 2
#define ROW4 3

#define ERROR_ROW1 (int)(LCD_ROWS - 3) 
#define ERROR_ROW2 (int)(LCD_ROWS - 2) 
#define ERROR_ROW3 (int)(LCD_ROWS - 1) 

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void InitializeDisplay();

void PrintLine(int row, const char *line);
void PrintfLine(int row, const char *format, ...);