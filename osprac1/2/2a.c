#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // swap arr[j] and arr[j+1]
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;

    printf("Enter the number of integers: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        sleep(2); // Simulating some work
        printf("\nChild process completed. Exiting.\n");
    } else if (pid > 0) {
        // Parent process
        printf("\nParent process sorting the integers...\n");
        bubbleSort(arr, n);

        // Wait for the child process
        wait(NULL);

        printf("Sorted integers: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\nParent process completed.\n");
    } else {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    return 0;
}
