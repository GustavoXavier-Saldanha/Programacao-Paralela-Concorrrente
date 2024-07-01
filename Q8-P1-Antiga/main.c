#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_E = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_R = PTHREAD_COND_INITIALIZER;

void* print_E(void* arg) {
    pthread_mutex_lock(&mutex);
    printf("E");
    pthread_cond_signal(&cond_R);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* print_R(void* arg) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond_R, &mutex);
    printf("R");
    pthread_cond_signal(&cond_E);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* print_O(void* arg) {
    printf("O");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread_E, thread_R, thread_O;

    pthread_create(&thread_E, NULL, print_E, NULL);
    pthread_create(&thread_R, NULL, print_R, NULL);
    pthread_create(&thread_O, NULL, print_O, NULL);

    pthread_join(thread_E, NULL);
    pthread_join(thread_R, NULL);
    pthread_join(thread_O, NULL);

    printf("\n");

    return 0;
}
