
#ifndef SENSOR_BME280_H
#define SENSOR_BME280_H
#pragma once

#define SEALEVELPRESSURE_HPA 1011 // Marseille France 30/01/25

void init_BME280();
void sensor_printValues();

bool sensor_status();
float sensor_getTemp();
float sensor_getHumidity();
float sensor_getPessure();

#endif // SENSOR_BME280_H
