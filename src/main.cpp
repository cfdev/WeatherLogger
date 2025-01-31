/**
Copyright 2025 Cyril Frausti <cfdev>
This file is part of mWeatherLogger with Adafruit ESP32-S3 Reverse TFT Feather board.

mWeatherLogger is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See licence.txt (MIT License).

www.cyrilfrausti.com
*/

#include <Arduino.h>
#include <string>
#include <Adafruit_MAX1704X.h>

#include "tft.h"
#include "sensor_bme280.h"
#include "sdcard.h"

String value;
Adafruit_MAX17048 maxlipo;
float cellVoltage=0.0;

#define BUTTON_D2 2 // GPIO2

void setI2CPower(bool state) {
  digitalWrite(I2C_POWER, state);
  delay(10);
}

void displayInfos(){

  cellVoltage = maxlipo.cellVoltage();
  if (!isnan(cellVoltage)) {

    int battpercent = maxlipo.cellPercent();
    if(battpercent>100)battpercent=100;

    Serial.print(F("Batt Voltage: ")); Serial.print(cellVoltage, 3); Serial.println(" V");
    Serial.print(F("Batt Percent: ")); Serial.print(battpercent); Serial.println(" %");

    value = "Vbatt:";
    value += battpercent;
    value += "%"; 
    drawText(value.c_str(), TFT_LINE2, ST77XX_BLUE); 
  }
  else{

    Serial.println("Failed to read cell voltage, check battery is connected!");
  }

 if(sensor_status()) {
    value = "Temp= ";
    value.concat(sensor_getTemp());
    value += "c";
    drawText(value.c_str(), TFT_LINE3);

    value = "Humi= ";
    value.concat(sensor_getHumidity());
    value += "%";
    drawText(value.c_str(), TFT_LINE4);
    sensor_printValues();
  }
  else{
     drawText("Sensor error", TFT_LINE3);   
  }  
} 

void setup(void) {
  Serial.begin(115200);
  delay(1000);
  // wait until serial monitor opens
  //while (!Serial) delay(10);    

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("");
  Serial.println("Hello! ESP32-S3 (Adafruit ESP32-S3 Reverse TFT Feather)");
  Serial.printf("ESP32 IDF Version %d.%d.%d\n", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
  Serial.printf("ESP32 Arduino Version %d.%d.%d\n", ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH);
  Serial.printf("CPU: %iMhz\n", getCpuFrequencyMhz());

  init_tft();
  init_BME280();
  if(init_sdcard()<0){
    drawText("SD error", TFT_LINE2, ST77XX_RED); 
  }
  while (!maxlipo.begin()) {
    Serial.println(F("Couldnt find Adafruit MAX17048?\nMake sure a battery is plugged in!"));
    delay(2000);
  }

  setCpuFrequencyMhz(80);
  Serial.printf("CPU Changed: %iMhz\n", getCpuFrequencyMhz());
  Serial.println("Initialized\n");

  // While to start user button D2
  drawText("D2 to start", TFT_LINE5, ST77XX_GREEN); 
  while(!digitalRead(BUTTON_D2)){
    displayInfos();
    delay(500);
  }

  clear_tft();
  setPower_tft(false);
}

void loop() {
  esp_sleep_enable_timer_wakeup(60e6); // us timer wake up

  setI2CPower(true);
  init_BME280();
  delay(50);

  // Store data into SDCard (datalogger)
  if(sensor_status()){
    addEntryToSdcard(sensor_getTemp(), sensor_getHumidity(), sensor_getPessure(), maxlipo.cellVoltage());
  }

  // Blink led
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);

  // Sleep
  setI2CPower(false);
  esp_light_sleep_start();
 }
