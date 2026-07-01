//
// Created by Joseph DiMartino on 7/1/26.
//
#include <stdio.h>
#include <string.h>

int main() {
    char FILE_PATH[45] = "/sys/bus/w1/devices/";
    char FILE_NAME[] = "28-000000ba3148/w1_slave";
    FILE *fptr;

    fptr = fopen(strcat(FILE_PATH, FILE_NAME), "r");

    if (fptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    int ch;
    char temp[3];
    while ((ch = fgetc(fptr)) != EOF) {
        if (putchar(ch) == 'c') {
            if (putchar(ch+1) == 'r' && putchar(ch+2) == 'c') {
                if (putchar(ch+7) == 'N') {
                    perror("CRC not confirmed");
                }
            }
        }
        else {
            putchar(ch);
        }
    }
    printf("%c", ch);

    fclose(fptr);
    return 0;
}
