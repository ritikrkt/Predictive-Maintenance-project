#include <stdio.h>

#include "fault.h"
#include "signal.h"

MachineStatus check_machine_status(SensorBuffer *buffer)
{
    float rms;
    float peak;
    float temperature;
    float current;

    if (buffer->count == 0)
    {
        return NORMAL;
    }

    rms = calculate_rms(buffer);
    peak = calculate_peak(buffer);

    temperature = buffer->samples[buffer->count - 1].temperature;
    current = buffer->samples[buffer->count - 1].current;

    if (rms > 3.5 ||
        peak > 3.8 ||
        temperature > 70.0 ||
        current > 5.5)
    {
        return CRITICAL;
    }

    if (rms > 2.5 ||
        peak > 3.0 ||
        temperature > 60.0 ||
        current > 4.5)
    {
        return WARNING;
    }

    return NORMAL;
}


FaultType detect_fault(SensorBuffer *buffer)
{
    float rms;
    float peak;
    float temperature;
    float current;

    if (buffer->count == 0)
    {
        return NO_FAULT;
    }

    rms = calculate_rms(buffer);
    peak = calculate_peak(buffer);

    temperature = buffer->samples[buffer->count - 1].temperature;
    current = buffer->samples[buffer->count - 1].current;


    /* Bearing fault */

    if (rms > 3.0 || peak > 3.5)
    {
        return BEARING_FAULT;
    }


    /* Overheating */

    if (temperature > 70.0)
    {
        return OVERHEATING_FAULT;
    }


    /* Motor overload */

    if (current > 5.5)
    {
        return OVERLOAD_FAULT;
    }


    return NO_FAULT;
}


void display_machine_status(MachineStatus status)
{
    printf("\nMachine Health Status\n");
    printf("----------------------------\n");

    switch (status)
    {
        case NORMAL:
            printf("Status : NORMAL\n");
            break;

        case WARNING:
            printf("Status : WARNING\n");
            break;

        case CRITICAL:
            printf("Status : CRITICAL\n");
            break;

        default:
            printf("Status : UNKNOWN\n");
    }
}


void display_fault(FaultType fault)
{
    printf("\nFault Detection\n");
    printf("----------------------------\n");

    switch (fault)
    {
        case NO_FAULT:
            printf("Fault : NO FAULT\n");
            break;

        case BEARING_FAULT:
            printf("Fault : BEARING FAULT\n");
            break;

        case OVERHEATING_FAULT:
            printf("Fault : OVERHEATING\n");
            break;

        case OVERLOAD_FAULT:
            printf("Fault : MOTOR OVERLOAD\n");
            break;

        default:
            printf("Fault : UNKNOWN\n");
    }
}
void init_fault_log(void)
{
    FILE *fp;

    fp = fopen("logs/fault_log.csv", "w");

    if (fp == NULL)
    {
        printf("ERROR: Unable to create fault log\n");
        return;
    }

    fprintf(fp,
            "Sample,Vibration,RMS,Temperature,Current,Status,Fault\n");

    fclose(fp);
}


void log_fault_event(int sample_number,
                     SensorData *data,
                     float rms,
                     MachineStatus status,
                     FaultType fault)
{
    FILE *fp;

    fp = fopen("logs/fault_log.csv", "a");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open fault log\n");
        return;
    }

    fprintf(fp,
            "%d,%.2f,%.2f,%.2f,%.2f,",
            sample_number,
            data->vibration,
            rms,
            data->temperature,
            data->current);

    switch (status)
    {
        case NORMAL:
            fprintf(fp, "NORMAL,");
            break;

        case WARNING:
            fprintf(fp, "WARNING,");
            break;

        case CRITICAL:
            fprintf(fp, "CRITICAL,");
            break;

        default:
            fprintf(fp, "UNKNOWN,");
    }

    switch (fault)
    {
        case NO_FAULT:
            fprintf(fp, "NO_FAULT\n");
            break;

        case BEARING_FAULT:
            fprintf(fp, "BEARING_FAULT\n");
            break;

        case OVERHEATING_FAULT:
            fprintf(fp, "OVERHEATING\n");
            break;

        case OVERLOAD_FAULT:
            fprintf(fp, "OVERLOAD\n");
            break;

        default:
            fprintf(fp, "UNKNOWN\n");
    }

    fclose(fp);
}
void log_fault_start(int sample_number,
                     SensorData *data,
                     float rms,
                     MachineStatus status,
                     FaultType fault)
{
    FILE *fp;

    fp = fopen("logs/fault_log.csv", "a");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open fault log\n");
        return;
    }

    fprintf(fp,
            "%d,%.2f,%.2f,%.2f,%.2f,",
            sample_number,
            data->vibration,
            rms,
            data->temperature,
            data->current);

    switch (status)
    {
        case WARNING:
            fprintf(fp, "WARNING,");
            break;

        case CRITICAL:
            fprintf(fp, "CRITICAL,");
            break;

        default:
            fprintf(fp, "UNKNOWN,");
    }

    switch (fault)
    {
        case BEARING_FAULT:
            fprintf(fp, "BEARING_FAULT_DETECTED\n");
            break;

        case OVERHEATING_FAULT:
            fprintf(fp, "OVERHEATING_DETECTED\n");
            break;

        case OVERLOAD_FAULT:
            fprintf(fp, "OVERLOAD_DETECTED\n");
            break;

        default:
            fprintf(fp, "WARNING_DETECTED\n");
    }

    fclose(fp);
}


void log_fault_clear(int sample_number)
{
    FILE *fp;

    fp = fopen("logs/fault_log.csv", "a");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open fault log\n");
        return;
    }

    fprintf(fp,
            "%d,0.00,0.00,0.00,0.00,NORMAL,FAULT_CLEARED\n",
            sample_number);

    fclose(fp);
}