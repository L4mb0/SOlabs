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


/////////////////////////////////////////

int m1f, m1c, m2f, m2c, sum = 0;
int **m1, **m2, **m3;

struct timeval start, end;
long double usedCPU;
int counter = 0;
int threadss = 4;


/////////////////////////////////////////

void *multiplicar(void *arg) {

    long id = (long) arg;
    long filasThread = m1f / threadss;
    long indexInicio = id * filasThread;
    long indexFin = (id + 1) * filasThread;

    for (long i = indexInicio; i < indexFin; i++) {
        for (int j = 0; j < m2c; j++) {
            for (int k = 0; k < m2f; k++) {
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

}

/////////////////////////////////////////

void printMatrix() {
    printf("\nla matriz 1 es...\n");

    for (int k = 0; k < m1f; ++k) {
        for (int i = 0; i < m1c; ++i) {
            printf("%d\t", m1[k][i]);
        }
        printf("\n");
    }

    printf("\nla matriz 2 es...\n");

    for (int k = 0; k < m2f; ++k) {
        for (int i = 0; i < m2c; ++i) {
            printf("%d\t", m1[k][i]);
        }
        printf("\n");
    }

    printf("\nproducto final es...\n");

    for (int i = 0; i < m1f; i++) {
        for (int j = 0; j < m2c; j++)
            printf("%d\t", m3[i][j]);

        printf("\n");
    }
}

/////////////////////////////////////////

int main() {

    srand(time(NULL));
    pthread_t tid[threadss];

    printf("columnas y filas de m1? >>\n");
    scanf("%d%d", &m1f, &m1c);


    printf("columnas y filas de m2? >>\n");
    scanf("%d%d", &m2f, &m2c);


    if (m1c != m2f) {
        printf("no se pueden multiplicar estas matrices ya que tienen diferentes dimensiones, intenta denuevo\n");
        exit(EXIT_FAILURE);
    }

    /**************************************/

    //malloc para filas

    m1 = (int **) malloc(sizeof(int *) * m1f);

    m2 = (int **) malloc(sizeof(int *) * m2f);

    m3 = (int **) malloc(sizeof(int *) * m1f);

    //malloc para columnas

    for (int i = 0; i < m1f; i++) {
        m1[i] = (int *) malloc(sizeof(int) * m1c);
    }

    for (int i = 0; i < m2f; i++) {
        m2[i] = (int *) malloc(sizeof(int) * m2c);
    }

    for (int i = 0; i < m1f; i++) {
        m3[i] = (int *) malloc(sizeof(int) * m2c);
    }

    /**************************************/

    //version manual inputs

    /*printf("elementos de m1? (de 0 a 5) >>\n");

    for (int i = 0; i < m1f; i++)
        for (int j = 0; j < m1c; j++)
            scanf("%d", &m1[i][j]);

    printf("elementos de m2? (de 0 a 5) >>\n");

    for (int i = 0; i < m2f; i++)
        for (int j = 0; j < m2c; j++)
            scanf("%d", &m2[i][j]);*/

    //version random inputs

    for (int i = 0; i < m1f; i++)
        for (int j = 0; j < m1c; j++)
            m1[i][j] = rand() % 6;

    for (int i = 0; i < m2f; i++)
        for (int j = 0; j < m2c; j++)
            m2[i][j] = rand() % 6;

    /**************************************/

    gettimeofday(&start, NULL);

    //thread join and create
    for (int i = 0; i < threadss; i++)
        pthread_create(&tid[i], NULL, multiplicar, (void *) i);

    //Unión de threads
    for (int i = 0; i < threadss; i++)
        pthread_join(tid[i], NULL);

    gettimeofday(&end, NULL);

    /**************************************/



    //printMatrix();

    free(m1);
    free(m2);
    free(m3);


    usedCPU = ((end.tv_sec - start.tv_sec) * 1000000);
    usedCPU += (end.tv_usec-start.tv_usec);
    usedCPU /= 1000000;
    printf("\ntiempo en realizar la operacion es...\n%Le\n", usedCPU);


    return 0;
}
