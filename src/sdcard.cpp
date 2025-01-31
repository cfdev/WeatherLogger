/**
Copyright 2025 Cyril Frausti <cfdev>
This file is part of mWeatherLogger with Adafruit ESP32-S3 Reverse TFT Feather board.

mWeatherLogger is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See licence.txt (MIT License).

www.cyrilfrausti.com
*/

#include "sdcard.h"

#include <string>
#include <SdFat.h>

bool sdcard_status = false;
RTC_PCF8523 rtc;

// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 1
/*
  Change the value of SD_CS_PIN if you are using SPI and
  your hardware does not use the default value, SS.
  Common values are:
  Arduino Ethernet shield: pin 4
  Sparkfun SD shield: pin 8
  Adafruit SD shields and modules: pin 10
*/

// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = 10; // < Adafruit ESP32-S3 Reverse TFT Feather
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif  // SDCARD_SS_PIN

// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
#define SPI_CLOCK SD_SCK_MHZ(25)

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

#if SD_FAT_TYPE == 0
SdFat sd;
File file;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
File32 file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile file;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

char line[100];

//@return int 
//  0 ok
//  -1 rtc error
//  -2 sdcard error
int init_sdcard() {

    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
    }  
    else if (! rtc.initialized()) {
        Serial.println("RTC is NOT running!");
        rtc.adjust(DateTime(2025, 01, 21, 15, 0, 0));
        return -1;
    }

    // see if the card is present and can be initialized:
    if (!sd.begin(SD_CONFIG)) {
        sd.initErrorPrint(&Serial);
        return -2;
    }
    else{
        Serial.println("Card init. OK!");
        // Remove any existing file.
        if (sd.exists("mWeatherLogger.csv")) {
            if(sd.remove("mWeatherLogger.csv")){
                Serial.println("mWeatherLogger file remove with success");
            };
        }
          // Create the file.
        if (!file.open("mWeatherLogger.csv", FILE_WRITE)) {
            Serial.println("open mWeatherLogger file failed");
            return -2;
        }
        else{
            file.print("DateTime,Temp,Humidity,Pression,Vbatt\r\n");
            file.rewind();
            if(file.close()) {
                Serial.println("mWeatherLogger file done with success");
            }
            else{
                Serial.println("Something was wrong with mWeatherLogger file...");
                return -2;
            }
        }
    }
    return 0;
}

//@return int 
//  0 ok
//  -1 write error
int addEntryToSdcard(const float& temperature, const float& humidity, const float& pressure, const float& vbatt) {
    // Create the file.
    if (!file.open("mWeatherLogger.csv", FILE_WRITE)) {
        Serial.println("addEntryToSdcard::open mWeatherLogger file failed");
        return -1;
    }
    else{
        file.seekEnd(); //set end of file
        String value="";
        value = sd_getDateTime().year();
        value += "/";  
        value += sd_getDateTime().month();   
        value += "/";  
        value += sd_getDateTime().day();   
        value += " ";  
        value += sd_getDateTime().hour();
        value += ":";
        value += sd_getDateTime().minute();
        value += ":";
        value += sd_getDateTime().second();
        value += ",";
        value.concat(temperature);
        value += ",";
        value.concat(humidity);
        value += ",";
        value.concat(pressure);
        value += ",";
        value.concat(vbatt);
        file.print(value + "\r\n");
        if(!file.close()) {
            Serial.println("addEntryToSdcard::Something was wrong with test file...");
            return -1;
        }
        else{
            Serial.println("addEntryToSdcard:" + value);
        }
    }
    delay(250);
    return 0;
}

DateTime sd_getDateTime() {
    return rtc.now();
}

