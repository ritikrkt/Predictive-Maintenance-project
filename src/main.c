#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "sensor.h"
#include "data.h"
#include "signal.h"
#include "fault.h"

int main()
{
    SensorData data;
    SensorBuffer buffer;

    MachineStatus status;
    FaultType fault;
    SensorMode mode;

    float vibration_trend;
    float temperature_trend;

    int fault_active = 0;

    srand(time(NULL));

    mode = SENSOR_BEARING_FAULT;

    init_data_file();
    init_fault_log();

    printf("====================================\n");
    printf(" Predictive Maintenance System\n");
    printf("====================================\n");

    printf("Starting continuous machine monitoring...\n\n");


    /* Continuous monitoring loop */

    while (1)
    {
        /* Start a new monitoring batch */

        init_buffer(&buffer);

        printf("\n====================================\n");
        printf("Starting new monitoring cycle...\n");
        printf("====================================\n");


        /* Collect 100 samples */

        while (buffer.count < MAX_SAMPLES)
        {
            read_sensor_data(&data, mode);

            display_sensor_data(&data);

            add_sample(&buffer, &data);

            log_sensor_data(buffer.count, &data);


            /* Fault detection after 10 samples */

            if (buffer.count >= 10)
            {
                status = check_machine_status(&buffer);

                fault = detect_fault(&buffer);


                if (status != NORMAL || fault != NO_FAULT)
                {
                    if (fault_active == 0)
                    {
                        log_fault_start(
                            buffer.count,
                            &data,
                            calculate_rms(&buffer),
                            status,
                            fault
                        );

                        printf("\n*** FAULT DETECTED ***\n");

                        display_machine_status(status);

                        display_fault(fault);

                        fault_active = 1;
                    }
                }
                else
                {
                    if (fault_active == 1)
                    {
                        log_fault_clear(buffer.count);

                        printf("\n*** FAULT CLEARED ***\n");

                        fault_active = 0;
                    }
                }
            }


            /* 1 second delay */

            Sleep(1000);
        }


        /* 100 samples completed */

        printf("\n====================================\n");
        printf("100 samples collected successfully!\n");
        printf("====================================\n");


        /* Signal analysis */

        printf("\nVibration Analysis\n");
        printf("----------------------------\n");

        printf("Average Vibration : %.2f mm/s\n",
               calculate_average(&buffer));

        printf("Peak Vibration    : %.2f mm/s\n",
               calculate_peak(&buffer));

        printf("RMS Vibration     : %.2f mm/s\n",
               calculate_rms(&buffer));


        vibration_trend =
            calculate_vibration_trend(&buffer);

        temperature_trend =
            calculate_temperature_trend(&buffer);


        printf("Vibration Trend   : %.2f mm/s\n",
               vibration_trend);

        printf("Temperature Trend : %.2f C\n",
               temperature_trend);


        /* Final status */

        status = check_machine_status(&buffer);

        display_machine_status(status);


        fault = detect_fault(&buffer);

        display_fault(fault);


        /* Generate health report */

        generate_health_report(&buffer);


        /*
         * Next monitoring cycle starts automatically.
         */

        printf("\nRestarting monitoring in 5 seconds...\n");

        Sleep(5000);

        fault_active = 0;
    }


    return 0;
}