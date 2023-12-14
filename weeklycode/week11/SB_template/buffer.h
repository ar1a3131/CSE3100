#ifndef __BUFFER_H
#define __BUFFER_H

#include <stdlib.h>
#include <pthread.h>

typedef struct Task {
   pthread_t worker;
   long long   query;
   long long   answer;
} Task;

typedef struct SBuffer {
   size_t sz;
   int enter,leave,nb;
   Task*  data;
} SBuffer;

SBuffer* makeBuffer(int sz);
void bufferEnQueue(SBuffer* b,Task t);
Task bufferDeQueue(SBuffer* b);

#endif