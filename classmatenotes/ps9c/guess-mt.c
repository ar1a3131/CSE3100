#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "guess-mt.h"

// Initialize the guessing game struct with the provided value
gmn_t* gmn_init(int val) {
    // Allocate memory for the guessing game struct
    gmn_t* pg = malloc(sizeof(gmn_t));
    pg->value = val;
    pg->guess = 0;      // Number guessed by the parent
    pg->result = 2;     // -1, 0, or 1 depending on the guess; initialized to arbitrary value of 2
    pg->status = 0;     // Predicate to check if a guess is made
    // Set the initial message prompting the user to guess a number
    snprintf(pg->message, MSG_BUF_SIZE, "Guess a number between 1 and %d.\n", MAX_VALUE);
    return pg;
}

// Check the guessed number against the actual value and update the message accordingly
int gmn_check(gmn_t *pg) {
    if (pg->value == pg->guess) {
        // If the guess is correct, update the message and return 0
        snprintf(pg->message, MSG_BUF_SIZE, "Congrats! You guessed the number %d.\n", pg->value);
        return 0; // Guess is correct!
    }

    if (pg->value > pg->guess) {
        // If the guess is smaller than the value, update the message and return 1
        snprintf(pg->message, MSG_BUF_SIZE, "Your guess is smaller than the value.\n");
        return 1; // Guess is smaller than the value!
    }

    // If the guess is greater than the value, update the message and return -1
    snprintf(pg->message, MSG_BUF_SIZE, "Your guess is greater than the value.\n");
    return -1; // Guess is greater than the value!
}
