#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semF, semE;

void *threadC(void *arg) {
    printf("C");
    sem_post(&semA);
    pthread_exit(NULL);
}

void *threadA(void *arg) {
    sem_wait(&semA);
    printf("A");
    sem_post(&semF);
    pthread_exit(NULL);
}

void *threadF(void *arg) {
    sem_wait(&semF);
    printf("F");
    sem_post(&semE);
    pthread_exit(NULL);
}

void *threadE(void *arg) {
    sem_wait(&semE);
    printf("E");
    pthread_exit(NULL);
}

int main() {
    pthread_t tidC, tidA, tidF, tidE;

    sem_init(&semA, 0, 0);
    sem_init(&semF, 0, 0);
    sem_init(&semE, 0, 0);

    pthread_create(&tidC, NULL, threadC, NULL);
    pthread_create(&tidA, NULL, threadA, NULL);
    pthread_create(&tidF, NULL, threadF, NULL);
    pthread_create(&tidE, NULL, threadE, NULL);

    pthread_join(tidC, NULL);
    pthread_join(tidA, NULL);
    pthread_join(tidF, NULL);
    pthread_join(tidE, NULL);

    printf("\n");

    sem_destroy(&semA);
    sem_destroy(&semF);
    sem_destroy(&semE);

    return 0;
}
