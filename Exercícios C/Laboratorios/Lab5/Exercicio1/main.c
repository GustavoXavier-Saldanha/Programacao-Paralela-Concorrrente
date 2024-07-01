#include <stdio.h>
#include <pthread.h>

pthread_barrier_t barrier1, barrier2;

void *thread_E(void *arg) {
    pthread_barrier_wait(&barrier1);
    printf("E");
    pthread_barrier_wait(&barrier2);
    pthread_exit(NULL);
}

void *thread_R(void *arg) {
    pthread_barrier_wait(&barrier1);
    printf("R");
    pthread_barrier_wait(&barrier2);
    pthread_exit(NULL);
}

void *thread_O(void *arg) {
    pthread_barrier_wait(&barrier1);
    printf("O");
    pthread_barrier_wait(&barrier2);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid_E, tid_R, tid_O;

    pthread_barrier_init(&barrier1, NULL, 3);
    pthread_barrier_init(&barrier2, NULL, 3);

    pthread_create(&tid_E, NULL, thread_E, NULL);
    pthread_create(&tid_R, NULL, thread_R, NULL);
    pthread_create(&tid_O, NULL, thread_O, NULL);

    pthread_join(tid_E, NULL);
    pthread_join(tid_R, NULL);
    pthread_join(tid_O, NULL);

    printf("\n");

    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);

    return 0;
}
