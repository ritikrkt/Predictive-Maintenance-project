#include <stdio.h>
#include <time.h>

#include "data.h"


void init_buffer(SensorBuffer *buffer)
{
    buffer->count = 0;
}


void add_sample(SensorBuffer *buffer, SensorData *data)
{
    if (buffer->count < MAX_SAMPLES)
    {
        buffer->samples[buffer->count] = *data;
        buffer->count++;
    }
}


void display_buffer(SensorBuffer *buffer)
{
    int i;

    printf("\nStored Samples\n");
    printf("----------------------------\n");

    for (i = 0; i < buffer->count; i++)
    {
        printf("Sample %d : Vibration = %.2f | Temperature = %.2f | Current = %.2f\n",
               i + 1,
               buffer->samples[i].vibration,
               buffer->samples[i].temperature,
               buffer->samples[i].current);
    }
}


/* Initialize machine data file */

void init_data_file(void)
{
    FILE *fp;

    /*
     * Check whether file already exists
     */

    fp = fopen("data/machine_data.csv", "r");

    if (fp == NULL)
    {
        /*
         * File does not exist.
         * Create file and write header.
         */

        fp = fopen("data/machine_data.csv", "w");

        if (fp == NULL)
        {
            printf("ERROR: Unable to create data file\n");
            return;
        }

        fprintf(fp,
                "Timestamp,Sample,Vibration,Temperature,Current\n");

        fclose(fp);

        return;
    }


    /*
     * File already exists.
     * Close it without deleting old data.
     */

    fclose(fp);
}


/* Log sensor data with timestamp */

void log_sensor_data(int sample_number,
                     SensorData *data)
{
    FILE *fp;

    time_t current_time;

    struct tm *time_info;

    char timestamp[30];


    /*
     * Get current system time
     */

    current_time = time(NULL);

    time_info = localtime(&current_time);


    /*
     * Convert time into readable format
     *
     * Example:
     * 2026-09-02 10:40:01
     */

    strftime(timestamp,
             sizeof(timestamp),
             "%Y-%m-%d %H:%M:%S",
             time_info);


    /*
     * Open CSV in append mode
     */

    fp = fopen("data/machine_data.csv", "a");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open data file\n");
        return;
    }


    /*
     * Write timestamp + sensor data
     */

    fprintf(fp,
            "%s,%d,%.2f,%.2f,%.2f\n",
            timestamp,
            sample_number,
            data->vibration,
            data->temperature,
            data->current);


    fclose(fp);
}


void generate_health_report(SensorBuffer *buffer)
{
    float total_vibration = 0.0;
    float total_temperature = 0.0;
    float total_current = 0.0;

    float max_vibration = 0.0;
    float max_temperature = 0.0;
    float max_current = 0.0;

    float avg_vibration;
    float avg_temperature;
    float avg_current;

    int i;


    if (buffer->count == 0)
    {
        printf("No data available for report.\n");
        return;
    }


    for (i = 0; i < buffer->count; i++)
    {
        total_vibration += buffer->samples[i].vibration;

        total_temperature +=
            buffer->samples[i].temperature;

        total_current +=
            buffer->samples[i].current;


        if (buffer->samples[i].vibration >
            max_vibration)
        {
            max_vibration =
                buffer->samples[i].vibration;
        }


        if (buffer->samples[i].temperature >
            max_temperature)
        {
            max_temperature =
                buffer->samples[i].temperature;
        }


        if (buffer->samples[i].current >
            max_current)
        {
            max_current =
                buffer->samples[i].current;
        }
    }


    avg_vibration =
        total_vibration / buffer->count;

    avg_temperature =
        total_temperature / buffer->count;

    avg_current =
        total_current / buffer->count;


    printf("\n");
    printf("============================================\n");
    printf("          MACHINE HEALTH REPORT\n");
    printf("============================================\n");

    printf("Total Samples       : %d\n",
           buffer->count);

    printf("--------------------------------------------\n");

    printf("Average Vibration   : %.2f mm/s\n",
           avg_vibration);

    printf("Maximum Vibration   : %.2f mm/s\n",
           max_vibration);

    printf("Average Temperature : %.2f C\n",
           avg_temperature);

    printf("Maximum Temperature : %.2f C\n",
           max_temperature);

    printf("Average Current     : %.2f A\n",
           avg_current);

    printf("Maximum Current     : %.2f A\n",
           max_current);

    printf("============================================\n");
}