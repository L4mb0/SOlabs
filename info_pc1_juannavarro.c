#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    char command[50];

    printf("\n------------------------------CPUINFO-------------------------------------------\n");

    strcpy( command, "sed '5!d' /proc/cpuinfo" );
    system(command);

    printf("\n------------------------------MEMINFO-------------------------------------------\n");

    strcpy( command, "sed '1!d' /proc/meminfo" );
    system(command);

    printf("\n------------------------------VERSION-------------------------------------------\n");

    strcpy( command, "cat /proc/version" );
    system(command);

    printf("\n------------------------------UPTIME--------------------------------------------\n");

    strcpy( command, "cat /proc/uptime" );
    system(command);

    return(0);
}
