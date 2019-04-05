#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int isdigit();


void MEMDISPONIBLE(){
    int x, y;

    char breakset[] = "0123456789";
    char word[]="MemFree";
    char word2[]="MemTotal";
    char file[]="/proc/meminfo";

    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("Error al abrir file.\n");
        exit(EXIT_FAILURE);
    }

    char line2[100];

    while(fgets(line2, sizeof(line2), fp) != NULL){
        if (strstr(line2 , word2 )!= NULL)
        {
            y = atoi(strpbrk(line2, breakset));
            break;
        }
    }

    char line[100];

    while(fgets(line, sizeof(line), fp) != NULL){
        if (strstr(line , word )!= NULL)
        {
            x = atoi(strpbrk(line, breakset));
            break;
        }
    }

    double porcentaje = ((double)x/(double)y)*100;
    printf("PORCENTAJE DE MEMORIA DISPONIBLE: \n");
    printf("%f\n", porcentaje);



    fclose(fp);

}

void TIEMPOS(){
    int arr[10];
    int sumaTotal=0;

    char word[]="cpu";
    char file[]="/proc/stat";

    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("Error al abrir file.\n");
        exit(EXIT_FAILURE);
    }


    char line[100];

    while(fgets(line, sizeof(line), fp) != NULL){
        if (strstr(line , word )!= NULL)
        {
            char *token = strtok(line, " ");
            int i = 0;
            while (token != NULL && i < 10) {
                if (sscanf(token, "%d", &arr[i++]) != 1)
                    ; // TODO: ERROR: numero no recuperado
                token = strtok(NULL, " ");
            }
            break;
        }
    }

    for (int j = 0; j < 10; ++j) {
        sumaTotal += arr[j];
    }
    
    double porcentaje = ((double)arr[1]/(double)sumaTotal)*100.00;

    printf("PORCENTAJE DE TIEMPO EN USER MODE: \n");
    printf("%f\n", porcentaje);

    double porcentaje2 = ((double)arr[3]/(double)sumaTotal)*100.00;

    printf("PORCENTAJE DE TIEMPO EN SYSTEM MODE: \n");
    printf("%f\n", porcentaje2);

    double porcentaje3 = ((double)arr[4]/(double)sumaTotal)*100.00;

    printf("PORCENTAJE DE TIEMPO EN IDLE MODE: \n");
    printf("%f\n", porcentaje3);

    fclose(fp);

}

int TASAKERNEL(){
    int arr[2];

    char word[]="ctxt";
    char file[]="/proc/stat";

    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("Error al abrir file.\n");
        exit(EXIT_FAILURE);
    }



    char line[100];

    while(fgets(line, sizeof(line), fp) != NULL){
        if (strstr(line , word )!= NULL)
        {
            char *token = strtok(line, " ");
            int i = 0;
            while (token != NULL && i < 2) {
                if (sscanf(token, "%d", &arr[i++]) != 1)
                    ; // TODO: ERROR: numero no recuperado
                token = strtok(NULL, " ");
            }
            break;
        }
    }

    fclose(fp);
    return arr[1];


}

int TASAPROCESOS(){
    int arr[2];

    char word[]="processes";
    char file[]="/proc/stat";

    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("Error al abrir file.\n");
        exit(EXIT_FAILURE);
    }



    char line[100];

    while(fgets(line, sizeof(line), fp) != NULL){
        if (strstr(line , word )!= NULL)
        {
            char *token = strtok(line, " ");
            int i = 0;
            while (token != NULL && i < 2) {
                if (sscanf(token, "%d", &arr[i++]) != 1)
                    ; // TODO: ERROR: numero no recuperado
                token = strtok(NULL, " ");
            }
            break;
        }
    }

    return arr[1];
    fclose(fp);

}



int sleep();

int main () {
    char command[50];
    int counter = 0;
    int x, y;


    while(true) {


        MEMDISPONIBLE();
        //memoria disponible
        /*strcpy(command, "free | grep Mem | awk '{print $4/$2 * 100.0}'");
        system(command);*/

        ///////////////////////////////////////////////////////////

        TIEMPOS();

        /*
        //tiempo en user
        strcpy(command, "cat /proc/stat | grep -w 'cpu' | awk '{print ($2/($2+$3+$4+$5+$6+$7+$8)) * 100.0}'");
        system(command);

        //tiempo en system
        strcpy(command, "cat /proc/stat | grep -w 'cpu' | awk '{print ($4/($2+$3+$4+$5+$6+$7+$8)) * 100.0}'");
        system(command);

        //tiempo en idle
        strcpy(command, "cat /proc/stat | grep -w 'cpu' | awk '{print ($5/($2+$3+$4+$5+$6+$7+$8)) * 100.0}'");
        system(command);

        */

        ///////////////////////////////////////////////////////////

        //printf("%d\n",TASAKERNEL());


        if (counter==0){
            x = TASAKERNEL();
            counter++;
        }
        else {
            y = TASAKERNEL();
            counter--;
        }

        double tasa = (((double)y-(double)x)/(double)x)*100.0;
        printf("TASA DE CAMBIO DEL KERNEL: \n%f\n", tasa);

        ///////////////////////////////////////////////////////////

        if (counter==0){
            x = TASAKERNEL();
            counter++;
        }
        else {
            y = TASAKERNEL();
            counter--;
        }

        double tasa2 = (((double)y-(double)x)/(double)x)*100.0;
        printf("TASA DE CAMBIO DE LOS PROCESOS: \n%f\n", tasa2);


        printf("\n");

        sleep(1);

    }

}
