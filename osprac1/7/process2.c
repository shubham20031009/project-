#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 100

int main() {
    char buffer[MAX_BUFFER_SIZE];
    int charCount = 0, wordCount = 0, lineCount = 0;

    while (1) {
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        if (feof(stdin)) {
            break;
        }

        // Count characters, words, and lines
        for (int i = 0; buffer[i] != '\0'; i++) {
            charCount++;

            // Increment word count when a space or newline is encountered
            if (buffer[i] == ' ' || buffer[i] == '\n') {
                wordCount++;
            }

            // Increment line count when a newline is encountered
            if (buffer[i] == '\n') {
                lineCount++;
            }
        }
    }

    // Write analysis results to a text file
    FILE *file = fopen("analysis.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Character count: %d\n", charCount);
    fprintf(file, "Word count: %d\n", wordCount);
    fprintf(file, "Line count: %d\n", lineCount);

    fclose(file);

    // Read the contents of the file and write to the pipe for process1
    file = fopen("analysis.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, MAX_BUFFER_SIZE, file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);

    return 0;
}

