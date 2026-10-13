#ifndef SIGNAL_H
#define SIGNAL_H

#include "data.h"

float calculate_average(SensorBuffer *buffer);
float calculate_peak(SensorBuffer *buffer);
float calculate_rms(SensorBuffer *buffer);

float calculate_vibration_trend(SensorBuffer *buffer);
float calculate_temperature_trend(SensorBuffer *buffer);

#endif