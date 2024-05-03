#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 100

int main() {
    int pipe1[2], pipe2[2];
    char buffer[MAX_BUFFER_SIZE];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {  // Parent process (process1)
        close(pipe1[0]);  // Close reading end of pipe1
        close(pipe2[1]);  // Close writing end of pipe2

        printf("Enter sentences (type 'exit' to stop):\n");

        while (1) {
            fgets(buffer, MAX_BUFFER_SIZE, stdin);

            if (strcmp(buffer, "exit\n") == 0) {
                break;
            }

            write(pipe1[1], buffer, strlen(buffer) + 1);

            // Read and display the analysis results from process2
            read(pipe2[0], buffer, MAX_BUFFER_SIZE);
            printf("Analysis results:\n%s", buffer);
        }

        close(pipe1[1]);  // Close writing end of pipe1
        close(pipe2[0]);  // Close reading end of pipe2
    } else {  // Child process (process2)
        close(pipe1[1]);  // Close writing end of pipe1
        close(pipe2[0]);  // Close reading end of pipe2

        dup2(pipe1[0], STDIN_FILENO);
       close(pipe1[0]); 
       
       dup2(pipe2[1], STDIN_FILENO);
       close(pipe2[1]);

        execlp("./process2", "process2", NULL);

        perror("Exec failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

