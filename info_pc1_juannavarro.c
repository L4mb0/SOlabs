#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void CPUINFO(){
    int n=0;


    char word[]="model name";
    char file[]="/proc/cpuinfo";

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
            printf("%s",line);
            break;
        }
    }

    fclose(fp);


}

void VERSION(){
    char ch;
    char file[]="/proc/version";

    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("Error al abrir file.\n");
        exit(EXIT_FAILURE);
    }

    while((ch = fgetc(fp)) != EOF)
        printf("%c", ch);


    fclose(fp);


}

void MEMINFO(){
    int n=0;


    char word[]="MemTotal";
    char file[]="/proc/meminfo";

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
            printf("%s",line);
            break;
        }
    }

    fclose(fp);


}

void UPTIME(){
    char ch;
    char file[]="/proc/uptime";

    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("Error al abrir file.\n");
        exit(EXIT_FAILURE);
    }

    while((ch = fgetc(fp)) != EOF)
        printf("%c", ch);


    fclose(fp);


}


int main () {
    char command[50];

    printf("\n------------------------------CPUINFO-------------------------------------------\n");

    /*strcpy( command, "sed '5!d' /proc/cpuinfo" );
    system(command);*/
    CPUINFO();

    printf("\n------------------------------MEMINFO-------------------------------------------\n");

    /*strcpy( command, "sed '1!d' /proc/meminfo" );
    system(command);*/
    MEMINFO();

    printf("\n------------------------------VERSION-------------------------------------------\n");

    /*strcpy( command, "cat /proc/version" );
    system(command);*/
    VERSION();

    printf("\n------------------------------UPTIME--------------------------------------------\n");

    /*strcpy( command, "cat /proc/uptime" );
    system(command);*/
    UPTIME();

    return(0);
}

