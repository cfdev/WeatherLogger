/**
Copyright 2025 Cyril Frausti <cfdev>
This file is part of mWeatherLogger with Adafruit ESP32-S3 Reverse TFT Feather board.

mWeatherLogger is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See licence.txt (MIT License).

www.cyrilfrausti.com
*/

#include "sensor_bme280.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C
bool sensor_bme_status = false;

void init_BME280() {
  if (!bme.begin()) {  
    sensor_bme_status = false;
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
  }
  else{
    sensor_bme_status = true;
    bme.setTemperatureCompensation(-0.5); // Force calibration
    sensor_printValues();
  }
}

bool sensor_status(){
  return sensor_bme_status;
}

float sensor_getTemp() {
  return bme.readTemperature();
}

float sensor_getHumidity() {
  return bme.readHumidity() + 2.5; // Calibration : +2.5
}

float sensor_getPessure() {
  return (bme.readPressure() / 100.0F) + 50.0; // Calibration : +50
}

void sensor_printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");

    Serial.print("Pressure = ");

    Serial.print(sensor_getPessure());
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
