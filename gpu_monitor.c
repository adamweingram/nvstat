#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "nvml.h"

int main(int argc, char *argv[]) {
    nvmlReturn_t result;
    nvmlDevice_t device;
    nvmlUtilization_t utilization;
    unsigned int powerUsage;
    bool interactive = false;
    int interval_ms = 500;
    char *output_file = NULL;
    
    // Parse command line arguments
    // -i: interactive mode
    // -t: interval in ms
    // -o: output file (CSV)
    // -h: print help
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'i') {
                interactive = true;
            }
            else if (argv[i][1] == 't') {
                if (i + 1 < argc) {
                    interval_ms = atoi(argv[i + 1]);
                    i++;
                }
            }
            else if (argv[i][1] == 'o') {
                if (i + 1 < argc) {
                    output_file = argv[i + 1];
                    i++;
                }
            }
            else if (argv[i][1] == 'h') {
                printf("Usage: %s [-i] [-t interval (ms)] [-o output_file] [-h]\n", argv[0]);
                return EXIT_SUCCESS;
            }
            else {
                fprintf(stderr, "[ERROR] Unknown flag: %s\n", argv[i]);
                printf("Usage: %s [-i] [-t interval (ms)] [-h]\n", argv[0]);
                return EXIT_FAILURE;
            }
        }
    }

    // Initialize NVML
    result = nvmlInit();
    if (NVML_SUCCESS != result) {
        fprintf(stderr, "[ERROR] Failed to initialize NVML: %s\n", nvmlErrorString(result));
        return EXIT_FAILURE;
    }

    // Get handle for device 0 (modify if you have multiple GPUs)
    result = nvmlDeviceGetHandleByIndex(0, &device);
    if (NVML_SUCCESS != result) {
        fprintf(stderr, "[ERROR] Failed to get handle for device 0: %s\n", nvmlErrorString(result));
        nvmlShutdown();
        return EXIT_FAILURE;
    }
    
    // If in standard (non-interactive) mode, print table header
    if (!interactive)
    {
        printf("________________________________\n");
        printf("| SM Utilization | Power Usage |\n");
        printf("|      (%%)       |     (W)     |\n");
        printf("|----------------+-------------|\n");
    }

    // Open output file if specified
    FILE *file = NULL;
    if (output_file) {
        file = fopen(output_file, "w");

        // If file is empty, write header
        fseek(file, 0, SEEK_END);
        if (ftell(file) == 0) {
            fprintf(file, "Timestamp,SM Utilization,Power Usage\n");
        }
    }

    // Main loop: update on interval
    while (1) {
        result = nvmlDeviceGetUtilizationRates(device, &utilization);
        if (NVML_SUCCESS != result) {
            fprintf(stderr, "[ERROR] Failed to get utilization rates: %s\n", nvmlErrorString(result));
            break;
        }

        result = nvmlDeviceGetPowerUsage(device, &powerUsage);
        if (NVML_SUCCESS != result) {
            fprintf(stderr, "[ERROR] Failed to get power usage: %s\n", nvmlErrorString(result));
            break;
        }

        // Handle interactive mode
        if (interactive) {
            // Clear the screen (ANSI escape sequence) and print header + stats.
            printf("\033[H\033[J");
           
            // Print header
            printf("GPU Monitoring Tool\n");
            printf("-------------------\n");
            
            // `utilization.gpu` represents the percentage time the GPU (i.e. SMs) was busy.
            printf("SM Utilization: %u%%\n", utilization.gpu);
            
            // `powerUsage` is returned in milliwatts, which we convert to watts for readability.
            printf("Power Usage   : %.2f W\n", powerUsage / 1000.0);
        } else {
            //      | SM Utilization | Power Usage |
            printf("| %14d | %11.2f |\n", utilization.gpu, powerUsage / 1000.0);

            // Write to output file if specified
            if (file) {
                // Get current time and format it as a string
                time_t now = time(NULL);
                char *timestamp = ctime(&now);
                timestamp[strcspn(timestamp, "\n")] = '\0'; // Remove newline character

                // Write to file
                fprintf(file, "%s,%d,%.2f\n", timestamp, utilization.gpu, powerUsage / 1000.0);
                fflush(file);
                fsync(fileno(file));
            }
        }

        usleep(interval_ms * 1000);
    }

    // Close output file if specified
    if (file) {
        fclose(file);
    }

    // Shutdown NVML
    nvmlShutdown();
    return EXIT_SUCCESS;
}
