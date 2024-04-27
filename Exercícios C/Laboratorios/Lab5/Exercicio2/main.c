#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

sem_t sem[NUM_THREADS];

void* thread(void* arg) {
    int thread_id = *((int*) arg);

    sem_wait(&sem[thread_id]);
    printf("A");
    sem_post(&sem[(thread_id + 1) % NUM_THREADS]);


    sem_wait(&sem[thread_id]);
    printf("B");
    sem_post(&sem[(thread_id + 1) % NUM_THREADS]);


    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        sem_init(&sem[i], 0, 0);
    }
    sem_post(&sem[0]);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        sem_destroy(&sem[i]);
    }

    printf("\n");

    return 0;
}
