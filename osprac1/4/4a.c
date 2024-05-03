#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

// Shared data structures
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// Semaphores and Mutex
sem_t empty, full;
pthread_mutex_t mutex;

// Producer function
void *producer(void *arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        item = rand() % 100; // Produce a random item

        sem_wait(&empty);    // Wait for an empty slot in the buffer
        pthread_mutex_lock(&mutex); // Acquire the mutex

        // Produce item and add to buffer
        printf("Producing item %d\n", item);
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Release the mutex
        sem_post(&full);     // Signal that buffer is no longer empty
    }

    pthread_exit(NULL);
}

// Consumer function
void *consumer(void *arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        sem_wait(&full);    // Wait for a full slot in the buffer
        pthread_mutex_lock(&mutex); // Acquire the mutex

        // Consume item from buffer
        item = buffer[out];
        printf("Consuming item %d\n", item);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Release the mutex
        sem_post(&empty);    // Signal that buffer is no longer full
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

