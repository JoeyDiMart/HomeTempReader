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
    printf("%s\n", fptr);

    return 0;
}
