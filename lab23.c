#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
int palitosFilosofos[INDEX]={0,0,0,0,0};
int palitos = 5;
int makis[INDEX] = {20, 20, 20, 20, 20};
int sum = 0;
int tiempo = 0;
int segundos, minutos, horas, segundosRestantes;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int cc = 0;

/////////////////////////////////////////

void *comer(void *idFilosofo) {
    while (true) {
        int *id = (int *) idFilosofo;
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);


        if (palitos > 1) {
                palitos--;
                palitosFilosofos[*id]++;
                if(palitos>1) {
                    palitos--;
                    palitosFilosofos[*id]++;
                    if (palitosFilosofos[*id] == 2) {
                        makis[*id]--;
                        printf("Filosofo %d esta comiendo\n", *id);
                    }
                }
        }
        else {
            printf("Filosofo %d esta hablando\n", *id);
        }

        cc++;
        if (makis[*id] == 0)break;
    }
    pthread_exit(NULL);
}


int main() {

    pthread_t tid[INDEX];


    for (int i = 0; i < INDEX; i++)
        pthread_create(&tid[i], NULL, comer, &filosofos[i]);

    sleep(1);//espera para que todos lleguen al lock
    while(true) {
        pthread_cond_broadcast(&cond); //GO

        sleep(0);

        palitos=5;
        for (int j = 0; j < 5; ++j) {
            palitosFilosofos[j]=0;
        }
        printf("\n");
        for (int i = 0; i < 5; ++i) {
            sum += makis[i];
        }
        if (sum==0)break;
        sum = 0;
        tiempo += 4;
    }



    for (int i = 0; i < INDEX; i++)
        pthread_join(tid[i], NULL);


    horas=tiempo/(60*60);
    segundosRestantes = tiempo-(horas*60*60);
    minutos=segundosRestantes/60;
    segundosRestantes=segundosRestantes-(minutos*60);
    segundos=segundosRestantes;

    printf("%d segundos\n", tiempo);
    printf("%d horas // %d minutos // %d segundos", horas, minutos, segundos);

    return 0;
}
