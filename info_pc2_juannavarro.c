#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int sleep();

int main () {
    char command[50];
    bool counter=true;

    while(true) {
        //memoria disponible
        strcpy(command, "free | grep Mem | awk '{print $4/$2 * 100.0}'");
        system(command);

        ///////////////////////////////////////////////////////////

        //tiempo en user
        strcpy(command, "cat /proc/stat | grep -w 'cpu' | awk '{print ($2/($2+$3+$4+$5+$6+$7+$8)) * 100.0}'");
        system(command);

        //tiempo en system
        strcpy(command, "cat /proc/stat | grep -w 'cpu' | awk '{print ($4/($2+$3+$4+$5+$6+$7+$8)) * 100.0}'");
        system(command);

        //tiempo en idle
        strcpy(command, "cat /proc/stat | grep -w 'cpu' | awk '{print ($5/($2+$3+$4+$5+$6+$7+$8)) * 100.0}'");
        system(command);

        ///////////////////////////////////////////////////////////

        //tiempo en idle
        if (counter == true) {
            strcpy(command, "cat /proc/stat | grep -w 'ctxt' | awk '{print $2}'");
            long x = system(command);
        }


        break;
        sleep(1);

    }

}
