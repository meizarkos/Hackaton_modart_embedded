#ifndef SENSOR_H
#define SENSOR_H
#include <Arduino.h>
#include <MPU6050.h>

float readTemperature(int pin);
void cardiacTask(void * parameter);
float accelerometerMagnitude(MPU6050 mpu);

#endif
