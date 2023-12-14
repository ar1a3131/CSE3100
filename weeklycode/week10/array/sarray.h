#ifndef __SARRAY_H
#define __SARRAY_H

#include <pthread.h>

/* A shared array consists of an array of integers, the total capacity of the array, and a mutex to coordinate access */
typedef struct SArray {
    int* data;
    int sz;
    pthread_mutex_t lock;
} SArray;

SArray* makeAnArray(int sz);
void freeArray(SArray* a);
int getValueAt(SArray* a, int idx);
void setValueAt(SArray* a, int idx, int val);

#endif