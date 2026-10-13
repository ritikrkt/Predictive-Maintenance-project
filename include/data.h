#ifndef DATA_H
#define DATA_H

#include "sensor.h"

#define MAX_SAMPLES 100

typedef struct
{
    SensorData samples[MAX_SAMPLES];
    int count;
} SensorBuffer;

void init_buffer(SensorBuffer *buffer);
void add_sample(SensorBuffer *buffer, SensorData *data);
void display_buffer(SensorBuffer *buffer);

void init_data_file(void);
void log_sensor_data(int sample_number, SensorData *data);

void generate_health_report(SensorBuffer *buffer);

#endif