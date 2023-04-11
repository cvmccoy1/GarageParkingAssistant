#pragma once

#define ROW1 0
#define ROW2 1 
#define ROW3 2
#define ROW4 3

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