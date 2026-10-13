#include <math.h>
#include "data.h"
#include "signal.h"

float calculate_average(SensorBuffer *buffer)
{
    float sum = 0.0;
    int i;

    if (buffer->count == 0)
    {
        return 0.0;
    }

    for (i = 0; i < buffer->count; i++)
    {
        sum = sum + buffer->samples[i].vibration;
    }

    return sum / buffer->count;
}

float calculate_peak(SensorBuffer *buffer)
{
    float peak;
    int i;

    if (buffer->count == 0)
    {
        return 0.0;
    }

    peak = buffer->samples[0].vibration;

    for (i = 1; i < buffer->count; i++)
    {
        if (buffer->samples[i].vibration > peak)
        {
            peak = buffer->samples[i].vibration;
        }
    }

    return peak;
}

float calculate_rms(SensorBuffer *buffer)
{
    float sum = 0.0;
    int i;

    if (buffer->count == 0)
    {
        return 0.0;
    }

    for (i = 0; i < buffer->count; i++)
    {
        sum = sum + (buffer->samples[i].vibration *
                     buffer->samples[i].vibration);
    }

    return sqrt(sum / buffer->count);
}

float calculate_vibration_trend(SensorBuffer *buffer)
{
    float first_average = 0.0;
    float last_average = 0.0;

    int i;
    int window = 10;

    if (buffer->count < 20)
    {
        return 0.0;
    }

    for (i = 0; i < window; i++)
    {
        first_average += buffer->samples[i].vibration;
    }

    first_average = first_average / window;

    for (i = buffer->count - window; i < buffer->count; i++)
    {
        last_average += buffer->samples[i].vibration;
    }

    last_average = last_average / window;

    return last_average - first_average;
}

float calculate_temperature_trend(SensorBuffer *buffer)
{
    float first_average = 0.0;
    float last_average = 0.0;

    int i;
    int window = 10;

    if (buffer->count < 20)
    {
        return 0.0;
    }

    for (i = 0; i < window; i++)
    {
        first_average += buffer->samples[i].temperature;
    }

    first_average = first_average / window;

    for (i = buffer->count - window; i < buffer->count; i++)
    {
        last_average += buffer->samples[i].temperature;
    }

    last_average = last_average / window;

    return last_average - first_average;
}