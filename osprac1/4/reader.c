#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_reader = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_writer = PTHREAD_COND_INITIALIZER;
int data = 0;
int readers_count = 0;
void* reader(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        readers_count++;
        if (readers_count == 1) {
            pthread_cond_signal(&cond_writer);
        }
        pthread_mutex_unlock(&mutex);
        printf("Reader %ld is reading data: %d\n", (long)arg, data);
        sleep(1);
        pthread_mutex_lock(&mutex);
        readers_count--;
        if (readers_count == 0) {
            pthread_cond_signal(&cond_writer);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}
int main() {
    pthread_t reader_threads[3];

    for (long i = 0; i < 3; ++i) {
        pthread_create(&reader_threads[i], NULL, reader, (void*)i);
    }
    for (int i = 0; i < 3; ++i) {
        pthread_join(reader_threads[i], NULL);
    }
    return 0;
}

