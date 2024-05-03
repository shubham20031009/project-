#include <stdio.h>
#include <stdlib.h>
void displayReverseArray(int arr[], int size) {
    for (int i = size - 1; i >= 0; i--) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
int main(int argc, char *argv[]) {
    int size = argc - 1;
    int arr[size];
    for (int i = 0; i < size; i++) {
        arr[i] = atoi(argv[i + 1]);
    }
    printf("Reversed Array (Child): ");
    displayReverseArray(arr, size);
    return 0;
}

