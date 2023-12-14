#include "buffer.h"
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

SBuffer* makeBuffer(int sz)
{
   SBuffer* b = malloc(sizeof(SBuffer));
   b->sz = sz;
   b->enter = b->leave = 0;
   b->nb = 0;
   b->data = malloc(sizeof(Task)*sz);
   return b;
}

void bufferEnQueue(SBuffer* b,Task t)
{
   assert(b->enter >=0 && b->enter < b->sz);
   assert(b->nb != b->sz);
   b->data[b->enter++] = t;
   if (b->enter == b->sz) b->enter = 0;
   b->nb += 1;
   assert(b->nb >= 0);
}

Task bufferDeQueue(SBuffer* b)
{
   assert(b->leave >=0 && b->leave < b->sz);
   assert(b->nb > 0);
   Task rv = b->data[b->leave++];
   if (b->leave == b->sz) b->leave = 0;
   b->nb -= 1;
   assert(b->nb >= 0);
   return rv;
}