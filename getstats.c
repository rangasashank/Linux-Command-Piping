/* getstats.c

 *
 * - If run without an argument, dumps information about the PC to STDOUT.
 *
 * - If run with a process number created by the current user,
 *   dumps information about that process to STDOUT.
 *
 * Author: Ranga Sashank Gurram
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 300
/*
 * Note: You are permitted, and even encouraged, to add other
 * support functions in order to reduce duplication of code, or
 * to increase the clarity of your solution, or both.
 */

void print_process_info(char *process_num)
{

    char process_status_file[50] = "/proc/";
    char process_cmdline_file[50] = "/proc/";
    strcat(process_status_file, process_num);
    strcat(process_cmdline_file, process_num);
    strcat(process_status_file, "/status");
    strcat(process_cmdline_file, "/cmdline");

    FILE *fp;
    fp = fopen(process_status_file, "r");
    if (!fp)
    {
        printf("Process number %s not found\n", process_num);
    }
    else
    {
        printf("Process number: %s\n", process_num);
        char buffer[SIZE];
        int voluntary;
        int non_voluntary;
        char threads[50] = "Threads:";
        while (fgets(buffer, sizeof(char) * SIZE, fp) != NULL)
        {
            char *token = strtok(buffer, ":");
            if (strcmp(token, "Name") == 0)
            {
                token = strtok(NULL, ":");
                printf("%s:%s", buffer, token);
            }
            if (strcmp(token, "Threads") == 0)
            {
                token = strtok(NULL, ":");
                strcat(threads, token);
            }
            if (strcmp(token, "voluntary_ctxt_switches") == 0)
            {
                token = strtok(NULL, ":");
                voluntary = atoi(token);
            }
            if (strcmp(token, "nonvoluntary_ctxt_switches") == 0)
            {
                token = strtok(NULL, ":");
                non_voluntary = atoi(token);
            }
        }
        fp = fopen(process_cmdline_file, "r");
        while (fgets(buffer, sizeof(char) * SIZE, fp) != NULL)
        {
            char *token = strtok(buffer, "-");
            printf("Filename (if any): %s\n", token);
            break;
        }

        printf("%s", threads);
        int total = voluntary + non_voluntary;
        printf("Total context switches: %d\n", total);
        fclose(fp);
    }
}

void convert(char token[SIZE])
{
    int total_seconds = atoi(token);

    int days = total_seconds / (24 * 3600);
    char days_final[50] = " ";
    sprintf(days_final, "%d", days);
    strcat(days_final, " days, ");

    total_seconds = total_seconds % (24 * 3600);
    int hours = total_seconds / 3600;
    char hours_final[50] = " ";
    sprintf(hours_final, "%d", hours);
    strcat(hours_final, " hours, ");

    total_seconds %= 3600;
    int minutes = total_seconds / 60;
    char minutes_final[50] = " ";
    sprintf(minutes_final, "%d", minutes);
    strcat(minutes_final, " minutes, ");

    total_seconds %= 60;
    int seconds = total_seconds;
    char seconds_final[50] = " ";
    sprintf(seconds_final, "%d", seconds);
    strcat(seconds_final, " seconds");

    // logic of getting days, hours, minutes and seconds from seconds taken from     https://www.geeksforgeeks.org/converting-seconds-into-days-hours-minutes-and-seconds/

    char final_uptime[SIZE] = "";
    strcat(final_uptime, days_final);
    strcat(final_uptime, hours_final);
    strcat(final_uptime, minutes_final);
    strcat(final_uptime, seconds_final);

    printf("Uptime: %s\n", final_uptime);
}
void print_full_info()
{
    char *cpu_info_file = "/proc/cpuinfo";
    FILE *fp = fopen(cpu_info_file, "r");
    char buffer[SIZE];
    int count = 0;

    while (fgets(buffer, sizeof(char) * SIZE, fp) != NULL)
    {
        if (count == 4)
        {
            printf("%s", buffer);
        }
        if (count == 12)
        {
            printf("%s", buffer);
            count = 0;
            break;
        }
        count++;
    }
    fclose(fp);

    char *version_file = "/proc/version";
    fp = fopen(version_file, "r");
    char version[SIZE];
    while (fgets(version, sizeof(char) * SIZE, fp) != NULL)
    {
        version[strcspn(version, "\r\n")] = 0;
    }
    fclose(fp);
    printf("%s\n", version);

    char *memory_file = "/proc/meminfo";
    fp = fopen(memory_file, "r");
    char memTotal[SIZE];

    while (fgets(memTotal, sizeof(char) * SIZE, fp) != NULL)
    {

        memTotal[strcspn(memTotal, "\r\n")] = 0;
        break;
    }
    fclose(fp);
    printf("%s\n", memTotal);

    char *uptime_file = "/proc/uptime";
    fp = fopen(uptime_file, "r");
    char uptime[SIZE];

    while (fgets(uptime, sizeof(char) * SIZE, fp) != NULL)
    {

        uptime[strcspn(uptime, "\r\n")] = 0;
        break;
    }

    fclose(fp);
    char *token = strtok(uptime, " ");
    convert(token);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        print_full_info();
    }
    else
    {
        print_process_info(argv[1]);
    }
}
