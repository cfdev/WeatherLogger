
#ifndef TFT_H
#define TFT_H
#pragma once

#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

// Pins definitions
#define TFT_RST  41 
#define TFT_CS   42
#define TFT_DC   40
#define TFT_I2C_POWER 7
#define TFT_BACKLIGHT 45

#define TFT_LINE1 0
#define TFT_LINE2 25
#define TFT_LINE3 50
#define TFT_LINE4 75
#define TFT_LINE5 100

// Functions
void init_tft(void);
void setPower_tft(bool state);
void drawText(const char *text, const unsigned char&line=TFT_LINE1, const unsigned int &color=ST77XX_WHITE);
void clear_tft();

#endif // TFT_H
