#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "matrix.h"

// THE PARAGRAPH BELOW IS TO EXPLAIN THE LOGIC. IF YOU DO NOT NEED THE LOGIC, YOU DO NOT NEED TO READ IT.

// This program leverages multithreading to perform matrix addition efficiently. The addMatrix_thread
// function orchestrates the process by dividing the task among multiple threads. Each thread independently 
// executes the thread_main function, responsible for adding a subset of rows from matrices m and n. This 
// subset is determined by the thread's unique identifier and the total number of threads. By dividing the 
// work in this manner, each thread can work concurrently on its designated subset, significantly improving 
// the overall performance. The thread_main function itself operates within a loop, ensuring that each thread 
// contributes to the summation of the matrices by adding elements row-wise. The coordination between threads 
// is facilitated through a structure (thread_arg_t) that encapsulates thread-specific information, such as matrices 
// and thread identifiers. This structured approach to multithreading enhances readability, maintainability, and 
// thread safety, preventing data hazards by isolating the processing of distinct rows. Upon completion of the 
// parallel computations, the program waits for all threads to finish before returning the result—a matrix 
// containing the sum of m and n.

// Create a new matrix of size nrows x ncols
TMatrix * newMatrix(unsigned int nrows, unsigned int ncols) {
    if (!nrows || !ncols)
        return NULL;

    TMatrix *newM = malloc(sizeof(TMatrix)); // Allocate memory for the matrix structure
    int **data = malloc(nrows * sizeof(int *)); // Allocate memory for row pointers
    int *storage = malloc(nrows * ncols * sizeof(int)); // Allocate memory for matrix elements

    if (newM == NULL || data == NULL || storage == NULL) {
        // Error checking: free allocated memory and return NULL if allocation fails
        free(newM);
        free(data);
        free(storage);
        return NULL;
    }

    // Set row array pointers
    for (size_t i = 0; i < nrows; i++)
        data[i] = storage + i * ncols;

    newM->nrows = nrows;
    newM->ncols = ncols;
    newM->data = data;

    return newM;
}

// Deallocate memory used by a matrix
void freeMatrix(TMatrix *m) {
    if (m == NULL)
        return;

    if (m->data) {
        free(m->data[0]); // Free storage allocated for data
        free(m->data);    // Free the array of pointers to row pointers
    }

    free(m); // Free the matrix structure itself
}

// Fill the matrix with random int values
TMatrix * fillMatrix(TMatrix *m) {
    if (m == NULL || m->data == NULL)
        return m;

    srand(3100);
    unsigned int i, j;

    for (i = 0; i < m->nrows; i++)
        for (j = 0; j < m->ncols; j++)
            m->data[i][j] = (int)rand() % 100;

    return m;
}

// Print the elements in a matrix
void printMatrix(TMatrix *m) {
    if (m == NULL)
        return;

    for (unsigned int i = 0; i < m->nrows; i++) {
        for (unsigned int j = 0; j < m->ncols; j++)
            printf("%5d", m->data[i][j]);
        printf("\n");
    }
    printf("\n");
}

// Compare two matrices m and n
int compareMatrix(TMatrix *m, TMatrix *n) {
    if (m == NULL || n == NULL)
        return -1;

    unsigned i, j;
    for (i = 0; i < m->nrows; i++)
        for (j = 0; j < m->ncols; j++)
            if (m->data[i][j] != n->data[i][j]) {
                fprintf(stderr, "element[%u][%u]  %d  %d.\n", i, j, m->data[i][j], n->data[i][j]);
                return 1;
            }

    return 0;
}

// Add two matrices m and n and return their sum. This function is implemented using a single thread
TMatrix * addMatrix(TMatrix *m, TMatrix *n) {
    if (m == NULL || n == NULL || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;
    for (unsigned int i = 0; i < m->nrows; i++)
        for (unsigned int j = 0; j < m->ncols; j++)
            t->data[i][j] = m->data[i][j] + n->data[i][j];

    return t;
}

// Function executed by threads to compute a subset of the total number of rows
static void *thread_main(void *p_arg) {
    // Extract thread arguments
    thread_arg_t *p = p_arg;

    // Matrix pointers and loop variables
    TMatrix *m, *n, *t;
    unsigned int i;
    int nbT;

    // Assign thread arguments to local variables
    m = p->m;
    n = p->n;
    t = p->t;
    i = p->id;
    nbT = p->num_threads;

    // Each thread adds a subset of the total number of rows
    while (i < m->nrows) {
        // Loop through the columns of the matrices and add corresponding elements
        for (unsigned int j = 0; j < m->ncols; j++)
            t->data[i][j] = m->data[i][j] + n->data[i][j];
        // Move to the next row, considering the total number of threads
        i += nbT;
    }

    return NULL;
}


// Add two matrices m and n and return their sum using nbT threads
TMatrix *addMatrix_thread(int nbT, TMatrix *m, TMatrix *n) {
    // Check for invalid input or mismatched matrix dimensions
    if (m == NULL || n == NULL || m->nrows != n->nrows || m->ncols != n->ncols)
        return NULL;

    // Create a new matrix to store the result
    TMatrix *t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    // Create an array to store thread IDs and another for thread arguments
    pthread_t tids[nbT];
    thread_arg_t args[nbT];
    int status;

    // Loop to create threads
    for (int i = 0; i < nbT; i++) {
        // Set thread arguments with thread ID, number of threads, matrices m, n, and result matrix t
        args[i].id = i;
        args[i].num_threads = nbT;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;

        // Create threads and check for errors
        status = pthread_create(&tids[i], NULL, thread_main, &args[i]);
        if (status) {
            fprintf(stderr, "pthread_create() returned %d\n", status);
            exit(1);
        }
    }

    // Loop to wait for threads to complete
    for (int i = 0; i < nbT; i++) {
        // Wait for thread completion and check for errors
        status = pthread_join(tids[i], NULL);
        if (status) {
            fprintf(stderr, "pthread_join() returned %d\n", status);
            exit(1);
        }
    }

    return t;
}
