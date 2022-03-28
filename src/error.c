#include <stdio.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

void error_no_value(char* mess){
        printf("Error: no %s value defined!\n", mess);
        PrintUsage();
}

void error_not_int(char* mess){
        printf("Error: %s value is not an integer!\n", mess);
        PrintUsage();
}

void error_over_limit(char* mess, int limit){
        printf("Error: %s value is over the limit, max value: %d!\n", mess, limit);
        PrintUsage();
}

