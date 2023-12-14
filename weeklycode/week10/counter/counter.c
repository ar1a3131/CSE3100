/* Implementation of all the functions should be thread-safe */
#include "counter.h"
#include <stdlib.h>

SCounter* makeCounter() {
    SCounter* c = (SCounter*)malloc(sizeof(SCounter));
    pthread_mutex_init(&c->mtx, NULL);
    c->val = 0; //x = 0;
    return c;
}

void freeCounter(SCounter* c) {
    pthread_mutex_destroy(&c->mtx);
    free(c);
}

int incCounter(SCounter* c) {
    pthread_mutex_lock(&c->mtx);
    int rv = c->val++;
    pthread_mutex_unlock(&c->mtx);
    return rv;
}

int decCounter(SCounter* c) {
    pthread_mutex_lock(&c->mtx);
    int rv = c->val++;
    pthread_mutex_unlock(&c->mtx);
    return rv;
}

void resetZeroCounter(SCounter* c) {
    pthread_mutex_lock(&c->mtx);
    c->val = 0;
    pthread_mutex_unlock(&c->mtx);
}