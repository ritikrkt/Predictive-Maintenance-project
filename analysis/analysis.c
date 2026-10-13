#include <stdio.h>

typedef struct
{
    char timestamp[30];

    int sample;

    float vibration;

    float temperature;

    float current;

} MachineData;


/* Calculate Machine Health Score */

float calculate_health_score(float avg_vibration,
                             float max_vibration,
                             float avg_temperature,
                             float max_temperature,
                             float avg_current,
                             float max_current)
{
    float score = 100.0;


    /* Vibration penalty */

    if (avg_vibration > 2.0)
    {
        score -= 15;
    }

    if (avg_vibration > 3.0)
    {
        score -= 15;
    }

    if (max_vibration > 3.5)
    {
        score -= 10;
    }


    /* Temperature penalty */

    if (avg_temperature > 55.0)
    {
        score -= 15;
    }

    if (max_temperature > 70.0)
    {
        score -= 15;
    }


    /* Current penalty */

    if (avg_current > 4.5)
    {
        score -= 10;
    }

    if (max_current > 5.5)
    {
        score -= 10;
    }


    if (score < 0)
    {
        score = 0;
    }


    return score;
}


int main()
{
    FILE *fp;

    MachineData data;

    int count = 0;


    float total_vibration = 0.0;

    float total_temperature = 0.0;

    float total_current = 0.0;


    float max_vibration = 0.0;

    float max_temperature = 0.0;

    float max_current = 0.0;


    float avg_vibration;

    float avg_temperature;

    float avg_current;

    float health_score;


    /* Open CSV */

    fp = fopen("data/machine_data.csv", "r");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open machine_data.csv\n");

        return 1;
    }


    /* Skip header */

    fscanf(fp, "%*[^\n]\n");



    char line[200];

/* Read CSV line by line */

while (fgets(line, sizeof(line), fp) != NULL)
{
    if (sscanf(line,
               "%29[^,],%d,%f,%f,%f",
               data.timestamp,
               &data.sample,
               &data.vibration,
               &data.temperature,
               &data.current) == 5)
    {
        total_vibration += data.vibration;

        total_temperature += data.temperature;

        total_current += data.current;


        if (data.vibration > max_vibration)
        {
            max_vibration = data.vibration;
        }


        if (data.temperature > max_temperature)
        {
            max_temperature = data.temperature;
        }


        if (data.current > max_current)
        {
            max_current = data.current;
        }


        count++;
    }
}


    fclose(fp);


    if (count == 0)
    {
        printf("No sensor data available.\n");

        return 0;
    }


    /* Calculate averages */

    avg_vibration =
        total_vibration / count;

    avg_temperature =
        total_temperature / count;

    avg_current =
        total_current / count;


    /* Calculate Health Score */

    health_score =
        calculate_health_score(
            avg_vibration,
            max_vibration,
            avg_temperature,
            max_temperature,
            avg_current,
            max_current
        );


    /* Display Analysis */

    printf("\n");
    printf("============================================\n");
    printf("       MACHINE HISTORICAL DATA ANALYSIS\n");
    printf("============================================\n");


    printf("Samples Analysed   : %d\n",
           count);


    printf("--------------------------------------------\n");


    printf("Average Vibration  : %.2f mm/s\n",
           avg_vibration);


    printf("Maximum Vibration  : %.2f mm/s\n",
           max_vibration);


    printf("Average Temperature: %.2f C\n",
           avg_temperature);


    printf("Maximum Temperature: %.2f C\n",
           max_temperature);


    printf("Average Current    : %.2f A\n",
           avg_current);


    printf("Maximum Current    : %.2f A\n",
           max_current);


    printf("--------------------------------------------\n");


    printf("Machine Health Score : %.0f / 100\n",
           health_score);


    if (health_score >= 80)
    {
        printf("Machine Condition    : HEALTHY\n");
    }
    else if (health_score >= 60)
    {
        printf("Machine Condition    : WARNING\n");
    }
    else if (health_score >= 40)
    {
        printf("Machine Condition    : POOR\n");
    }
    else
    {
        printf("Machine Condition    : CRITICAL\n");
    }


    printf("============================================\n");


    return 0;
}