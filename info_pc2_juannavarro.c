#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int sleep();

int main () {
    char command[50];


    while(true) {

        strcpy(command, "free | grep Memoria | awk '{print $4/$2 * 100.0}'");
        system(command);

    sleep(1);

    }

}
