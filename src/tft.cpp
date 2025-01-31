/**
Copyright 2025 Cyril Frausti <cfdev>
This file is part of mWeatherLogger with Adafruit ESP32-S3 Reverse TFT Feather board.

mWeatherLogger is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See licence.txt (MIT License).

www.cyrilfrausti.com
*/

#include "tft.h"
#include <Adafruit_GFX.h>    // Core graphics library

// For 1.14", 1.3", 1.54", and 2.0" TFT with ST7789:
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void init_tft() {
  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  setPower_tft(true);

  // initialize TFT
  tft.init(135, 240);           // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(3);
 // tft.setTextWrap(true);
  tft.print("Hello ESP32!\n");
}

void drawText(const char *text, const unsigned char&line, const unsigned int &color) {
  tft.setTextSize(3);
  tft.setCursor(0, line);
  tft.setTextColor(color, ST77XX_BLACK); // Set back color refresh display
  tft.setTextWrap(true);
  tft.print(text);
 // tft.write(0xF7); //https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#clearing-or-filling-the-screen
}

void setPower_tft(bool state) {
  digitalWrite(TFT_I2C_POWER, state);
  delay(10);
  digitalWrite(TFT_BACKLIGHT, state);
}

void clear_tft() {
  //tft.fillRect(0, 0, 240, 135, ST77XX_BLACK);
  tft.fillScreen(ST77XX_BLACK);
}