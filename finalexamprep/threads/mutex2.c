#include <stdio.h>
#include <pthread.h>

// Define a global variable to be protected by a mutex
int shared_variable = 0;

// TO DO: Declare a mutex variable here
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment_function(void* arg) {
    // TO DO: Acquire the mutex before modifying the shared variable
    pthread_mutex_lock(&mutex);

    // TO DO: Increment the shared variable
    shared_variable++;

    // TO DO: Release the mutex after modifying the shared variable
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    // TO DO: Initialize the mutex

    pthread_t thread1, thread2;

    // TO DO: Create two threads, both calling increment_function
    pthread_create(&thread1, NULL, increment_function, NULL);
    pthread_create(&thread2, NULL, increment_function, NULL);

    // TO DO: Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // TO DO: Destroy the mutex
    pthread_mutex_destroy(&mutex);

    // Print the final value of the shared variable
    printf("Final shared variable value: %d\n", shared_variable);

    return 0;
}