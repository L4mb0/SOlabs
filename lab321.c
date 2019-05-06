#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sched.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

#include "atomic_ops.h"
#include "hrtimer_x86.c"

/////////////////////////////////////////


int THREADS=0;
int count = 0;

volatile unsigned long variable = 0;
long double start, end, totalTime;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/////////////////////////////////////////

void *contarSinSincronismo(void *arg){
    long id = (long)arg;
    int x=0;

    while(true){
        x++;
        count++;
        if (x>=50000) {
            printf("hilo %ld salio\n", id);
            pthread_exit(NULL);}
    }

}

/////////////////////////////////////////

void *contarPthreadLock(void *arg){
    long id = (long)arg;
    int x=0;

    while(true){
        x++;
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
        if (x>=50000) {
            printf("hilo %ld salio\n", id);
            pthread_exit(NULL);}
    }

}

/////////////////////////////////////////

void *contarTAS(void *arg){
    long id = (long)arg;
    int x=0;

    while(true){
        while(tas(&variable)==1);
        x++;
        count++;
        variable=0;
        if (x>=50000) {
            printf("hilo %ld salio\n", id);
            pthread_exit(NULL);}
    }

}

/////////////////////////////////////////


void *contarTATAS(void *arg){
    long id = (long)arg;
    int x=0;

    while(true){
        btatas_acquire(&variable);
        x++;
        count++;
        btatas_release(&variable);
        if (x>=50000) {
            printf("hilo %ld salio\n", id);
            pthread_exit(NULL);}
    }

}

/////////////////////////////////////////



void *contarTATASBackoff(void *arg){
    long id = (long)arg;
    int x=0;
    int time=1;

    while(true){
        btatas_acquire(&variable);
        x++;
        count++;
        btatas_release(&variable);
        if (x>=50000) {
            printf("hilo %ld salio\n", id);
            pthread_exit(NULL);}
    }

}

/////////////////////////////////////////

ticket_lock_t Ticket;

void *contarTicket(void *arg){
    long id = (long)arg;
    int x=0;

    while(true){
        x++;
        ticket_acquire(&Ticket);
        count++;
        ticket_release(&Ticket);
        if (x>=50000) {
            printf("hilo %ld salio\n", id);
            pthread_exit(NULL);}
    }

}

/////////////////////////////////////////

int main() {

    pthread_t tid[THREADS];
    char casos;

    printf("ingrese el numero de threads a usar\n>>");
    scanf("%d", &THREADS);

    printf("\ningrese la letra de la funcion que quiere probar(ej: a ): \n"
           "a) sin sincronismo\n"
           "b) pthread lock\n"
           "c) tas\n"
           "d) tastas\n"
           "e) tastas con backoff\n"
           "f) ticket\n"
           ">> ");
    scanf(" %c", &casos);
    printf("\n//////////////////////////////////////////////////////////\n\n");

    switch(casos){
        default:
            printf("\nopcion no valida, por favor reinicie el programa\n");
            return 1;
        case 'a':
            printf("sin sincronismo: \n\n");


            start = gethrtime_x86();

            for (long i = 0; i < THREADS; i++)
                pthread_create(&tid[i], NULL, contarSinSincronismo, (void*)i);


            for (long i = 0; i < THREADS; i++)
                pthread_join(tid[i], NULL);


            end = gethrtime_x86();

            printf("\ncount -> %d", count);

            totalTime = end -start;
            printf("\ntiempo en realizar la operacion es %Le\n", totalTime);


            return 0;


        case 'b':

            printf("pthread lock: \n");

            start = gethrtime_x86();

            for (long i = 0; i < THREADS; i++)
                pthread_create(&tid[i], NULL, contarPthreadLock, (void*)i);


            for (long i = 0; i < THREADS; i++)
                pthread_join(tid[i], NULL);


            end = gethrtime_x86();

            printf("\ncount -> %d", count);

            totalTime = end-start;
            printf("\ntiempo en realizar la operacion es %Le\n", totalTime);


            return 0;


        case 'c':
            printf("TAS lock: \n");

            start = gethrtime_x86();

            for (long i = 0; i < THREADS; i++)
                pthread_create(&tid[i], NULL, contarTAS, (void*)i);


            for (long i = 0; i < THREADS; i++)
                pthread_join(tid[i], NULL);


            end = gethrtime_x86();

            printf("\ncount -> %d", count);

            totalTime = end-start;
            printf("\ntiempo en realizar la operacion es %Le\n", totalTime);


            return 0;

        case 'd':
            printf("TATAS lock: \n");

            start = gethrtime_x86();

            for (long i = 0; i < THREADS; i++)
                pthread_create(&tid[i], NULL, contarTATAS, (void*)i);


            for (long i = 0; i < THREADS; i++)
                pthread_join(tid[i], NULL);


            end = gethrtime_x86();

            printf("\ncount -> %d", count);

            totalTime = end-start;
            printf("\ntiempo en realizar la operacion es %Le\n", totalTime);


            return 0;

        case 'e':
            printf("TATAS con backoff lock: \n");

            start = gethrtime_x86();

            for (long i = 0; i < THREADS; i++)
                pthread_create(&tid[i], NULL, contarTATASBackoff, (void*)i);


            for (long i = 0; i < THREADS; i++)
                pthread_join(tid[i], NULL);


            end = gethrtime_x86();

            printf("\ncount -> %d", count);

            totalTime = end-start;
            printf("\ntiempo en realizar la operacion es %Le\n", totalTime);


            return 0;

        case 'f':
            printf("Ticket lock: \n");

            start = gethrtime_x86();

            for (long i = 0; i < THREADS; i++)
                pthread_create(&tid[i], NULL, contarTicket, (void*)i);


            for (long i = 0; i < THREADS; i++)
                pthread_join(tid[i], NULL);


            end = gethrtime_x86();

            printf("\ncount -> %d", count);

            totalTime = end-start;
            printf("\ntiempo en realizar la operacion es %Le\n", totalTime);


            return 0;
    }




}
