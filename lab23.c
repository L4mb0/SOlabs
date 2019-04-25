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
#define INDEX 5
/////////////////////////////////////////
int filosofos[INDEX] = {0, 1, 2, 3, 4};
int palitos[INDEX] = {1, 1, 1, 1, 1};
bool c =true;
int procesosTerminados = 0;
int filoVivos=5;
int llegoHilo=0;
int tiempo = 0;
int segundos, minutos, horas, segundosRestantes;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_barrier_t barrera;
/////////////////////////////////////////
void *comer(void *idFilosofo) {

    int *id = (int *) idFilosofo;
    int misPalitos = 0;
    int misMakis = 20;
    while (true) {
        pthread_mutex_lock(&mutex);
        llegoHilo++;
        if(llegoHilo==5){
            llegoHilo=0;
            pthread_cond_broadcast(&cond);
        }
        else
            pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
        misPalitos=0;
        pthread_mutex_lock(&mutex);
        c=true;
        procesosTerminados=0;
        pthread_mutex_unlock(&mutex);
        for (int i = 0; i < INDEX; ++i) {
            if (misPalitos < 2) {
                pthread_mutex_lock(&mutex);
                if (palitos[i] == 1) {
                    palitos[i] = 0;
                    misPalitos++;
                }
                pthread_mutex_unlock(&mutex);
            }
            if (misPalitos == 2)break;
        }

        if (misPalitos == 2) {
            misMakis--;
            printf("Filosofo %d esta comiendo\n", *id);
            pthread_mutex_lock(&mutex);
            if(c ==true){
                c=false;
                tiempo+=4;
            };
            pthread_mutex_unlock(&mutex);
        }
        else {
            printf("Filosofo %d esta hablando\n", *id);
        }

        pthread_mutex_lock(&mutex);
        procesosTerminados++;
        if(procesosTerminados == filoVivos){
            for (int i = 0; i < INDEX; ++i) {
                palitos[i]=1;
            }
            printf("//\n");
        }
        pthread_mutex_unlock(&mutex);
        if (misMakis == 0)break;
    }

    printf("mate el thread %d\n\n", *id);
    pthread_mutex_lock(&mutex);
    filoVivos--;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


int main() {

    pthread_t tid[INDEX];
    pthread_barrier_init(&barrera, NULL, INDEX);
    for (int i = 0; i < INDEX; i++)
        pthread_create(&tid[i], NULL, comer, &filosofos[i]);
    sleep(1);//espera para que todos lleguen al lock
    for (int i = 0; i < INDEX; i++)
        pthread_join(tid[i], NULL);
    horas = tiempo / (60 * 60);
    segundosRestantes = tiempo - (horas * 60 * 60);
    minutos = segundosRestantes / 60;
    segundosRestantes = segundosRestantes - (minutos * 60);
    segundos = segundosRestantes;
    printf("%d segundos\n", tiempo);
    printf("%d horas // %d minutos // %d segundos", horas, minutos, segundos);
    return 0;
}
