#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define MAX_READS 10
#define MAX_WRITES 5

pthread_mutex_t mutex;
pthread_mutex_t resource_mutex;
int readers_count = 0; // Number of active readers

// Reader function
void *reader(void *arg) {
    int reader_id = *((int *)arg);

    while (1) {
        // Entry section
        pthread_mutex_lock(&mutex);
        readers_count++;
        if (readers_count == 1) {
            // First reader, lock the resource for writers
            pthread_mutex_lock(&resource_mutex);
        }
        pthread_mutex_unlock(&mutex);

        // Reading section
        printf("Reader %d is reading.\n", reader_id);
        // Simulating reading by sleeping for a short duration
        usleep(rand() % 1000000);

        // Exit section
        pthread_mutex_lock(&mutex);
        readers_count--;
        if (readers_count == 0) {
            // Last reader, unlock the resource for writers
            pthread_mutex_unlock(&resource_mutex);
        }
        pthread_mutex_unlock(&mutex);

        // Simulating some delay between successive readings
        usleep(rand() % 1000000);
    }

    pthread_exit(NULL);
}

// Writer function
void *writer(void *arg) {
    int writer_id = *((int *)arg);

    while (1) {
        // Writing section
        pthread_mutex_lock(&resource_mutex);
        printf("Writer %d is writing.\n", writer_id);
        // Simulating writing by sleeping for a short duration
        usleep(rand() % 1000000);
        pthread_mutex_unlock(&resource_mutex);

        // Simulating some delay between successive writings
        usleep(rand() % 1000000);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t reader_threads[NUM_READERS];
    pthread_t writer_threads[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];

    // Initialize mutexes
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&resource_mutex, NULL);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&reader_threads[i], NULL, reader, (void *)&reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writer_threads[i], NULL, writer, (void *)&writer_ids[i]);
    }

    // Wait for threads to finish (Note: This program runs indefinitely and needs to be manually terminated)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writer_threads[i], NULL);
    }

    // Destroy mutexes
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&resource_mutex);

    return 0;
}
