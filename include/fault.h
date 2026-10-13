#ifndef FAULT_H
#define FAULT_H

#include "data.h"

typedef enum
{
    NORMAL,
    WARNING,
    CRITICAL
} MachineStatus;

typedef enum
{
    NO_FAULT,
    BEARING_FAULT,
    OVERHEATING_FAULT,
    OVERLOAD_FAULT
} FaultType;

MachineStatus check_machine_status(SensorBuffer *buffer);

FaultType detect_fault(SensorBuffer *buffer);

void display_machine_status(MachineStatus status);

void display_fault(FaultType fault);

void init_fault_log(void);

void log_fault_event(int sample_number,
                     SensorData *data,
                     float rms,
                     MachineStatus status,
                     FaultType fault);

void log_fault_start(int sample_number,
                     SensorData *data,
                     float rms,
                     MachineStatus status,
                     FaultType fault);

void log_fault_clear(int sample_number);

#endif