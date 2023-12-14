/* Implementing a shared array - coarse grained locking */

#include "sarray.h"
#include <stdlib.h>
#include <pthread.h>

/* Allocate space for the array and the integers it would store. Initialize the mutex */
SArray* makeAnArray(int sz) {
    SArray* p = (SArray*)malloc(sizeof(SArray));
    p->sz = sz;
    p->data = (int*)calloc(sz, sizeof(int));
    pthread_mutex_init(&p->lock, NULL);
    return p;
}

/* Free the array and destroy the mutex. */
void freeArray(SArray* a) {
    pthread_mutex_destroy(&a->lock);
    free(a->data);
    free(a);
}

/* Obtain the value at index idx */
int getValueAt(SArray* a, int idx) {
    pthread_mutex_lock(&a->lock);
    int rv = a->data[idx];
    pthread_mutex_unlock(&a->lock);
    return rv;
}

/* Set the value val at index idx */
void setValueAt(SArray* a, int idx, int val) {
    pthread_mutex_lock(&a->lock);
    a->data[idx] = val;
    pthread_mutex_unlock(&a->lock);
}