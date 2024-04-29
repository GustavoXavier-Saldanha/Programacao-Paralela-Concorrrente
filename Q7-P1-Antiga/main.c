#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 4

char string[] = "aoaaaoaaobobodfdhnsosfgndfgofdgmdofbsfgnbo";
char letter = 'o';

typedef struct {
    char* segment_start;
    int segment_size;
    char letter;
    int result;
} ThreadData;

void* count_occurrences(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int count = 0;
    for (int i = 0; i < data->segment_size; i++) {
        if (data->segment_start[i] == data->letter) {
            count++;
        }
    }
    data->result = count;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    int string_length = strlen(string);
    int segment_size = string_length / NUM_THREADS;
    int segment_remainder = string_length % NUM_THREADS;
    int index = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].segment_start = &string[index];
        thread_data[i].segment_size = segment_size + (i < segment_remainder ? 1 : 0);
        thread_data[i].letter = letter;
        pthread_create(&threads[i], NULL, count_occurrences, (void*)&thread_data[i]);
        index += thread_data[i].segment_size;
    }

    int total_occurrences = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_occurrences += thread_data[i].result;
    }

    printf("Total de ocorrencias da letra '%c': %d\n", letter, total_occurrences);

    return 0;
}
