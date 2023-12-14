#ifndef __COUNTER_H
#define __COUNTER_H

#include <pthread.h>

typedef struct SCounterTag {
    pthread_mutex_t mtx;
    int val; //same as x
} SCounter;

SCounter* makeCounter();
void freeCounter(SCounter*);
int incCounter(SCounter*);
int decCounter(SCounter*);
void resetZeroCounter(SCounter*);

#endif