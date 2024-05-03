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

    // Prompt the user to enter a message
    printf("Enter the message to be passed to the client: ");
    fgets(message, sizeof(message), stdin);

    // Create a shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Create a semaphore
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize the semaphore
    arg.val = 0;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *)(-1)) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Write the user-entered message to the shared memory segment
    strncpy(shmaddr, message, SHM_SIZE - 1);
    shmaddr[SHM_SIZE - 1] = '\0';  // Ensure null-termination

    printf("Message written to shared memory by the server.\n");

    // Signal the semaphore to notify the client
    signal_semaphore(semid);

    // Detach the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}

