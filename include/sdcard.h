
#ifndef SDCARD_H
#define SDCARD_H
#pragma once

#include <RTClib.h>

//@return int 
//  0 ok
//  -1 rtc error
//  -2 sdcard error
int init_sdcard();

//@return int 
//  0 ok
//  -1 write error
int addEntryToSdcard(const float& temperature=0.0, const float& humidity=0.0,const float& pressure=0.0, const float& vbatt=0.0);

DateTime sd_getDateTime();


#endif // SDCARD_H
