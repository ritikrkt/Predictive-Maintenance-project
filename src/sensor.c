#include <stdio.h>
#include <stdlib.h>

#include "sensor.h"

void read_sensor_data(SensorData *data, SensorMode mode)
{
    static int sample_number = 0;

    sample_number++;

    switch (mode)
    {
        case SENSOR_NORMAL:

            data->vibration = 1.0 + (rand() % 200) / 100.0;
            data->temperature = 40.0 + (rand() % 150) / 10.0;
            data->current = 2.0 + (rand() % 250) / 100.0;

            break;


        case SENSOR_BEARING_FAULT:

            /*
             * Vibration gradually increases with time.
             */

            data->vibration =
                1.5 + (sample_number * 0.025)
                + (rand() % 50) / 100.0;

            data->temperature =
                45.0 + (sample_number * 0.05)
                + (rand() % 50) / 100.0;

            data->current =
                2.5 + (sample_number * 0.01)
                + (rand() % 50) / 100.0;

            break;


        case SENSOR_OVERHEATING:

            /*
             * Temperature gradually increases.
             */

            data->vibration =
                1.5 + (rand() % 100) / 100.0;

            data->temperature =
                45.0 + (sample_number * 0.30)
                + (rand() % 50) / 100.0;

            data->current =
                3.5 + (sample_number * 0.03)
                + (rand() % 50) / 100.0;

            break;


        default:

            data->vibration = 0;
            data->temperature = 0;
            data->current = 0;

            break;
    }
}


void display_sensor_data(SensorData *data)
{
    printf("\nMachine Sensor Data\n");
    printf("--------------------\n");

    printf("Vibration   : %.2f mm/s\n", data->vibration);
    printf("Temperature : %.2f C\n", data->temperature);
    printf("Current     : %.2f A\n", data->current);
}