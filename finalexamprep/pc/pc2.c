#include <stdio.h>
#include <pthread.h>

// Define a global variable to be updated by two threads
int shared_data = 0;

// TO DO: Declare a mutex and a condition variable here
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer_function(void* arg) {
    // TO DO: Acquire the mutex before updating the shared data
    pthread_mutex_lock(&mutex);

    // TO DO: Update the shared data
    shared_data = 42;

    // TO DO: Signal the condition variable
    pthread_cond_signal(&cond);

    // TO DO: Release the mutex
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* consumer_function(void* arg) {
    // TO DO: Acquire the mutex
    pthread_mutex_lock(&mutex);

    // TO DO: Wait for the condition variable to be signaled
    while (shared_data == 0) {
        pthread_cond_wait(&cond, &mutex);
    }

    // TO DO: Consume the shared data
    printf("Consumed: %d\n", shared_data);

    // TO DO: Release the mutex
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    // TO DO: Initialize the mutex and condition variable

    pthread_t producer_thread, consumer_thread;

    // TO DO: Create the producer and consumer threads
    pthread_create(&producer_thread, NULL, producer_function, NULL);
    pthread_create(&consumer_thread, NULL, consumer_function, NULL);

    // TO DO: Wait for both threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // TO DO: Destroy the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}