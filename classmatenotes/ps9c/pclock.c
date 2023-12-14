#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "guess-mt.h"

// THE PARAGRAPH BELOW IS TO EXPLAIN THE LOGIC. IF YOU DO NOT NEED THE LOGIC, YOU DO NOT NEED TO READ IT.

// This multithreaded program implements a number guessing game with one thread acting as the producer 
// and another as the consumer. The program uses guess-mt, encapsulated in the gmn_t structure, 
// to manage the guessing game's state, including the target value, current guess, result, status, and a message. 
// The producer thread (thread_p) continually generates guesses within a specified range, updating the range based 
// on the feedback from the consumer thread (thread_c). The consumer, in turn, checks the correctness of the guesses 
// using the gmn_check function from the guess-mt library. The program employs mutexes (pthread_mutex_t) and 
// conditional variables (pthread_cond_t) to synchronize access to shared data and signal when guesses or results 
// are ready for processing. The producer and consumer threads communicate through these shared variables, ensuring 
// thread-safe interactions. The producer produces guesses in a loop until the correct guess is made, and the consumer 
// processes each guess, providing feedback to guide subsequent guesses. The implementation leverages multithreading 
// to enhance performance, allowing parallel execution of guess generation and result processing.

// Structure to hold thread-specific data
typedef struct thread_arg_tag {
    gmn_t* sb;                    // Struct for the guessing game
    pthread_mutex_t mutex;        // Mutex to protect the struct
    pthread_cond_t cond_guess;    // Signal that a guess is ready
    pthread_cond_t cond_result;   // Signal that the result is ready
} thread_arg_t;

// Consumer thread function
void *thread_c(void* consumer_thread_data);

// Producer thread function
void *thread_p(void* producer_thread_data);

// Main function - entry point of the program
int main(int argc, char* argv[]) {
    // Create a structure to hold thread-specific data and initialize the guessing game struct
    thread_arg_t arg;
    arg.sb = gmn_init(atoi(argv[1]));  // Initialize the guessing game struct

    // Initialize the mutex and condition variables in the thread-specific data structure
    pthread_mutex_init(&arg.mutex, NULL);
    pthread_cond_init(&arg.cond_guess, NULL);
    pthread_cond_init(&arg.cond_result, NULL);

    // Create two threads - one for the producer and another for the consumer
    pthread_t tidc, tidp;
    pthread_create(&tidc, NULL, thread_c, &arg);
    pthread_create(&tidp, NULL, thread_p, &arg);

    // Variables to hold the return values of the threads
    void *v1, *v2;

    // Wait for both threads to finish
    pthread_join(tidc, &v1);
    pthread_join(tidp, &v2);

    // Clean up resources - destroy the mutex and condition variables
    pthread_mutex_destroy(&arg.mutex);
    pthread_cond_destroy(&arg.cond_guess);
    pthread_cond_destroy(&arg.cond_result);

    // Exit the program with a success status
    return 0;
}


// Producer thread function
void* thread_p(void *producer_thread_data) {
    // Extract the thread-specific data from the argument
    thread_arg_t *arg = producer_thread_data;

    // Initialize the range for guessing
    int min = 1, max = MAX_VALUE;

    // Infinite loop to keep producing guesses
    while (1) {
        // Acquire the lock to safely access and modify shared data
        pthread_mutex_lock(&arg->mutex);

        // Wait while the consumer is still processing the previous guess
        while (arg->sb->status == 1)
            pthread_cond_wait(&arg->cond_result, &arg->mutex);

        // Check if the game has been won or needs to continue
        if (arg->sb->result == 0)
            break;

        // Adjust the guess range based on the result of the previous guess
        if (arg->sb->result == -1)
            max = arg->sb->guess - 1;
        if (arg->sb->result == 1)
            min = arg->sb->guess + 1;

        // Generate a new guess as the midpoint of the current range
        arg->sb->guess = (min + max) / 2;

        // Update the message with the current guess
        snprintf(arg->sb->message, MSG_BUF_SIZE, "My guess is %d.\n", arg->sb->guess);

        // Print the guess to the console
        printf("%s\n", arg->sb->message);

        // Update the status to indicate that a new guess is ready
        arg->sb->status = 1;

        // Signal the consumer that a new guess is available
        pthread_cond_signal(&arg->cond_guess);

        // Release the lock to allow the consumer to process the guess
        pthread_mutex_unlock(&arg->mutex);
    }

    // Exit the thread
    return NULL;
}

// Consumer thread function
void* thread_c(void *consumer_thread_data) {
    // Extract the thread-specific data from the argument
    thread_arg_t *arg = consumer_thread_data;

    int result;

    // Continue processing guesses until the game is won
    do {
        // Acquire the lock to safely access and modify shared data
        pthread_mutex_lock(&arg->mutex);

        // Wait while the producer hasn't generated a new guess
        while (arg->sb->status == 0)
            pthread_cond_wait(&arg->cond_guess, &arg->mutex);

        // Check if the current guess is correct or not
        arg->sb->result = gmn_check(arg->sb);
        result = arg->sb->result;

        // Print the message associated with the current guess
        printf("%s\n", arg->sb->message);

        // Update the status to indicate that the guess has been processed
        arg->sb->status = 0;

        // Signal the producer that the result has been processed
        pthread_cond_signal(&arg->cond_result);

        // Release the lock to allow the producer to generate a new guess
        pthread_mutex_unlock(&arg->mutex);

    } while (result != 0);  // Continue until the correct guess is made

    // Exit the thread
    return NULL;
}
