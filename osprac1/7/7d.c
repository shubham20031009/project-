#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE 1024

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void wait_semaphore(int sem_id) {
    struct sembuf buf = {0, -1, SEM_UNDO};
    semop(sem_id, &buf, 1);
}

void signal_semaphore(int sem_id) {
    struct sembuf buf = {0, 1, SEM_UNDO};
    semop(sem_id, &buf, 1);
}

int main() {
    int shmid, semid;
    char *shmaddr;
    union semun arg;
    char message[SHM_SIZE];

    // Locate the existing shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Locate the existing semaphore
    semid = semget(SEM_KEY, 1, 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *)(-1)) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Wait for the semaphore to be signaled by the server
    wait_semaphore(semid);

    // Read the message from the shared memory segment
    printf("Message received from shared memory: %s\n", shmaddr);

    // Detach the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}

