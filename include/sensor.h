#ifndef SENSOR_H
#define SENSOR_H

typedef struct
{
    float vibration;
    float temperature;
    float current;
} SensorData;

typedef enum
{
    SENSOR_NORMAL,
    SENSOR_BEARING_FAULT,
    SENSOR_OVERHEATING
} SensorMode;

void read_sensor_data(SensorData *data, SensorMode mode);
void display_sensor_data(SensorData *data);

#endif