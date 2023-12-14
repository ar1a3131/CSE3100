#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to compare integers for qsort
int compare_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
      printf("usage is: prep <number>\n");
      exit(1);
   }
    int n = atoi(argv[1]);  // Change this to your desired even number of integers
    if (n % 2 != 0) {
        printf("n must be an even number.\n");
        return 1;
    }

    int u[n];
    int sorted[n];

    // Generate random integers and save them in array u
    for (int i = 0; i < n; i++) {
        u[i] = rand();
    }

    // Create pipes for communication with child processes
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork two child processes
    pid_t pid1, pid2;
    if ((pid1 = fork()) == 0) {
        // Child process 1
        close(pipe1[0]);  // Close the read end
        qsort(u, n / 2, sizeof(int), compare_int);
        write(pipe1[1], u, (n / 2) * sizeof(int));
        close(pipe1[1]);
        exit(0);
    } else if ((pid2 = fork()) == 0) {
        // Child process 2
        close(pipe2[0]);  // Close the read end
        qsort(u + (n / 2), n / 2, sizeof(int), compare_int);
        write(pipe2[1], u + (n / 2), (n / 2) * sizeof(int));
        close(pipe2[1]);
        exit(0);
    } else {
        // Parent process
        close(pipe1[1]);  // Close the write end of pipe1
        close(pipe2[1]);  // Close the write end of pipe2

        // Read sorted data from child process 1 and child process 2
        read(pipe1[0], sorted, (n / 2) * sizeof(int));
        read(pipe2[0], sorted + (n / 2), (n / 2) * sizeof(int));

        close(pipe1[0]);
        close(pipe2[0]);

        // Wait for child processes to finish
        wait(NULL);
        wait(NULL);

        qsort(sorted, n, sizeof(int), compare_int);

        // Print unsorted and sorted arrays
        printf("Unsorted Array:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", u[i]);
        }
        printf("\n");

        printf("Sorted Array:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", sorted[i]);
        }
        printf("\n");
    }

    return 0;
}

/*int i = 0, j = n/2, k = 0;
    while (i < n/2 && j < n) {
        if (u[i] < u[j]) {
            sorted[k++] = u[i++];
        } else {
            sorted[k++] = u[j++];
        }
    }
    while (i < n/2) {
        sorted[k++] = u[i++];
    }
    while (j < n) {
        sorted[k++] = u[j++];
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");*/