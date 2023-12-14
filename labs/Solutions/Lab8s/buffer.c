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

SBuffer* makeBuffer(void* z,int sz,size_t mapSize) {
   /* TODO
    * Initialize and return a shared buffer pointer that can be held at address z.
    * The buffer should have a capacity sz and size of mapSize.
    */
   SBuffer* b = z; //the buffer starts at this address in the memory. Therefore, assign the value of z to pointer to buffer
   b->sz = sz;      // capacity
   b->enter = b->leave = 0; // the buffer is empty initially
   b->nb = 0;               // number of tasks = 0
   b->mapSize = mapSize;
   return b;
}

void bufferEnQueue(SBuffer* b,Task t) {
   /* TODO: add Task t to the shared buffer b. 
    * Do not use synchronization here. */
   assert(b->enter >=0 && b->enter < b->sz); // ensure the index is in the valid range
   assert(b->nb != b->sz);                   // make sure the buffer is not full
   b->data[b->enter++] = t;                  // add the task in the buffer
   if (b->enter == b->sz) b->enter = 0;      // wrap-around
   b->nb += 1;
   assert(b->nb >= 0);
}

Task bufferDeQueue(SBuffer* b) {
   /* TODO: deQueue and return a task from the shared buffer b. 
    * Do not use synchronization here. */
   assert(b->leave >=0 && b->leave < b->sz);   // ensure that the index is valid
   assert(b->nb > 0);                           // make sure there is at least one task to dequeue
   Task rv = b->data[b->leave++];               // remove the task and increment the index
   if (b->leave == b->sz) b->leave = 0;         // wrap-around
   b->nb -= 1;
   assert(b->nb >= 0);
   return rv;
}

SBuffer* setupBuffer(char* zone,int nbTasks) {
   /* TODO: 
    * Create a memory mapping of the shared object "zone" capable of holding SBuffer and nbTasks entries.
    * Initialize the SBuffer and return the address of the shared buffer. 
    */
   size_t nbb = sizeof(Task) * nbTasks + sizeof(SBuffer);   // get the size of the buffer
   size_t nbPages = 1 + nbb/4096;                           // compute the number of pages needed
   size_t mapSize = nbPages * 4096;                         // compute the size of the mapping
   printf("Mapping: %ld\n",mapSize);
   int md = shm_open(zone,O_RDWR|O_CREAT,S_IRWXU);          // create a shared memory object
   ftruncate(md,mapSize); 
   void* ptr = mmap(NULL,mapSize,
                    PROT_READ|PROT_WRITE,
                    MAP_SHARED|MAP_FILE,
                    md,0);                                  // create a mapping
   if (ptr == MAP_FAILED) {
      printf("mmap failure: %s\n",strerror(errno));
      exit(1);
   }
   close(md);
   return makeBuffer(ptr,nbTasks,mapSize);
}

SBuffer* getBuffer(char* zone,int nbTasks) {
   /* TODO: Given a shared memory object "zone", retrieve it and return the address of the mapping. */ 
   size_t nbb = sizeof(Task) * nbTasks + sizeof(SBuffer);
   size_t nbPages = 1 + nbb/4096;
   size_t mapSize = nbPages * 4096;
   int md = shm_open(zone,O_RDWR,S_IRWXU);                  // open the mapping in read/write mode and obtain the pointer
   void* ptr = mmap(NULL,mapSize,
                    PROT_READ|PROT_WRITE,
                    MAP_SHARED|MAP_FILE,
                    md,0);
   close(md);
   return ptr;
}

void tearDownBuffer(char* zone,SBuffer* b) {
   /* TODO: Release the resources (map/shared object) needed for the shared buffer b */
   munmap(b,b->mapSize);                                // unmap and unlink the object
   shm_unlink(zone);
}
