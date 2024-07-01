#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM_ARR 10

const int arr[TAM_ARR] = {5, 7, 7, 8, 8, 5, 1, 7, 7777777, 0};
int count = 0;
int num;
int num_threads;
pthread_mutex_t lock;

void *count_occurrences(void *arg) {
    int thread_id = *(int *)arg;
    int start = (TAM_ARR / num_threads) * thread_id;
    int end = (TAM_ARR / num_threads) * (thread_id + 1);
    if (thread_id == num_threads - 1) {
        end = TAM_ARR;
    }

    for (int i = start; i < end; i++) {
        if (arr[i] == num) {
            pthread_mutex_lock(&lock);
            count++;
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    num_threads = atoi(argv[1]);

    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    printf("Numero de threads utilizadas no sistema: %d\n\n", num_threads);
    printf("Numero a ser contabilizado: ");
    scanf("%d", &num);

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, count_occurrences, (void *)&thread_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("O numero %d ocorre %d vezes no array.\n", num, count);

    return 0;
}
