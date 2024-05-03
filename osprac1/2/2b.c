#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void displayArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int size;

    printf("Enter the size of the array: ");
    scanf("%d", &size);

    int arr[size];

    printf("Enter %d numbers for the array:\n", size);
    for (int i = 0; i < size; i++) {
        printf("Number %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) 
    {  
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j] > arr[j + 1]) 
                {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        wait(NULL);

        printf("Sorted Array (Parent): ");
        displayArray(arr, size);
    } else { 
        char *args[size + 2];  

        args[0] = "./2c";

        for (int i = 0; i < size; i++) {
            args[i + 1] = malloc(10); 
            sprintf(args[i + 1], "%d", arr[i]);
        }

        args[size + 1] = NULL;

        if (execve(args[0], args, NULL) == -1) {
            perror("Execve failed");
            for (int i = 1; i <= size; i++) {
                free(args[i]);
            }
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

