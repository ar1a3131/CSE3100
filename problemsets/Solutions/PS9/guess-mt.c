#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "guess-mt.h"

gmn_t* gmn_init(int val)
{
    gmn_t* pg = malloc(sizeof(gmn_t));
    pg->value = val;
    pg->guess = 0;     /* number guessed by the parent */
    pg->result = 2;    /* -1, 0 or 1 depending on the guess; initialized to arbitrary value of 2*/
    pg->status = 0;    /* Predicate to check if a guess is made */
    snprintf(pg->message, MSG_BUF_SIZE, "Guess a number between 1 and %d.\n", MAX_VALUE); 
    return pg;
}  

int gmn_check(gmn_t *pg)
{
    if (pg->value == pg->guess) {
        snprintf(pg->message, MSG_BUF_SIZE, "Congrats! You guessed the number %d.\n", pg->value);  
        return 0; /* Guess is correct! */
    }
    if (pg->value > pg->guess) {
        snprintf(pg->message, MSG_BUF_SIZE, "Your guess is smaller than the value\n");
        return 1;   /* Guess is smaller than the value! */
    }
    snprintf(pg->message, MSG_BUF_SIZE, "Your guess is greater than the value\n");   
    return -1;      /* Guess is greater than the value! */
}

