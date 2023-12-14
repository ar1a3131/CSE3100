/* This is the leader program - comments are given to help you understand what the leader does */
/* DO NOT MODIFY THIS PROGRAM */

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "buffer.h"

int main()
{
   SBuffer* requests = setupBuffer("/requests",100000);                 // set up a buffer for the requests
   SBuffer* answers = setupBuffer("/answers",200);                      // set up a buffer for the answers
   printf("Number of answer slots:%ld\n",answers->sz);
   sem_t* slots = sem_open("/semaSlots",O_CREAT,S_IRWXU,answers->sz);   // create a semaphore for slots
   sem_t* tasks = sem_open("/semaTasks",O_CREAT,S_IRWXU,0);             // create a semaphore for tasks
   sem_t* asw = sem_open("/semaASW",O_CREAT,S_IRWXU,0);                 // create a semaphore for answers
   sem_t* lock  = sem_open("/semaPullLock",O_CREAT,S_IRWXU,1);          // create a semaphore for lock
   for(int i=0;i< requests->sz;i++) {                                   // Only the leader accesses the requests buffer to enqueue tasks
      Task t = {0,i,-1};
      bufferEnQueue(requests,t);
      sem_post(tasks);                                                  // raise the flag when a task has been added
   }
   long long check = 0;
   for(int i=0;i< requests->sz;i++) {                                   // read responses from the answers buffer
      sem_wait(asw);                                                    // ensure there is an answer to read. If not, wait
      sem_wait(lock);                                                   // get the lock to read the answer buffer
      Task t = bufferDeQueue(answers);
      sem_post(lock);                                                   // release lock once read
      sem_post(slots);                                                  // raise a flag to increment the number of slots available
      //printf("Worker produced (%d,%lld,%lld)\n",t.worker,t.query,t.answer);
      check += t.answer;
   }
   printf("Final: %lld\n",check);
   tearDownBuffer("/requests",requests);                                // destroy the memory maps            
   tearDownBuffer("/answers",answers);
   sem_close(lock);                                                     // close the semaphores
   sem_close(tasks);
   sem_close(slots);
   sem_close(asw);
   return 0;
}
