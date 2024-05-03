#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// Global variables
int available[MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

int processes, resources;

// Function prototypes
void input();
int requestResources(int process, int request[]);
int isSafe();

// Function definitions
void input() {
    printf("Enter the number of processes: ");
    scanf("%d", &processes);

    printf("Enter the number of resources: ");
    scanf("%d", &resources);

    printf("Enter the available resources: ");
    for (int i = 0; i < resources; i++) {
        scanf("%d", &available[i]);
    }

    printf("Enter the maximum demand of each process:\n");
    for (int i = 0; i < processes; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < resources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the allocation of resources to each process:\n");
    for (int i = 0; i < processes; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < resources; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

int requestResources(int process, int request[]) {
    // ... (unchanged)
}

int isSafe() {
    // ... (unchanged)
    return 1;  // The system is safe
}

int main() {
    input();

    int process;
    printf("Enter the process to request resources for: ");
    scanf("%d", &process);

    int request[MAX_RESOURCES];
    printf("Enter the resource request for process %d: ", process);
    for (int i = 0; i < resources; i++) {
        scanf("%d", &request[i]);
    }

    if (requestResources(process, request)) {
        printf("Request granted. System in safe state.\n");
    } else {
        printf("Request denied. System in unsafe state.\n");
    }

    return 0;
}

