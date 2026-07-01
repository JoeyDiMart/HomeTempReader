#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SENSOR_ID "28-000000ba3148" // declare a constant here, this is the ID for the sensor on pin 7

int main(void) {
    char path[128];
    snprintf(path, sizeof(path), "/sys/bus/w1/devices/%s/w1_slave", SENSOR_ID); // path on le potato

    FILE *fptr = fopen(path, "r");  // file to read
    if (fptr == NULL) {
        perror("Error opening sensor file");
        return 1;
    }

    char line1[128];
    char line2[128];

    if (fgets(line1, sizeof(line1), fptr) == NULL) { // if reading line for 128 chars at the file pointer = null exit
        fprintf(stderr, "Failed to read CRC line\n");
        fclose(fptr);
        return 1;
    }

    if (fgets(line2, sizeof(line2), fptr) == NULL) { // if reading line for 128 chars at the file pointer = null exit
        fprintf(stderr, "Failed to read temperature line\n");
        fclose(fptr);
        return 1;
    }

    fclose(fptr);

    if (strstr(line1, "YES") == NULL) {  // find first occurance of "YES" or else null and fail
        fprintf(stderr, "CRC check failed, discarding reading\n");
        return 1;
    }

    char *t_pos = strstr(line2, "t=");  // get the temp
    if (t_pos == NULL) {
        fprintf(stderr, "Could not find temperature value\n");
        return 1;
    }

    int temp_millideg = atoi(t_pos + 2);  // converts a string of chars to its int value
    const double temp_c = temp_millideg / 1000.0; // full-precision Celsius
    const double temp_f = floor(temp_c * 1.8 + 32);

    printf("%d\n", temp_millideg);
    printf("%.0f\n", temp_f);

    return 0;
}