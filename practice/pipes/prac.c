#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 100

// Function to compare integers for qsort
int compare_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0 || n % 2 != 0) {
        fprintf(stderr, "n must be a positive even number.\n");
        return 1;
    }

    int u[MAX_SIZE];

    // Generate random integers
    for (int i = 0; i < n; i++) {
        u[i] = rand() % 10; // Change this to match your requirements
    }

    int sorted[MAX_SIZE];
    int n1 = n / 2;
    int n2 = n / 2;

    // Create pipes
    int pipe1[2];
    int pipe2[2];

    /*read end of pipe - array[0]
      write end of pipe - array[1]*/

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    // Create two child processes
    pid_t child1, child2;
    child1 = fork();

    if (child1 < 0) {
        perror("fork");
        return 1;
    }

    if (child1 == 0) {
        // Child process 1
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        qsort(u, n1, sizeof(int), compare_int);
        write(pipe1[1], u, n1 * sizeof(int));
        close(pipe1[1]);

        exit(0);
    } else {
        child2 = fork();

        if (child2 < 0) {
            perror("fork");
            return 1;
        }

        if (child2 == 0) {
            // Child process 2
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);

            qsort(u + n1, n2, sizeof(int), compare_int);
            write(pipe2[1], u + n1, n2 * sizeof(int));
            close(pipe2[1]);

            exit(0);
        } else {
            // Parent process
            close(pipe1[1]);
            close(pipe2[1]);

            int status1, status2;
            waitpid(child1, &status1, 0);
            waitpid(child2, &status2, 0);

            if (WIFEXITED(status1) && WIFEXITED(status2)) {
                read(pipe1[0], sorted, n1 * sizeof(int));
                read(pipe2[0], sorted + n1, n2 * sizeof(int));
                close(pipe1[0]);
                close(pipe2[0]);

                // Merge the two sorted halves into 'sorted' array
                // You can implement a merge function or use a library function for merging.

                // Print the unsorted and sorted arrays
                //printf("Unsorted array: ");
                for (int i = 0; i < n; i++) {
                    printf("%d ", u[i]);
                }
                printf("\n");

                //printf("Sorted array: ");
                for (int i = 0; i < n; i++) {
                    printf("%d ", sorted[i]);
                }
                printf("\n");
            } else {
                fprintf(stderr, "Child processes did not terminate properly.\n");
                return 1;
            }
        }
    }

    return 0;
}