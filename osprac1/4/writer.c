#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_reader = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_writer = PTHREAD_COND_INITIALIZER;
int data = 0;
int readers_count = 0;
void* writer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (readers_count > 0) {
            pthread_cond_wait(&cond_writer, &mutex);
        }
        data++;
        printf("Writer %ld is writing data: %d\n", (long)arg, data);
        sleep(1);

        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
}
int main() {
    pthread_t writer_threads[2];
    for (long i = 0; i < 2; ++i) {
        pthread_create(&writer_threads[i], NULL, writer, (void*)i);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_join(writer_threads[i], NULL);
    }
    return 0;
}

