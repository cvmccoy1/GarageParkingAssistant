#pragma once

#define ROW1 0
#define ROW2 1 
#define ROW3 2
#define ROW4 3

void InitializeDisplay();

void PrintLine(int row, const char *line);
void PrintfLine(int row, const char *format, ...);