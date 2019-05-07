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

#define ITERACIONES 100
#define TT 8
//int TT=0;
int BARRERAS=0;
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
        x++;
        while(tas(&variable)==1);
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
        tatas_acquire(&variable);
        x++;
        count++;
        tatas_release(&variable);
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
pthread_barrier_t barrera;

void *usandoPthreadBarrier(void *arg){
    long id=(long)arg;

    for (int i = 0; i < BARRERAS; ++i) {
        pthread_barrier_wait(&barrera);
    }

    printf("barrera %ld salio\n", id);
    pthread_exit(NULL);

}

/////////////////////////////////////////

void centralizedBarrier(int id){

    static volatile unsigned long count = 0;
    static volatile unsigned int sense = 0;
    static volatile unsigned int local_sense = 0;

    local_sense = !local_sense;
    if (fai(&count) == TT-1) {
        count = 0;
        sense = local_sense;
    } else {
        while (sense != local_sense);
    }
}

void *usandoBCentral(void* arg){
    long id=(long)arg;

    for (int i = 0; i < BARRERAS; ++i) {
        centralizedBarrier((int) id);
    }

    printf("barrera %ld salio\n", id);
    pthread_exit(NULL);


}

/////////////////////////////////////////

void reverseSense(int id) {
    static volatile unsigned long count = 0;
    static volatile unsigned int sense = 0;
    static volatile unsigned int threads_sense[TT] = {0};

    threads_sense[id] = !threads_sense[id];
    if (fai(&count) == TT - 1) {
        count = 0;
        sense = !sense;
    } else {
        while (sense != threads_sense[id]);
    }
}

void *reverseSenseBarrier(void* arg){
    long id=(long)arg;

    for (int i = 0; i < BARRERAS; ++i) {
        reverseSense((int) id);
    }

    printf("barrera %ld salio\n", id);
    pthread_exit(NULL);

}

/////////////////////////////////////////

int main() {
    int a=0;
    double sumTotal=0;

    pthread_t tid[TT];
    char casos;
    
    printf("NOTA: CAMBIA #define TT PARA JUGAR CON EL NUMERO DE THREADS\n\n");

    //printf("ingrese el numero de threads a usar (NOTA: SI USAS CENTRALIZED BARRIER O RSB NECESITAS CAMBIAR EL DEFINE TT EN LA SECCION DE VARIABLES GLOBALES)\n>>");
    //scanf("%d", &TT);

    printf("\ningrese la letra de la funcion que quiere probar(ej: a ): \n"
           "a) sin sincronismo\n"
           "b) pthread lock\n"
           "c) tas\n"
           "d) tastas\n"
           "e) tastas con backoff\n"
           "f) ticket\n"
           "g) pthread barrier\n"
           "h) barrera central clasica\n"
           "i) reverse sense barrier\n"
           ">> ");
    scanf(" %c", &casos);
    printf("\n//////////////////////////////////////////////////////////\n\n");

    switch(casos){
        default:
            printf("\nopcion no valida, por favor reinicie el programa\n");
            return 1;
        case 'a':
            printf("sin sincronismo: \n\n");

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, contarSinSincronismo, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }

            printf("\ncount -> %d", count);


            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;


        case 'b':

            printf("pthread lock: \n");


            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, contarPthreadLock, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }

            printf("\ncount -> %d", count);


            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;


        case 'c':
            printf("TAS lock: \n");

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, contarTAS, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }

            printf("\ncount -> %d", count);


            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;

        case 'd':
            printf("TATAS lock: \n");

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, contarTATAS, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }

            printf("\ncount -> %d", count);


            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;

        case 'e':
            printf("TATAS con backoff lock: \n");

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, contarTATASBackoff, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }

            printf("\ncount -> %d", count);


            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;

        case 'f':
            printf("Ticket lock: \n");

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, contarTicket, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }

            printf("\ncount -> %d", count);


            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;

        case 'g':
            printf("pthread barrier: \n"
                   "ingrese el numero de veces que quieres llegar a la barrera:\n>> ");
            pthread_barrier_init(&barrera, NULL, TT);

            scanf("%d", &BARRERAS);

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, usandoPthreadBarrier, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }



            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;

        case 'h':
            printf("central barrier: \n"
                   "ingrese el numero de veces que quieres llegar a la barrera:\n>> ");

            scanf("%d", &BARRERAS);

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, usandoBCentral, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }



            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;

        case 'i':
            printf("reverse sense barrier: \n"
                   "ingrese el numero de veces que quieres llegar a la barrera:\n>> ");

            scanf("%d", &BARRERAS);

            while(a!=ITERACIONES) {

                start = gethrtime_x86();

                for (long i = 0; i < TT; i++)
                    pthread_create(&tid[i], NULL, reverseSenseBarrier, (void *) i);


                for (long i = 0; i < TT; i++)
                    pthread_join(tid[i], NULL);


                end = gethrtime_x86();

                sumTotal += (end-start);
                a++;

            }



            totalTime = sumTotal/a;
            printf("\ntiempo PROMEDIO en realizar %d operaciones es %Le\n",  a,totalTime);


            return 0;
    }




}
