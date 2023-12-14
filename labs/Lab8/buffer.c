#include "buffer.h"
#include <sys/mman.h>
#include <sys/stat.h>        
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
}

void bufferEnQueue(SBuffer* b,Task t) {
   /* TODO: add Task t to the shared buffer b. 
    * Do not use synchronization here. */
}

Task bufferDeQueue(SBuffer* b) {
   /* TODO: deQueue and return a task from the shared buffer b. 
    * Do not use synchronization here. */
}

SBuffer* setupBuffer(char* zone,int nbTasks) {
   /* TODO: 
    * Create a memory mapping of the shared object "zone" capable of holding SBuffer and nbTasks entries.
    * Initialize the SBuffer and return the address of the shared buffer. 
    */
}

SBuffer* getBuffer(char* zone,int nbTasks) {
   /* TODO: Given a shared memory object "zone", retrieve it and return the address of the mapping. */ 
}

void tearDownBuffer(char* zone,SBuffer* b) {
   /* TODO: Release the resources (map/shared object) needed for the shared buffer b */
}
